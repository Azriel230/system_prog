#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

void readFile()
{
    int fileToRead = open ("fileToRead", O_CREAT | O_RDWR, 0666);
    if (fileToRead > 0) 
    {
        printf("fd = %d\n", fileToRead);
    }
    if (fileToRead == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    size_t total_size = 0;
    char* buf = malloc(1024 + total_size + 1);
    ssize_t fileReader;

    while (fileReader = read(fileToRead, buf + total_size, 1024))
    {
        if (fileReader == -1)
        {
            if (errno == EINTR)
                continue;
            close(fileToRead);
            perror("read");
            break;
        }
        total_size += fileReader;
        buf = realloc(buf, 1024 + total_size + 1);
    }

    buf[total_size] = 0;
    printf("buf = %s\n", buf);
    printf("total_size = %ld\n", total_size);
    close(fileToRead);

///////////////////////////////////////////////////////////////////////

    int fileToWrite = open ("fileToWrite", O_CREAT | O_RDWR, 0666);
    if (fileToWrite > 0) 
    {
        printf("fd = %d\n", fileToWrite);
    }
    if (fileToWrite == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    ssize_t fileWriter = write(fileToWrite, buf, total_size);
    if (fileWriter == -1)
    {
        close(fileToWrite);
        perror("write");
        exit(EXIT_FAILURE);
    }
    close(fileToWrite);
}







int main()
{
    readFile();

    return 0;
}