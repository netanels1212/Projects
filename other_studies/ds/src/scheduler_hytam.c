#include <stdio.h> /*fprintf*/
#include <stdlib.h> /*malloc, free*/
#include <stddef.h> /*size_t*/
#include <unistd.h> /*sleep*/
#include <time.h> /*time_t*/
#include <assert.h> /*assert*/

#include "priority.h"
#include "task.h"
#include "scheduler.h"

#define CONTINUE 0

struct scheduler 
{
	priority_ty *p_queue;
	task_ty *curr_task;
	int to_stop;
};

static void SleepRoutine(sched_ty *scheduler);

/*
 *****************************PSEUDOCODE******************************
sched_ty *SchedCreate(sched_cmp_func_ty compare_func)
{
	0. Validate Input
	1. CreateSched.
	2. CreatePQueue.
	3. Return pointer to sched.
}

void SchedDestroy(sched_ty *schedler)
{
	0. Validate Input
	1. Destroy PQueue.
	2. Destroy Tasks.
	3. Destroy sched.
}

uid_ty SchedAddTask(sched_ty *sched, sched_op_func_ty op_func, 
										void *op_params, time_t interval_in_sec)
{
	0. Validate Input
	1. CreateTask
	2. GetUID
	3. if uid == bad_uid : Return ERROR(1)
	4. PEnqueue(task)
		4.1 If failed return bad_uid
	5. Return uid
}

sched_status_ty SchedRemoveTask(sched_ty *sched, uid_ty uid)
{
	0. Validate Input
	1. PQErase(uid)
	2. Destroy(task)
	3. return status
}

sched_status_ty SchedRun(sched_ty *sched)
{
	0. Validate Input
	1. Init to_stop = 0
	2. Loop while queue not empty and not to stop
	3. Dequeue task from the priority queue
	4. Update current task
	5. Get time to run from the task
	6. Sleep until that time
	7. Run the task
        7.1 If need to repeat:
	        7.2 Update time of the task
	        7.3 Enqueue the task into the priority queue
        7.4 Else:
	        7.5 Update current task into null
	        7.6 Destroy the task
	        7.7 Alert the user of failure
    8. Update current task to null
}

void SchedStop(sched_ty *sched)
{
	0. Validate Input
	1. Update flag to_stop=1
}

size_t SchedSize(sched_ty *sched)
{
	0. Validate Input
	1. Return size of PQueue
}

int SchedIsEmpty(sched_ty *sched)
{
	0. Validate Input
	1. Return if PQueue is empty
}

void SchedClear(sched_ty *sched)
{
	0. Validate Input
	1. Clear PQueue
	2. Destory Tasks
}*/

sched_ty *SchedCreate(void)
{
	sched_ty* scheduler = NULL;

	scheduler = (sched_ty*)malloc(sizeof(sched_ty));

	if(NULL == scheduler)
	{
		fprintf(stderr, "ERROR ALLOCATING MEMORY\n");
		return NULL;
	}

	scheduler->p_queue = PriorityCreate((sched_cmp_func_ty)TaskIsBefore);

	if(NULL == scheduler->p_queue)
	{
		free(scheduler);
		fprintf(stderr, "ERROR ALLOCATING MEMORY\n");
		return NULL;
	}

	scheduler->curr_task = NULL;
	scheduler->to_stop = 0;

	return scheduler;
}

void SchedDestroy(sched_ty *scheduler)
{
	assert(NULL != scheduler);

	SchedClear(scheduler);

	PriorityDestroy(scheduler->p_queue);
	scheduler->p_queue = NULL;

	free(scheduler);
	scheduler = NULL;
}

uid_ty SchedAddTask(sched_ty *scheduler, sched_op_func_ty op_func, 
										void *op_params,time_t interval_in_sec)
{
	task_ty *new_task = NULL;
	assert(NULL != scheduler);
	assert(0 < interval_in_sec);

	new_task = TaskCreate(interval_in_sec, op_func, op_params);

	if(NULL == new_task)
	{
		fprintf(stderr, "ERROR CREATING TASK\n");
		return UIDGetBadUID();
	}

	if(UIDIsSame(UIDGetBadUID(),TaskGetUID(new_task)))
	{
		free(new_task);
		fprintf(stderr, "ERROR CREATING UID\n");
		return UIDGetBadUID();
	}

	if(SCHED_ERROR == (sched_status_ty)PriorityEnqueue(scheduler->p_queue,
																	new_task))
	{
		return UIDGetBadUID();
	}

	return TaskGetUID(new_task);
}

sched_status_ty SchedRemoveTask(sched_ty *scheduler, uid_ty uid)
{
	task_ty *task = NULL;
	sched_cmp_func_ty cmp_func = TaskIsMatch;

	assert(NULL != scheduler);

	if(NULL != scheduler->curr_task && 
								0 == TaskIsMatch(scheduler->curr_task,&uid))
	{
		TaskDestroy(scheduler->curr_task);
		scheduler->curr_task = NULL;
		return (sched_status_ty)NO_REPEAT;
	}

	task = PriorityErase(scheduler->p_queue,cmp_func,&uid);
	TaskDestroy(task);
	task = NULL;

	return SCHED_NO_ERROR;
}

void SchedRun(sched_ty *scheduler)
{
	task_repeat_status_ty repeat_status;
	assert(NULL != scheduler);

	scheduler->to_stop = CONTINUE;

	while(CONTINUE == scheduler->to_stop && !SchedIsEmpty(scheduler))
	{
		scheduler->curr_task = PriorityDequeue(scheduler->p_queue);

		SleepRoutine(scheduler);

		repeat_status = TaskRun(scheduler->curr_task);

		switch(repeat_status)
		{
			case REPEAT:
				TaskUpdateTimeToRun(scheduler->curr_task);
				PriorityEnqueue(scheduler->p_queue,scheduler->curr_task);
				scheduler->curr_task = NULL;
			break;

			case NO_REPEAT:
				if(NULL != scheduler->curr_task)
				{
					TaskDestroy(scheduler->curr_task);
					scheduler->curr_task = NULL;
				}
			break;

			default:
				fprintf(stderr, "ERROR WHILE PERFORMING TASK\n");

				TaskDestroy(scheduler->curr_task);
				scheduler->curr_task = NULL;
			break;
		}
	}
}

int SchedStop(sched_ty *scheduler)
{
	printf("Stopping\n");
	assert(NULL != scheduler);
	scheduler->to_stop = 1;
	return 0;
}

size_t SchedSize(const sched_ty *scheduler)
{
	size_t size = 0;
	assert(NULL != scheduler);
	size = PrioritySize(scheduler->p_queue) + (scheduler->curr_task != NULL? 1 : 0);
	return size;
}

int SchedIsEmpty(const sched_ty *scheduler)
{
	assert(NULL != scheduler);

	return PriorityIsEmpty(scheduler->p_queue) && (scheduler->curr_task == NULL? 1 : 0);
}

void SchedClear(sched_ty *scheduler)
{
	task_ty *curr_task = NULL;
	assert(NULL != scheduler);

	if(NULL != scheduler->curr_task)
	{
		TaskDestroy(scheduler->curr_task);
		scheduler->curr_task = NULL;
	}

	while(!SchedIsEmpty(scheduler))
	{
		curr_task = PriorityDequeue(scheduler->p_queue);
		TaskDestroy(curr_task);
		curr_task = NULL;
	}
}

void SleepRoutine(sched_ty *scheduler)
{
	long time_left_to_sleep = 0;

	assert(NULL != scheduler);

	time_left_to_sleep = TaskGetTimeToRun(scheduler->curr_task) - time(NULL);

	if(time_left_to_sleep < 0)
	{
		TaskUpdateTimeToRun(scheduler->curr_task);
		time_left_to_sleep = TaskGetTimeToRun(scheduler->curr_task) - time(NULL);
	}
	
	while(time_left_to_sleep)
	{
		time_left_to_sleep = sleep(time_left_to_sleep);
	}
}