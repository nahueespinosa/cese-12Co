#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 300

#define DATA_PREFIX "DATA:"
#define PREFIX_SIZE 5

#define SIGN_1_MSG  "SIGN:1"
#define SIGN_2_MSG  "SIGN:2"

#define TOTAL_SIZE  300

int32_t fd;

void sigusr_handler(int sig);

int main(void)
{
    struct sigaction sa;
    char outputBuffer[TOTAL_SIZE];
	uint32_t bytesWrote;
	int32_t returnCode;

    /* Write DATA: prefix in output buffer */
    strncpy(outputBuffer, DATA_PREFIX, PREFIX_SIZE);

    /* Create named fifo. -1 means already exists so no action if already exists */
    if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1 )
    {
        printf("Error creating named fifo: %d\n", returnCode);
        exit(1);
    }

    /* Open named fifo. Blocks until other process opens it */
	printf("waiting for readers...\n");
	if ( (fd = open(FIFO_NAME, O_WRONLY) ) < 0 )
    {
        printf("Error opening named fifo file: %d\n", fd);
        exit(1);
    }

    sa.sa_handler = sigusr_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    
    /* open syscalls returned without error -> other process attached to named fifo */
	printf("got a reader--type some stuff\n");

    /* Loop forever */
	while (1)
	{
        /* Get some text from console */
		if (fgets(outputBuffer + PREFIX_SIZE, BUFFER_SIZE, stdin) != NULL)
        {
            /* Write buffer to named fifo. Strlen - 1 to avoid sending \n char */
            if ((bytesWrote = write(fd, outputBuffer, strlen(outputBuffer)-1)) == -1)
            {
                perror("write");
            }
            else
            {
                printf("writer: wrote %d bytes\n", bytesWrote);
            }
        }
	}
	return 0;
}

void sigusr_handler(int sig)
{
    if (sig == SIGUSR1)
    {
        write(fd, SIGN_1_MSG, sizeof(SIGN_1_MSG));
    }

    if (sig == SIGUSR2)
    {
        write(fd, SIGN_2_MSG, sizeof(SIGN_2_MSG));
    }
    
}