#include <stdlib.h> /* system */
#include <sys/types.h> /* pid_t, fork */
#include <unistd.h> /* fork, execvp */
#include <sys/wait.h> /* waitpid */
#include <errno.h> /* errno */
#include <assert.h> /* assert */
#include <string.h> /* strcmp */
#include <stdio.h> /* printf, scanf, fprintf */

#define COMMAND_MAX_SIZE 100
#define MAX_ARGS 32

enum
{
    FORK = 0,
    SYSTEM = 1
};

enum
{
    NO_ERROR = 0,
    EXIT
};

static int GetChoiceIMP(void)
{
    int choice = 0;

    printf("\nEnter how you want to execute your command:\n");
    printf("For 'fork' - press 0, for 'system' - press 1: ");
    scanf("%d", &choice);

    return choice;
}

static int IsExitIMP(char *command_)
{
    assert(NULL != command_);

    return !strcmp(command_, "exit");
}

int Shell(void)
{
    char command[COMMAND_MAX_SIZE] = {0};
    pid_t pid = 0;
    int choice = -1;
    int child_status = 0;
    char *args[MAX_ARGS];

    printf("\n *** Hello to my Simple Shell program! ***\n");

    while (1)
    {
        printf("\nEnter your command ('exit' - for exit): ");
        scanf(" %[^\n]", command);

        if (IsExitIMP(command))
        {
            printf("\ngood bye!\n");

            return EXIT;
        }

        choice = -1;

        while (0 > choice || 1 < choice)
        {
            choice = GetChoiceIMP(); 

            switch(choice)
            {
                case FORK:
                    pid = fork();

                    if (0 > pid)
                    {
                        fprintf(stderr, "\nThere was an error in execute the command using 'fork'\n");

                        return errno;
                    }

                    else if (0 < pid)
                    {
                        waitpid(pid, &child_status, 0);
                    }

                    else
                    {
                        printf("\nRunning the shell by fork\n\n");

                        args[0] = "/bin/sh";
                        args[1] = "-c";
                        args[2] = command;
                        args[3] = NULL;

                        child_status = execvp(args[0], args);
                    }

                    printf("\nChild proccess exit status: %d\n", child_status);

                    if (0 != child_status)
                    {
                        fprintf(stderr, "There was an error in execute the command using 'fork'\n");

                        return errno;
                    }

                    break;

                case SYSTEM:

                    printf("\nRunning the shell by system\n\n");
                    child_status = system(command);
                    
                    printf("\nChild proccess exit status: %d\n", child_status);

                    if (-1 == child_status)
                    {
                        fprintf(stderr, "There was an error in execute the command using 'system'\n");

                        return errno;
                    }

                    break;

                default:
                    printf("\nInvalid choice! please try again...\n");
                    break;
            }
        }
    }
}


int main()
{
    Shell();

    return 0;
}
