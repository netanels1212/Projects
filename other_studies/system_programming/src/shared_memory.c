#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>

#define SHMSZ 1024

int main() 
{
    FILE *file;
    key_t key;
    int shmid;
    pid_t pid1;
    pid_t pid2;
    pid_t pid3;
    pid_t pid4;
    pid_t pid5;

    file = fopen("common_file.txt", "w+");

    if (file == NULL) {
        perror("Failed to create/open file");
        exit(1);
    }
    fclose(file);

    key = ftok("common_file.txt", 65);
    
    shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
    if (shmid < 0) 
    {
        perror("shmget");
        exit(1);
    }

    pid1 = fork();

    if (pid1 < 0) 
    {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid1 == 0) 
    {
        char *shm, *s;
        char *msg = "Hello from P1";
        
        shm = shmat(shmid, NULL, 0);
        if (shm == (char *) -1) 
        {
            perror("shmat");
            exit(1);
        }

        s = shm;
        strcpy(s, msg);
        printf("P1 wrote: %s\n", msg);

        shmdt(shm);

        exit(0); 
    }

    pid2 = fork();

    if (pid2 < 0) 
    {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid2 == 0) 
    {
        char *shm, *s;
        
        shm = shmat(shmid, NULL, 0);
        if (shm == (char *) -1) 
        {
            perror("shmat");
            exit(1);
        }

        s = shm;
        printf("P2 read: %s\n", s);

        shmdt(shm);

        exit(0); 
    }

    pid3 = fork();

    if (pid3 < 0) 
    {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid3 == 0) 
    {
        char *shm, *s;
        
        shm = shmat(shmid, NULL, 0);
        if (shm == (char *) -1)
        {
            perror("shmat");
            exit(1);
        }

        s = shm;
        printf("P3 read: %s\n", s);

        shmdt(shm);

        exit(0);
    }

    pid4 = fork();

    if (pid4 < 0) 
    {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid4 == 0) 
    {
        char *shm, *s;
        
        shm = shmat(shmid, NULL, 0);
        if (shm == (char *) -1) 
        {
            perror("shmat");
            exit(1);
        }

        s = shm;
        printf("P4 read: %s\n", s);

        shmdt(shm);

        exit(0);
    }

    pid5 = fork();

    if (pid5 < 0) 
    {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid5 == 0) 
    {
        char *shm, *s;
        
        shm = shmat(shmid, NULL, 0);
        if (shm == (char *) -1) 
        {
            perror("shmat");
            exit(1);
        }

        s = shm;
        printf("P5 read: %s\n", s);

        shmdt(shm);

        exit(0);
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);

    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}