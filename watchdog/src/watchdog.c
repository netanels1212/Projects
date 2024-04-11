#define _POSIX_C_SOURCE 200809L

#include <fcntl.h> /* O_CREAT, O_EXCL */
#include <sys/stat.h> /* For mode constants */
#include <pthread.h> /* threads functions */
#include <string.h> /* strcpy, strlen */
#include <time.h> /* timespec */
/*#include <sys/time.h> gettimeofday */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "keepwatching.h"
#include "watchdog.h"
#include "scheduler.h"

enum
{
	CLIENT = 0,
	WD = 1
};

pthread_t g_watching_thread_id;
char **g_client_program_info = NULL;

int GenerateParams(int which_info_, int argc_, char **argv_, pid_t pid, size_t interval_, size_t max_failures_num_)
{
	char **dest = NULL;
	int i = 0;

	assert(NULL != argv_);

	/* for also the pid, interval, max_failures_num, argc, and NULL in the end */
	dest = (char **)malloc((argc_ + 5) * sizeof(char *));

    if (NULL == dest)
    {
        fprintf(stderr, "\nFailed to allocate memory for params array of strings.\n");
        
        return ERROR;
    }

    for (i = 0; i < 4; ++i)
    {
    	dest[i] = (char *)malloc(MAX_STR_LEN * sizeof(char));

    	if (NULL == dest[i])
    	{
        	fprintf(stderr, "\nFailed to allocate memory for argumant in the array of strings.\n");
        
        	return ERROR;
    	}
    }
    sprintf(dest[0], "%d", pid);
    sprintf(dest[1], "%lu", interval_);
    sprintf(dest[2], "%lu", max_failures_num_);
    sprintf(dest[3], "%d", argc_);

    for (i = 0; i < argc_; ++i)
    {
        dest[i + 4] = (char *)malloc((1 + strlen(argv_[i])) * sizeof(char));

        if (NULL == dest[i + 4])
    	{
        	fprintf(stderr, "\nFailed to allocate memory for argument in the array of strings.\n");
        
        	return ERROR;
    	}

    	strcpy(dest[i + 4], argv_[i]);
    }

    dest[i + 4] = NULL;

    if (CLIENT == which_info_)
    {
    	g_client_program_info = dest;
    }

    if (WD == which_info_)
    {
    	g_other_program_info = dest;
    }

    return NO_ERROR;
}

int SemWaitUntil(void)
{
	struct timespec sem_wait_time;

	clock_gettime(CLOCK_REALTIME, &sem_wait_time);

    sem_wait_time.tv_sec += 10;
    sem_wait_time.tv_nsec = 0;

	/* sem wait specific time */
	if(0 != sem_timedwait(g_is_wd_exist_sem, &sem_wait_time))
	{
		fprintf(stderr, "\nThere was an error in waiting to the semaphore.\n");

		return errno;
	}

	return NO_ERROR;
}

int MakeMeImmortal(int argc, char *argv[], size_t interval_, size_t max_failures_num_)
{


	pid_t fork_pid = 0; /* BAD_PID */
	char *run_watchdog_file_as_argv[] = {"./run_watchdog.out"};
	int i = 0;

	printf("**************IN MMI***************\n");

	/* assert */
	assert(NULL != argv);

	/* open named semaphore - g_is_ws_exist_sem */
	sem_unlink("g_is_wd_exist_sem");
	if (NO_ERROR != CreateSem())
	{
		return errno;
	}

	/* mask the signals for main process */
	if (NO_ERROR != SignalMaskOrUnmask(MASK))
	{
		return errno;
	}

	/* if there is no watchdog exist (check WD_PID env variable) */
	if (NULL == getenv("WD_PID"))
	{
		/* create child process */

		/* init the params for the child */
		GenerateParams(CLIENT, argc, (char **)argv, getpid(), interval_, max_failures_num_);

		fork_pid = fork();

		/* creation failure handler */
		if (-1 == fork_pid)
		{
			fprintf(stderr, "\nThere was an error in create the child process.\n");

			return errno;
		}

		/* if the current process is the child */
		if (0 == fork_pid)
		{
			/* set current process to execute run_watchdog program */
			for(i=0;i<argc+4;++i)
			{
				printf("my args - %s\n",g_client_program_info[i]);
			}
			if (-1 == execv("./run_watchdog.out", g_client_program_info))
			{
				/* exec failure handler */
				fprintf(stderr, "\nThere was an error in execute run_watchdog.out\n");

				return errno;
			}
		}

	}
	
	else
	{
		fork_pid = atoi(getenv("WD_PID"));
	}

	/* sem wait specific time */
	if(NO_ERROR != SemWaitUntil())
	{
		return errno;
	}

	sem_close(g_is_wd_exist_sem);
	
	printf("check after sem close~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	GenerateParams(WD, 1, run_watchdog_file_as_argv, fork_pid, interval_, max_failures_num_);
	for(i=0;i<argc+4;++i)
	{
		printf("my g_other_program_info - %s\n",g_other_program_info[i]);
	}

	/* create thread to execute KeepWatching with the params of the child */
	if (0 != pthread_create(&g_watching_thread_id, NULL, KeepWatchingWraper, g_other_program_info))
	{
		/* creation failure handler */
		fprintf(stderr, "\nThere was an error in create the thread to watch on the watchdog.\n");

		FreeOtherProgramInfo(g_client_program_info);

		return errno;
	}

	FreeOtherProgramInfo(g_client_program_info);

	/* return no error */
	return NO_ERROR;
}


int StopWD(void)
{
	pid_t wd_id = atoi(getenv("WD_PID"));

	/* send SIGUSR2 signal to the watchdog */
	if (0 != kill(wd_id, SIGUSR2))
	{
		/* sending failure handler */
		fprintf(stderr, "\nThere is an error in sending SIGUSR2 to the watchdog.\n");

		return errno;
	}

	if (-1 == waitpid(atoi(getenv("WD_PID")), NULL, 0))
	{
		fprintf(stderr, "\nThere was an error in wait for the watchdog to terminate.\n");

		return errno;
	}

	/* stop the scheduler */
	if (1 == SchedStop(g_curr_program_sched))
	{
		return ERROR;
	}

	/* join for the thread from MakeMeImmortal function to end */
	if (0 != pthread_join(g_watching_thread_id, NULL))
	{
		fprintf(stderr, "\nThere was an error in join the watching thread.\n");

		return errno;
	}

	if (-1 == unsetenv("CLIENT_NAME"))
	{
		fprintf(stderr, "\nThere was an error in delete the env WD_PID variable.\n");

		return errno;
	}

	/* unmasked the signals for this current process */
	if (NO_ERROR != SignalMaskOrUnmask(UNMASK))
	{
		return errno;
	}

    FreeOtherProgramInfo(g_other_program_info);

	/* destroy the scheduler */
	SchedDestroy(g_curr_program_sched);

	/* return no error */
	return NO_ERROR;
}