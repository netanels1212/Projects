#include <stddef.h> /* size_t */
#include <time.h> /* time_t, time */
#include <assert.h> /* assert */
#include <stdio.h> /* fprintf */
#include <stdlib.h> /* malloc, free */
#include <unistd.h> /* sleep */

#include "scheduler.h"
#include "priority.h"
#include "task.h"

#define INVALID_TIME -1
#define IS_MATCH 0

enum 
{
	IS_NOT_STOP = 0,
	IS_STOP = 1
};

enum
{
	IS_NO_TASK_IN_CURR_TASK = 0,
	IS_TASK_IN_CURR_TASK = 1
};

struct scheduler
{
	priority_ty *p_queue;
	task_ty *curr_task;
	int to_stop;
};

sched_ty *SchedCreate(void)
{
	/* allocate memory for the scheduler */
	sched_ty *scheduler = (sched_ty *) malloc(sizeof(sched_ty));
	
	/* check if the allocation for the scheduler success */
	if (NULL == scheduler)
	{
		fprintf(stderr, "\nThe memory allocation for the scheduler failed.\n");
		
		return NULL;
	}
	
	/* allocate the priority queue inside the scheduler */
	scheduler->p_queue = PriorityCreate(TaskIsBefore);
	
	/* check that if allocation for the priority queue success */
	if (NULL == scheduler->p_queue)
	{
		fprintf(stderr, "\nThe memory allocation for the scheduler priority\
 queue failed.\n");
		
		/* if the allocation for the priority queue failed, the scheduler can't
		   work, and we free the memory we allocate for him */
		free(scheduler);
		scheduler = NULL;
		
		return NULL;
	}
	
	/*if all memory allocation success - we initial the feilds in the scheduler*/
	scheduler->curr_task = NULL;
	scheduler->to_stop = IS_STOP;
	
	return scheduler;
}

void SchedDestroy(sched_ty *sched)
{
	/* input check */
	assert(NULL != sched);
	assert(NULL != sched->p_queue);
	
	/* free the priority queue memory that store pointers to all the tasks,
	   and free the memory that store the tasks data */
	SchedClear(sched);
	
	/* free the remaining memory of the priority queue */
	PriorityDestroy(sched->p_queue);
	
	/* free the memory for the scheduler, and assign to "sched" pointer NULL 
	   to mark not to use it anymore */
	free(sched);
	sched = NULL;
}

uid_ty SchedAddTask(sched_ty *sched, sched_op_func_ty op_func, 
	void *op_params, time_t interval_in_sec)
{
	task_ty *new_task = NULL;
	uid_ty bad_uid;
	priority_queue_status_ty priority_enqueue_status = PRIORITY_NO_ERROR;
	
	/* input check */
	assert(NULL != sched);
	assert(NULL != sched->p_queue);
	assert(NULL != op_func);
	assert(0 < interval_in_sec);
	
	/* create new task, with properties like the parameters
	   we send to the function */ 
	new_task = TaskCreate(interval_in_sec, op_func, op_params);
	/* get the generic bad_uid, to return it if the function failed */
	bad_uid = UIDGetBadUID();
	
	/* add the new task to the priority queue, and save the enqueue status */ 
	priority_enqueue_status = PriorityEnqueue(sched->p_queue, new_task);
	
	/* check if the enqueue success */
	if (PRIORITY_NO_ERROR != priority_enqueue_status)
	{
		fprintf(stderr, "\nThere was an error in adding the task.\n");
		
		/* if the enqueue failed, we didn't need to save the data for those task
		   in the memory, and we free the memory of it */ 
		TaskDestroy(new_task);
		new_task = NULL;
		
		/* because no real task was added to the scheduler, we return bad_uid */
		return bad_uid;
	}
	
	/* if all success - we return the id of the new task we add */
	return TaskGetUID(new_task);
}										 

sched_status_ty SchedRemoveTask(sched_ty *sched, uid_ty uid)
{
	task_ty *task_to_remove = NULL;
	
	/* input check */
	assert(NULL != sched);
	assert(NULL != sched->p_queue);

	/* if the task we want to remove is running new in the scheduler
	   and didn't in the priority queue, we destroy it */
	if (NULL != sched->curr_task &&
	                             IS_MATCH == TaskIsMatch(sched->curr_task, &uid))
	{
		TaskDestroy(sched->curr_task);

		/* if the current task that run in the scheduler called those function
		   to remove herself, we return NO_REPEAT to indicate to the scheduler
		   that is no needed to reapet those operation, and to not enqueue again
		   those task to the priority queue */
		return (sched_status_ty)NO_REPEAT; 
	}
	
	/* we free the memory in the priority queue 
	   that store the pointer to those task */
	task_to_remove = (task_ty *) PriorityErase(sched->p_queue, TaskIsMatch,
	                                                              (void *) &uid);
	
	/* check if the erase success */ 
	if (NULL == task_to_remove)
	{
		fprintf(stderr, "\nThere was an error in removing the task.\n");
		
		return SCHED_ERROR;
	}
	
	/* if the erase success we also free the memory for the task itself */
	TaskDestroy(task_to_remove);
	task_to_remove = NULL;
	
	return SCHED_NO_ERROR;
}

void SchedRun(sched_ty *sched)
{
	task_repeat_status_ty is_repeat = NO_REPEAT;
	priority_queue_status_ty priority_queue_enqueue_status = PRIORITY_NO_ERROR;
	time_t curr_time = 0;
	time_t time_to_sleep = 0;

	/* input check */
	assert(NULL != sched);
	assert(NULL != sched->p_queue);
	
	/* we start to run the scheduler */
	sched->to_stop = IS_NOT_STOP;
	
	/* while the scheduler need to run, and also there is tasks for running: */
	while (!PriorityIsEmpty(sched->p_queue) && IS_NOT_STOP == sched->to_stop)
	{
		/* we dequeue from the priority queue the highest priority task to run,
		   to the scheduler curr_task field - represent the current task that run
		   in the scheduler */
		sched->curr_task = PriorityDequeue(sched->p_queue);
		/* we get the current time */
		curr_time = time(NULL);
		
		/* check if we success to get the current time */
		if (INVALID_TIME == curr_time)
		{
			fprintf(stderr, "\nThere was an error in get the current time.\n");
		}
		
		/* we calculate the time we need to sleep until the current task need to
		   start to run */
		time_to_sleep = TaskGetTimeToRun(sched->curr_task) - curr_time;

		/* if the sleep function stop in the middle because a interrupet,
		   she return the remaining time she was needed to sleep (if she success
		   she return 0).
		   so if the task need to run now, or if the sleep function end to sleep
		   all the time she was needed - we stop to sleep, but until those point
		   we sleep! */
		while (0 < time_to_sleep)
		{
			time_to_sleep = sleep(time_to_sleep);
		}
		
		/* we run the task, and check if those task need to repeat */
		is_repeat = TaskRun(sched->curr_task);
		
		/* if we didn't need to repeat those task, we free her memory, and adjust
		   the curr_task to NULL because there is no task that run now */
		if (NO_REPEAT == is_repeat)
		{
			if(NULL != sched->curr_task)                                              /* my TODO: need those if? */
			{
				TaskDestroy(sched->curr_task);
				sched->curr_task = NULL;
			}
		}

		/* if we need to repeat those task: */
		else
		{
			/* we calculate the next time we need to run those task */
			TaskUpdateTimeToRun(sched->curr_task);
			/* and add her to the priority queue for running again in her time */
			priority_queue_enqueue_status = PriorityEnqueue(sched->p_queue, 
			sched->curr_task);
		}
		
		/* check if the enqueue success, if failed - we free to memory for those
		   task because she won't run anymore */
		if (PRIORITY_NO_ERROR != priority_queue_enqueue_status)                      /* my TODO: do inside the "else"? */
		{
			fprintf(stderr, "\nThere was an error in add the task \
				for next interval.\n");
			
			TaskDestroy(sched->curr_task);
			sched->curr_task = NULL;
		}                                                     	
	}
	
	/* if the scheduler need to stop to run, or there is no more tasks for run -
	   we adjust the curr_task to NULL, because there is no task that run now */
	sched->curr_task = NULL;
}

int SchedStop(sched_ty *sched)
{
	/* input check */
	assert(NULL != sched);
	assert(NULL != sched->p_queue);
	
	/* adjust the scheduler field that indicait if the scheduler run or not, to
	   represent that he need to stop to run */
	sched->to_stop = IS_STOP;

	/* we send NO_REPEAT because there is a case that the current task
	   in the scheduler call this function, and we didn't need to repeat it */
	return NO_REPEAT;
}

size_t SchedSize(const sched_ty *sched)
{
	size_t is_task_in_the_scheduler = IS_NO_TASK_IN_CURR_TASK;

	/* input check */
	assert(NULL != sched);
	assert(NULL != sched->p_queue);
	
	/* check if there is a task that run in the scheduler and didn't in the 
	   priority queue (because we do dequeue).
	   if there is task - return 1, otherwise - return 0 
	   (this will be happen if the current task call this function)*/
	is_task_in_the_scheduler = NULL != sched->curr_task ? 
		                          IS_TASK_IN_CURR_TASK : IS_NO_TASK_IN_CURR_TASK;

	/* return the priority queue size (number of tasks in it) + 1 or 0 if there
	   is one more task in the scheduler */	                          
	return (PrioritySize(sched->p_queue) + is_task_in_the_scheduler);
}

int SchedIsEmpty(const sched_ty *sched)
{
	/* input check */
	assert(NULL != sched);
	assert(NULL != sched->p_queue);
	
	/* return true only if there is no tasks in the priority queue and also if
	   there is no task that run now in the scheduler
	   (this will be happen if the current task call this function) */
	return (PriorityIsEmpty(sched->p_queue) && (NULL == sched->curr_task));
}

/*
	-- SchedClear --

	1. declare task_ty pointer ("task_to_destroy") initialize to NULL.

	2. input check (assert).
	
	3. while the priority queue not empty, do:	
		3.1. dequeue from the priority queue to "task_to_destroy".
		3.2. destroy "task_to_destroy".
	
	4. in "sched" assign:
	   curr_task = NULL
	   to_stop = TRUE
*/

void SchedClear(sched_ty *sched)
{
	task_ty *task_to_destroy = NULL;

	/* input check */
	assert(NULL != sched);
	assert(NULL != sched->p_queue);

	/* if there is task that run now in the scheduler and didn't in the priority
	   queue - we free her memory */
	if(NULL != sched->curr_task)
	{
		TaskDestroy(sched->curr_task);
		sched->curr_task = NULL;
	}

	/* until there is no task in the priority queue - we dequeue task and free 
	   her memory */
	while (!PriorityIsEmpty(sched->p_queue))
	{
		task_to_destroy = PriorityDequeue(sched->p_queue);
		TaskDestroy(task_to_destroy);
	}
	
	/* now the scheduler is in the position of his creation,
	   so we initial his feilds */
	sched->curr_task = NULL;
	sched->to_stop = IS_STOP;
}	
						    