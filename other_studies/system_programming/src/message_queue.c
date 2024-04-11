#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_SIZE 1024

struct msg_buffer 
{
    long msg_type;
    char msg_text[MAX_SIZE];
};

int main() 
{
    /* Generate a unique key using ftok*/
    key_t key = ftok("progfile", 65);
    int msgid;
    pid_t pid;
    int i;

    /* Create a message queue*/
    msgid = msgget(key, 0666 | IPC_CREAT);

    /*Forking to create P1*/ 
    pid = fork();

    if (pid < 0) 
    {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid == 0) 
    { 
        /*Child process (P1)*/ 
        struct msg_buffer message;

        /*Write several messages to the message queue*/ 
        for (i = 0; i < 5; ++i) 
        {
            sprintf(message.msg_text, "Message %d from P1", i);
            message.msg_type = 1;
            msgsnd(msgid, &message, sizeof(message), 0);
            printf("P1 sent: %s\n", message.msg_text);
        }

        exit(0); /*P1 exits after writing messages*/ 
    } 
    else 
    {
        for (i = 0; i < 3; ++i) 
        {
            pid_t child_pid = fork();

            if (child_pid < 0) 
            {
                perror("Fork failed");
                exit(1);
            } 
            else if (child_pid == 0) 
            { 
                struct msg_buffer message;

                /*Read messages from the message queue after P1 dies*/ 
                while (msgrcv(msgid, &message, sizeof(message), 1, IPC_NOWAIT) != -1) 
                {
                    printf("P%d received: %s\n", i + 2, message.msg_text);
                }

                exit(0); /*Child exits after reading messages*/ 
            }
        }
    }

    /*Wait for all child processes to finish*/ 
    for (i = 0; i < 4; ++i) 
    {
        wait(NULL);
    }

    /*Delete the message queue*/ 
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}