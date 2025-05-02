#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define DATA_SIZE 32768

int main() 
{
    int pipefd[2];
    if (pipe(pipefd) == -1) 
    {
        perror("pipe");
        return 1;
    }

    pid_t cpid = fork();
    if (cpid == -1) 
    {
        perror("fork");
        return 1;
    }

    if (cpid == 0) 
    { 
        close(pipefd[0]);
        clock_t start = clock();

        for (int index = 0; index < DATA_SIZE; ++index) 
        {
            write(pipefd[1], "t", 1);
        }
        
        clock_t end = clock();
        close(pipefd[1]);

        printf("Writing time in pipe: %f секунд\n", (double)(end - start) / CLOCKS_PER_SEC);
    } 
    else 
    {
        close(pipefd[1]);
        clock_t start = clock();

        char buf = '\0';

        for (int index = 0; index < DATA_SIZE; ++index) 
        {
            read(pipefd[0], &buf, 1);
        }

        clock_t end = clock();
        close(pipefd[0]);

        wait(NULL); 
        printf("Reading time from pipe: %f секунд\n", (double)(end - start) / CLOCKS_PER_SEC);
    }

    return 0;
}