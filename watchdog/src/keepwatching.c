#define _POSIX_C_SOURCE  200809L

#include <fcntl.h> /* O_CREAT, O_EXCL */
#include <sys/types.h> /* pid_t, fork */
#include <unistd.h> /* fork, execvp */
#include <sys/wait.h> /* waitpid */
#include <sys/types.h> /* waitpid */
#include <semaphore.h> /* semaphore functions */
#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdio.h> /* fprintf */
#include <stdlib.h> /* atoi, malloc */
#include <errno.h> /* errno */
#include <signal.h> /* signal operations */
#include <string.h> /* strcmp */

#include "keepwatching.h"
#include "uid.h" /* for bad_uid */
#include "scheduler.h"

void FreeOtherProgramInfo(char **params_)
{
	int params_argc = 0;
	int i = 0;

	assert(NULL != params_);

	params_argc = atoi(params_[3]);

	for (i = 0; i < params_argc + 4; ++i)
	{
		free(params_[i]);
	}

	free(params_);
}

int CreateSem(void)
{
	/* open named semaphore - g_is_ws_exist_sem */
	g_is_wd_exist_sem = sem_open("g_is_wd_exist_sem", O_CREAT | O_EXCL, 0644, 0);

	/* open failure handler */
	if(SEM_FAILED == g_is_wd_exist_sem)
    {
        if(EEXIST == errno)
        {
            g_is_wd_exist_sem = sem_open("g_is_wd_exist_sem", 0);

            if(SEM_FAILED == g_is_wd_exist_sem)
            {
                return ERROR;
            }
        }

        else
        {
            fprintf(stderr, "can't create new semaphore\n");

            return ERROR;
        }
    }

    return NO_ERROR;
}

int SignalMaskOrUnmask(int is_mask_)
{
	sigset_t signal_mask;

	if (-1 == sigaddset(&signal_mask, SIGUSR1))
    {
        fprintf(stderr, "\nThere was an error in add SIGUSR1 to the signals set.\n");

        return errno;
    }

    if (-1 == sigaddset(&signal_mask, SIGUSR2))
    {
        fprintf(stderr, "\nThere was an error in add SIGUSR2 to the signals set.\n");

        return errno;
    }

    if (MASK == is_mask_)
    {
    	if (-1 == pthread_sigmask(SIG_BLOCK, &signal_mask, NULL)) 
    	{
        	fprintf(stderr, "\nThere was an error in mask the signals set.\n");

        	return errno;
    	}
    }

    if (UNMASK == is_mask_)
    {
	    if (0 != pthread_sigmask(SIG_UNBLOCK, &signal_mask, NULL))
	    {
	        fprintf(stderr, "\nThere was an error in unmask the signals for the keepwatching thread.\n");

	        return errno;
	    }     	
    }

    return NO_ERROR;
}

int CleanUpWd(void)
{
	pid_t wd_pid = atoi(getenv("WD_PID"));
	int i = 0;

	/* stop the scheduler */
	if (1 == SchedStop(g_curr_program_sched))
	{
		return ERROR;
	}

	/* destroy the scheduler */
	SchedDestroy(g_curr_program_sched);

	/* close semaphore */
    if (-1 == sem_close(g_is_wd_exist_sem))
    {
    	fprintf(stderr, "\nThere was an error in closing the semaphore.\n")	;

    	return errno;
    }

    FreeOtherProgramInfo(g_other_program_info);

	/* kill the run_watchdog process */
    if (0 != kill(wd_pid, SIGKILL))
	{
		/* sending failure handler */
		fprintf(stderr, "\nThere is an error in sending SIGUSR2 to the watchdog.\n");

		return errno;
	}

	/* wait for the killing to end */
	if (-1 == waitpid(wd_pid, NULL, 0))
	{
		fprintf(stderr, "\nThere was an error in wait for the watchdog to terminate.\n");

		return errno;
	}

	if (-1 == unsetenv("WD_PID"))
	{
		fprintf(stderr, "\nThere was an error in delete the env WD_PID variable.\n");

		return errno;
	}

	/* return no error */
	return NO_ERROR;
}

int Revive(void)
{
	pid_t fork_pid;
	int is_client = 0;

	/* create child process - fork */
	fork_pid = fork();

	/* creation failure handler */
	if (-1 == fork_pid)
	{
		fprintf(stderr, "\nThere was an error in create the child process.\n");

		return errno;
	}

	if(0 == strcmp(g_other_program_info[4], "./run_watchdog.out"))
	{
		is_client = 0;
	}

	else
	{
		is_client = 1;
	}

	/* if the current process is the child */
	if (0 == fork_pid)
	{
		/* set current process to execute the other program */
		if (-1 == execv(g_other_program_info[3], g_other_program_info + 4*is_client))
		{
			/* exec failure handler */
			fprintf(stderr, "\nThere was an error in execute the other program.\n");

		}
	}

	sprintf(g_other_program_info[0], "%d", fork_pid);

	/* return no error */
	return NO_ERROR;
}

static int SendSigTask(void *params_)
{
	(void)params_;

	printf("%d send SIGUSR1 to %s\n", getpid(), g_other_program_info[0]);

	pid_t sent_to_pid;

	sent_to_pid = atoi(g_other_program_info[0]);

	/* send SIGUSR1 signal */
	if (0 != kill(sent_to_pid, SIGUSR1))
	{
		/* sending failure handler */
		fprintf(stderr, "\nThere is an error in sending SIGUSR1 to the %s.\n",sent_to_pid);

		return errno;
	}

	/* increase counter with atomic op */
	__sync_fetch_and_add(&g_curr_program_counter, 1);

    /* return 1 for repeating the task */
    return 1;
}

static int CheckCounterTask(void *params_)
{
	(void)params_;

	pid_t sent_to_pid;

	/* if g_to_stop flag is true */
	if (g_to_stop)
	{
		/* call CleanUpWd */
		CleanUpWd();

		/* return 0 for not repeating the task */
		return 0;
	}

	sent_to_pid = atoi(g_other_program_info[0]);


	printf("%d in CheckCounterTask %s\n", getpid(), g_other_program_info[0]);

	/* if counter reach to max_failures_num_ */
	if (atoi(g_other_program_info[2]) <= g_curr_program_counter)
	{
		/* make sure that the other process was killed (killed it in force!!) */
		if (0 != kill(sent_to_pid, SIGKILL))
		{
			/* sending failure handler */
			fprintf(stderr, "\nThere is an error in sending SIGUSR2 to the %s.\n",sent_to_pid);

			return errno;
		}

		/* wait for the killing to end */
	    if (-1 == waitpid(atoi(g_other_program_info[0]), NULL, 0))
	    {
        	fprintf(stderr, "\nThere was an error in waiting to the other process to finish.\n");

        	return errno;
    	}

    	__sync_lock_test_and_set(&g_curr_program_counter, 0);

		/* revive the other process again - fork and exec */
		Revive();
	}

	/* return 1 for repeating the task */
	return 1;
}

void Siguser1Handler(int signo_)
{
	(void)signo_;

	/* set the counter to zero with atomic op */
	__sync_lock_test_and_set(&g_curr_program_counter, 0);
}

void Siguser2Handler(int signo_)
{
	(void)signo_;

	/* call to SigUsr2Routine */
	__sync_lock_test_and_set(&g_to_stop, 1);
}

int KeepWatching(void *params_)
{
	struct sigaction sigusr1_handler;
	struct sigaction sigusr2_handler;

	/* assert */
	assert(NULL != params_);

	/* create scheduler */
	g_curr_program_sched = SchedCreate();

	/* creation failure handler */
	if (NULL == g_curr_program_sched)
	{
		return ERROR;
	}

	/* unmasked the signals for this current process */
	if (NO_ERROR != SignalMaskOrUnmask(UNMASK))
	{
		return errno;
	}

	/* set the specific signal handlers */
    sigusr1_handler.sa_handler = Siguser1Handler;
    sigusr1_handler.sa_flags = 0;

    sigusr2_handler.sa_handler = Siguser2Handler;
    sigusr2_handler.sa_flags = 0;

    if (-1 == sigaction(SIGUSR1, &sigusr1_handler, NULL)) 
    {
        fprintf(stderr, "\nThere was an error in set the SIGUSR1 handler.\n");

        return errno;
    }

    if (-1 == sigaction(SIGUSR2, &sigusr2_handler, NULL))
    {
        fprintf(stderr, "\nThere was an error in set the SIGUSR2 handler.\n");

        return errno;
    }

	/* add task to the scheduler that sends in each interval_ SIGUSR1 to the other proccess */
	if (UIDIsSame(UIDGetBadUID(), SchedAddTask(g_curr_program_sched, SendSigTask, NULL, 1)))
	{
		return ERROR;
	}

	/* add task to the scheduler to check if the counter reach to max_failures_num_ */
	if (UIDIsSame(UIDGetBadUID(), SchedAddTask(g_curr_program_sched, CheckCounterTask, NULL, atoi(g_other_program_info[1]))))
	{
		return ERROR;
	}

	printf("**************IN KEEPWATCHING PID %s ***************\n", ((char **)params_)[0]);

	/* run the scheduler */
	SchedRun(g_curr_program_sched);

	/* return no error */
	return NO_ERROR;
}

void *KeepWatchingWraper(void *params_)
{
	assert(NULL != params_);

	printf("check\n");
	KeepWatching(params_);
 
	return NULL;
}