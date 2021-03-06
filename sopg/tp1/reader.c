#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

#define FIFO_NAME       "myfifo"
#define BUFFER_SIZE     300

#define SIGN_FILE_NAME  "Sign.txt"
#define DATA_FILE_NAME  "Log.txt"

#define DATA_PREFIX     "DATA:"
#define SIGN_PREFIX     "SIGN:"

#define PREFIX_SIZE     5

int main(void)
{
	uint8_t inputBuffer[BUFFER_SIZE];
	int32_t bytesRead, returnCode, fd;
    FILE *fp_sign, *fp_data;

    /* Append to a file. The file is created if it does not exist. */
    if ( (fp_sign = fopen(SIGN_FILE_NAME, "a")) == NULL )
    {
        printf("Error opening %s file for writing\n", SIGN_FILE_NAME);
    }

    /* Append to a file. The file is created if it does not exist. */
    if ( (fp_data = fopen(DATA_FILE_NAME, "a")) == NULL )
    {
        printf("Error opening %s file for writing\n", DATA_FILE_NAME);
    }
    
    /* Create named fifo. -1 means already exists so no action if already exists */
    if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1  )
    {
        printf("Error creating named fifo: %d\n", returnCode);
        exit(1);
    }
    
    /* Open named fifo. Blocks until other process opens it */
	printf("waiting for writers...\n");
	if ( (fd = open(FIFO_NAME, O_RDONLY) ) < 0 )
    {
        printf("Error opening named fifo file: %d\n", fd);
        exit(1);
    }
    
    /* open syscalls returned without error -> other process attached to named fifo */
	printf("got a writer\n");

    /* Loop until read syscall returns a value <= 0 */
	do
	{
        /* read data into local buffer */
		if ((bytesRead = read(fd, inputBuffer, BUFFER_SIZE)) == -1)
        {
			perror("read");
        }
        else
		{
			inputBuffer[bytesRead] = '\0';
			printf("reader: read %d bytes: \"%s\"\n", bytesRead, inputBuffer);

            if (strncmp(inputBuffer, SIGN_PREFIX, PREFIX_SIZE) == 0) {
                fprintf(fp_sign, "%s\n", inputBuffer);
                fflush(fp_sign);
            }

            if (strncmp(inputBuffer, DATA_PREFIX, PREFIX_SIZE) == 0) {
                fprintf(fp_data, "%s\n", inputBuffer);
                fflush(fp_data);
            }
		}
	}
	while (bytesRead > 0);
    
    fclose(fp_sign);
    fclose(fp_data);

	return 0;
}
