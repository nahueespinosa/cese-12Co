#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>

#include "SerialManager.h"

#define BUFFER_SIZE 128

#define TCP_PORT 10000
#define SERVER_IP "127.0.0.1"

#define SERIAL_BAUDRATE 115200
#define SERIAL_PORT 1

int s;
bool close_from_signal = false;
pthread_t tcp_service_thread;
pthread_t serial_service_thread;

/* msleep(): Block signals for this thread. */
void block_signals(void);

/* msleep(): Unblock signals for this thread. */
void unblock_signals(void);

/* SIGINT and SIGTERM signal handler. */
void sig_handler(int sig);

void *tcp_service_routine(void *arg);
void *serial_service_routine(void *arg);

int main()
{
	int fd;
	socklen_t addr_len;
	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;
	char ip_client[32];
	int main_status = EXIT_FAILURE;

	/* Initialize signal handler */
	struct sigaction sa;
	sa.sa_handler = sig_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGINT, &sa, NULL) < 0)
	{
		perror("sigaction(SIGINT) error");
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGTERM, &sa, NULL) < 0)
	{
		perror("sigaction(SIGTERM) error");
		exit(EXIT_FAILURE);
	}

	/* Ignore sigpipe to handle the client disconnect in the main loop */
	sa.sa_handler = SIG_IGN;

	if (sigaction(SIGPIPE, &sa, NULL) < 0)
	{
		perror("sigaction(SIGPIPE) error");
		exit(EXIT_FAILURE);
	}

	/* Initialize server IP address */
	bzero((char *)&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(TCP_PORT);

	if (inet_pton(AF_INET, SERVER_IP, &(serveraddr.sin_addr)) <= 0)
	{
		perror("invalid server IP\n");
		exit(EXIT_FAILURE);
	}

	/* Create TCP socket */
	s = socket(AF_INET, SOCK_STREAM, 0);

	if (s < 0)
	{
		perror("socket() error");
		exit(EXIT_FAILURE);
	}

	if (bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		close(s);
		perror("bind() error");
		exit(EXIT_FAILURE);
	}

	if (listen(s, 10) < 0)
	{
		close(s);
		perror("listen() error");
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		printf("Esperando conexion desde InterfaceService\n");

		addr_len = sizeof(struct sockaddr_in);
		if ((fd = accept(s, (struct sockaddr *)&clientaddr, &addr_len)) < 0)
		{
			/* Check if the user is trying to close the program */
			if (errno == EINTR && close_from_signal == true)
			{
				main_status = EXIT_SUCCESS;
			}
			else
			{
				perror("accept() error");
			}
			break;
		}

		inet_ntop(AF_INET, &(clientaddr.sin_addr), ip_client, sizeof(ip_client));
		printf("Nueva conexion desde %s\n", ip_client);

		printf("Conectando con la EDU-CIAA\n");

		if (serial_open(SERIAL_PORT, SERIAL_BAUDRATE) != 0)
		{
			perror("serial_open() error");
			break;
		}

		block_signals(); // Only the main thread handles the signals

		if (pthread_create(&tcp_service_thread, NULL, tcp_service_routine, &fd) != 0)
		{
			perror("pthread_create(&tcp_service_thread) error");
			break;
		}

		if (pthread_create(&serial_service_thread, NULL, serial_service_routine, &fd) != 0)
		{
			perror("pthread_create(&serial_service_thread) error");
			break;
		}

		unblock_signals(); // Only the main thread handles the signals

		printf("Esperando comandos\n");

		if (pthread_join(tcp_service_thread, NULL) < 0)
		{
			perror("pthread_join() error");
			break;
		}

		printf("\nCliente desconectado\n");

		pthread_cancel(serial_service_thread);

		if (pthread_join(serial_service_thread, NULL) < 0)
		{
			perror("pthread_join() error");
			break;
		}

		serial_close();
		close(fd);

		/* Check if the user is trying to close the program */
		if (close_from_signal == true)
		{
			main_status = EXIT_SUCCESS;
			break;
		}
	}

	printf("\nCerrando el servidor\n");

	/* Make sure everything is closed */
	close(s);
	close(fd);
	serial_close();

	return main_status;
}

void *tcp_service_routine(void *arg)
{
	int fd = *((int *)arg);
	char buffer[BUFFER_SIZE];
	int n;

	while (1)
	{
		n = read(fd, buffer, BUFFER_SIZE);

		if (n > 0)
		{
			printf(".");
			fflush(NULL);

			serial_send(buffer, n);
		}
		else
		{
			break;
		}
	}

	return (void *)0;
}

void *serial_service_routine(void *arg)
{
	int fd = *((int *)arg);
	char buffer[BUFFER_SIZE];
	int n;

	while (1)
	{
		n = serial_receive(buffer, BUFFER_SIZE);

		if (n > 0)
		{
			printf(":");
			fflush(NULL);

			if (write(fd, buffer, n) < 0)
			{
				break;
			}
		}

		usleep(10000);
	}

	return (void *)0;
}

void sig_handler(int sig)
{
	if (sig == SIGTERM || sig == SIGINT)
	{
		/* Try to cancel TCP thread if it is running */
		pthread_cancel(tcp_service_thread);

		/* Flag to close the program */
		close_from_signal = true;
	}
}

void block_signals(void)
{
	sigset_t set;
	int s;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGTERM);
	pthread_sigmask(SIG_BLOCK, &set, NULL);
}

void unblock_signals(void)
{
	sigset_t set;
	int s;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGTERM);
	pthread_sigmask(SIG_UNBLOCK, &set, NULL);
}
