#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 16

void print(unsigned char *buffer, ssize_t bytes_read, ssize_t offset) {
    printf("%08lx: ", offset); // the offset
    ssize_t i;
    for (i = 0; i < bytes_read; ++i) { //each byte read
        printf("%02x", buffer[i]); //make sure theres leading zeros!

        if (i % 2) { //space for every 2 hexadecimals
            printf(" ");
        }

        if (!(buffer[i] >= 32 && buffer[i] <= 126)) {
            buffer[i] = '.';
        }
    }

    while (i < 16) { //space for alignment if num of bytes read is less than total buffer size
        printf("  ");

        if (i % 2 == 1) {
            printf(" ");
        }
        i++;
    }
    printf(" ");
    for (ssize_t i = 0; i < bytes_read; ++i) {
        printf("%c", buffer[i]);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    //int fd; //file descriptor
    //ssize_t n; //store # bytes read
    ssize_t offset = 0;
    unsigned char buffer[BUFFER_SIZE];

    int fd;
    if (argc == 2) {

        fd = open(argv[1], O_RDONLY);
        if (fd == -1) {
            return -1;
        }
    } else {
        fd = STDIN_FILENO; //read from standard input
            //fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
            //return -1;
    }

    /*while ((n = read(fd, buffer, BUFFER_SIZE)) > 0) {
        print(buffer, n, offset);
        offset += n;
    }
    if (argc == 2) {
        close(fd);
    }
    return 0;*/

    while (1) { // this infinite loop will handle throttled input
        ssize_t bytesRead = 0;
        while (bytesRead < BUFFER_SIZE) {
            ssize_t numBytes = read(fd, buffer + bytesRead, (size_t) (BUFFER_SIZE - bytesRead));
            if (numBytes <= 0) {
                break;
            }
            bytesRead += numBytes;
        }

        if (bytesRead > 0) {
            print(buffer, bytesRead, offset);
            offset += bytesRead;
        }
        if (bytesRead < BUFFER_SIZE) {
            break; //break if less than buffersize (EOF)
        }
    }
}
