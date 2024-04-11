/********************************************************************************
Project: Watchdog
Name: Netanel shmuel
Reviewer: Shahar 
Date: 14.1.2024
Version: 1.0
********************************************************************************/

#ifndef __KEEPWATCHING_H__
#define __KEEPWATCHING_H__

#include <stddef.h> /* size_t */
#include <signal.h>

#include "scheduler.h"

#define MAX_STR_LEN 30

enum
{
	NO_ERROR = 0,
	ERROR
};

enum
{
	UNMASK = 0,
	MASK = 1
};

/* declare global semaphore */
sem_t *g_is_wd_exist_sem;

/* define dlobal flag - g_to_stop */
sig_atomic_t g_to_stop = 0;

/* define a global counter */
sig_atomic_t g_curr_program_counter = 0;

/* define a global scheduler */
sched_ty *g_curr_program_sched = NULL;

/* declare global information about the other process */
char **g_other_program_info = NULL;

int CreateSem(void);
int SignalMaskOrUnmask(int is_mask_);
void FreeOtherProgramInfo(char **g_other);
void *KeepWatchingWraper(void *args_);


#endif /* __KEEPWATCHING_H__ */