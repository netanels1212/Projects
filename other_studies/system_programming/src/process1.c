#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <sys/types.h> /* pid_t */
#include <unistd.h>
#include <stdlib.h> /* kill */
#include <signal.h> /* signal */
#include <assert.h> /* assert */

/* for not declare it in each handling */
static pid_t caller = 0;

static void Process1HandlerIMP(int signo, siginfo_t *info, void *context);

int main()
{
    pid_t pid = getpid();

    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = Process1HandlerIMP;
    sigemptyset(&action.sa_mask);

    printf("process1 id: %d\n", pid);

    if (-1 == sigaction(SIGUSR1, &action, NULL)) 
    {
        perror("sigaction");

        return 1;
    }

    while(1)
    {
        printf("\nwait for signal for process2 :)\n");
        pause();

        printf("\npreparing to send a signal to process1\n");

        kill(caller, SIGUSR2);
        sleep(1);
    }
    
    return 0;
}

static void Process1HandlerIMP(int signo, siginfo_t *info, void *context)
{
	assert(NULL != info);
	(void)signo;
	(void)context;

    caller = info->si_pid;

    printf("Received SIGUSR1\n");
    sleep(1);
}