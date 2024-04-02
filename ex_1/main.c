#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

void readFile()
{
    int fileToRead = open("fileToRead", O_CREAT | O_RDWR, 0666);
    if (fileToRead > 0)
    {
        printf("fd = %d\n", fileToRead);
    }
    if (fileToRead == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int fileToWrite = open("fileToWrite", O_CREAT | O_RDWR, 0666);
    if (fileToWrite > 0)
    {
        printf("fd = %d\n", fileToWrite);
    }
    if (fileToWrite == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    size_t len = 1024;
    char buf[len];
    ssize_t fileReader;
    ssize_t fileWriter;

    do
    {
        fileReader = read(fileToRead, buf, len);
        if (fileReader == -1)
        {
            if (errno == EINTR)
                continue;
            close(fileToRead);
            perror("read");
            break;
        }

        if (fileReader < len) //записываем последний кусок файла
        {
            ssize_t fileWriter = write(fileToWrite, buf, fileReader);
            if (fileWriter == -1)
            {
                close(fileToWrite);
                perror("write");
                exit(EXIT_FAILURE);
            }
            break;
        }

        ssize_t fileWriter = write(fileToWrite, buf, len);
        if (fileWriter == -1)
        {
            close(fileToWrite);
            perror("write");
            exit(EXIT_FAILURE);
        }
    } while (fileReader > 0);

    close(fileToRead);
    close(fileToWrite);
}

int main()
{
    readFile();

    return 0;
}