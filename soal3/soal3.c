#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <limits.h>
#include <dirent.h>

pthread_t tid[1000]; 
pid_t child_id;
char cwd[1024] = {0};

int is_directory(const char* path) 
{
    struct stat path_stat; 
    stat(path, &path_stat); 
    return S_ISDIR(path_stat.st_mode); 
}

void *count(void *arg)
{
    if (!is_directory(arg))
    {
        char copy[1024] = {0};
        memset(copy, 0, 1024);

        char *slash, *dot;
        int x = 0, y = 0;
        char *arrs[4], *arrd[10], extension[100], temp[100];
        memset(arrs, 0, 4);
        memset(arrd, 0, 10);
        memset(extension, 0, 100);
        memset(temp, 0, 100);

        strcpy(copy, arg); 
        slash = strtok(copy, "/"); 
        while(slash != NULL)
        {
            arrs[x] = slash;
            x++;
            slash = strtok(NULL, "/");
        }
        
        strcpy(temp, arrs[x - 1]);
        dot = strtok(temp, ".");
        while(dot != NULL)
        {
        arrd[y] = dot;
            y++;
            dot = strtok(NULL, ".");
        }

        strcpy(extension, arrd[y-1]);
        for(int i=0 ; i < strlen(extension);i++)
        {
            extension[i] = tolower(extension[i]);
        }

        char temp1[1024] = {0};
        char directory[1024] = {0};
        memset(temp1, 0, 1024);
        memset(directory, 0, 1024);
        strcpy(temp1, "/home/salma/Documents/modul3/soal3/");
}
}
