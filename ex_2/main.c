#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <error.h>
#include <string.h>

void printDirs(const char* dirPath)
{
    DIR* dir = opendir(dirPath);
    if (dir == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    struct dirent* files;
    struct stat buf;
    char filePath[1000];
    while ((files = readdir(dir)) != NULL)
    {
        if (!strcmp(files->d_name, ".") || !strcmp(files->d_name, ".."))
            continue;
            
        strcpy(filePath, dirPath);
        strcat(filePath, "/");
        strcat(filePath, files->d_name);
        printf("%s\n", filePath);
        if (stat(filePath, &buf) == -1)
        {
            perror("stat");
        }
        if ((buf.st_mode & __S_IFMT) == __S_IFDIR && files->d_name[0] != '.')
        {
            printDirs(filePath);
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Error! Argc != 2\n");
        exit(EXIT_FAILURE);
    }

    printDirs(argv[1]);

    return 0;
}