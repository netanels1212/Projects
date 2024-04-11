#include <errno.h> /* errno */
#include <sys/types.h> /* pid_t */
#include <signal.h> /* signal, kill */
#include <unistd.h> /* getppid */
#include <unistd.h> /* pause */
#include <stdio.h> /* printf */

static void pong_handler(int signum)
{
	(void)signum;
    printf("Recive SIGUSR1\n");
    sleep(1);
}

void Pong(void)
{
	while (1)
	{
	    signal(SIGUSR1, pong_handler);

	    pause();
	    kill(getppid(), SIGUSR2);
	}
}

int main()
{
	Pong();

	return 0;
}
