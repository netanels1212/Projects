#include <errno.h> /* errno */
#include <sys/types.h> /* pid_t */
#include <signal.h> /* signal, kill */
#include <unistd.h> /* getppid, fork */
#include <unistd.h> /* pause */
#include <stdio.h> /* printf */


static void ParentHandler(int signum)
{
    (void)signum;
    printf("Recive SIGUSR2\n");
}

static void ChildHandler(int signum)
{
    (void)signum;
    printf("Recive SIGUSR1\n");
}

int PingPong(void)
{
    pid_t child_pid = fork();

    if (0 > child_pid)
    {
        fprintf(stderr, "\nCreation of child process using 'fork' failed.\n");

        return errno;
    }

    else if (0 < child_pid)
    {
        signal(SIGUSR2, ParentHandler);
        
        /* for the fork finish */
        sleep(2);

        kill(child_pid, SIGUSR1);
        pause();
    }

    else
    {
        signal(SIGUSR1, ChildHandler);

        pause();
        kill(getppid(), SIGUSR2);
    }

    return 0;
}

int main()
{
    while (1)
    {
       PingPong(); 
    }

    return 0;
}