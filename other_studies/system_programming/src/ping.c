#include <errno.h> /* errno */
#include <sys/types.h> /* pid_t */
#include <signal.h> /* signal, kill */
#include <unistd.h> /* fork, execl */
#include <unistd.h> /* pause */
#include <stdio.h> /* printf */

static void ping_handler(int signum)
{
	(void)signum;
    printf("Recive SIGUSR2\n");
}

int Ping(void)
{
	pid_t child_pid = fork();
	int child_exit_status = 0;

	if (0 > child_pid)
    {
        fprintf(stderr, "\nCreation of child process using 'fork' failed.\n");

        return errno;
    }

    else if (0 < child_pid)
    {
    	while (1)
    	{
        	signal(SIGUSR2, ping_handler);
        
        	/* for the fork finish */
	        sleep(2);

	        kill(child_pid, SIGUSR1);
	        pause();
	    }
    }

    else
    {
    	child_exit_status = execl("./pong", NULL);

    	if (-1 == child_exit_status)
    	{
    		printf("\nThere is an error in execute child program.\n");

    		return -1;
    	}
    }

    return 0;
}

int main()
{
	Ping();

	return 0;
}