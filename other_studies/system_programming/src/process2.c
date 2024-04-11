#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h> /* kill */
#include <signal.h> /* sigaction */
#include <assert.h> /* assert */

static void Process2HandlerIMP(int signo, siginfo_t *info, void *context);

int main(int argc, char *argv[])
{
    struct sigaction action;
    pid_t child_id;

    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = Process2HandlerIMP;
    sigemptyset(&action.sa_mask);

    if (2 != argc)
    {
        fprintf(stderr, "missing child ID.");

        return 1;
    }

    child_id = atoi(argv[1]);

    if (-1 == sigaction(SIGUSR2, &action, NULL)) 
    {
        perror("sigaction");

        return 1;
    }

    while(1)
    {
        printf("\npreparing to send a signal to process1\n");

        sleep(1);
        kill(child_id, SIGUSR1);

        pause();

        printf("\nwait for signal from process1 :)\n");
    }

    return 0;
}

void Process2HandlerIMP(int signo, siginfo_t *info, void *context)
{
	assert(NULL != info);
	(void)signo;
	(void)context;

    printf("Received SIGUSR2\n");
    sleep(1);
}