#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() 
{
    int pong_fd[2];
    int ping_fd[2];
    pid_t pid;
    char message[50];
    size_t i = 0;

    if(-1 == pipe(pong_fd)) 
    {
        perror("pong pipe creation failed");
        exit(EXIT_FAILURE);
    }

    if(-1 == pipe(ping_fd)) 
    {
        perror("ping pipe creation failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if(-1 == pid) 
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if(0 == pid) 
    {
        close(ping_fd[1]);  
        close(pong_fd[0]);  

        for(i = 0; i < 50; ++i) 
        {
            read(ping_fd[0], message, sizeof(message));
            printf("Pong received: %s", message);
            write(pong_fd[1], "Pong send:\n", strlen("Pong send:\n") + 1);
        }

        close(ping_fd[0]);  
        close(pong_fd[1]);  
    } 
    else 
    {
        close(ping_fd[0]);  
        close(pong_fd[1]);

        write(ping_fd[1], "ping send to pong\n", strlen("ping send to pong\n") + 1);

        for(i = 0; i < 50; ++i) 
        {
            read(pong_fd[0], message, sizeof(message));
            printf("Ping received: %s\n", message);
            write(ping_fd[1], "Ping sent:\n", strlen("Ping send:\n") + 1);
        }

        close(ping_fd[1]);  
        close(pong_fd[0]); 

        waitpid(pid, NULL, 0);
    }

    return 0;
}