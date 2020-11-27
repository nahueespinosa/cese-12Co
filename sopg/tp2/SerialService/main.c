#include <stdio.h>
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
pthread_t tcp_service_thread;
pthread_t serial_service_thread;

/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec);

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

	/* Initialize signal handler */

	struct sigaction sa;
	sa.sa_handler = sig_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGINT, &sa, NULL) < 0)
	{
		perror("sigaction() error");
		exit(1);
	}

	if (sigaction(SIGTERM, &sa, NULL) < 0)
	{
		perror("sigaction() error");
		exit(1);
	}

	signal(SIGPIPE, SIG_IGN);

	/* Initialize server IP address */

	bzero((char *)&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(TCP_PORT);

	if (inet_pton(AF_INET, SERVER_IP, &(serveraddr.sin_addr)) <= 0)
	{
		perror("invalid server IP\n");
		exit(1);
	}

	/* Create TCP socket */

	s = socket(AF_INET, SOCK_STREAM, 0);

	if (bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		close(s);
		perror("bind() error");
		exit(1);
	}

	if (listen(s, 10) < 0)
	{
		close(s);
		perror("listen() error");
		exit(1);
	}

	while (1)
	{
		printf("Esperando conexion desde InterfaceService\n");

		addr_len = sizeof(struct sockaddr_in);
		if ((fd = accept(s, (struct sockaddr *)&clientaddr, &addr_len)) < 0)
		{
			perror("accept() error");
			break;
		}

		char ipClient[32];
		inet_ntop(AF_INET, &(clientaddr.sin_addr), ipClient, sizeof(ipClient));
		printf("Nueva conexion desde %s\n", ipClient);

		printf("Conectando con la EDU-CIAA\n");

		if (serial_open(SERIAL_PORT, SERIAL_BAUDRATE) != 0)
		{
			perror("serial_open() error");
			break;
		}

		block_signals();	// Only the main thread handles the signals

		if (pthread_create(&tcp_service_thread, NULL, tcp_service_routine, &fd) != 0)
		{
			perror("pthread_create() error");
			break;
		}

		if (pthread_create(&serial_service_thread, NULL, serial_service_routine, &fd) != 0)
		{
			perror("pthread_create() error");
			break;
		}

		unblock_signals();

		printf("Esperando comandos\n");

		pthread_join(tcp_service_thread, NULL);
		pthread_cancel(serial_service_thread);

		serial_close();

		printf("\nCliente desconectado\n");
	}

	close(s);
	exit(1);
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

		msleep(10);
	}
}

void sig_handler(int sig)
{
	if (sig == SIGTERM || sig == SIGINT)
	{
		write(1, "\nCerrando el servidor\n", 22);

		pthread_cancel(tcp_service_thread);
		pthread_cancel(serial_service_thread);

		serial_close();
		close(s);
		exit(0);
	}
}

int msleep(long msec)
{
	struct timespec ts;
	int res;

	if (msec < 0)
	{
		errno = EINVAL;
		return -1;
	}

	ts.tv_sec = msec / 1000;
	ts.tv_nsec = (msec % 1000) * 1000000;

	do
	{
		res = nanosleep(&ts, &ts);
	} while (res && errno == EINTR);

	return res;
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
