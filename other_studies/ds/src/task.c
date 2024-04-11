#include <time.h>/* time_t, time */
#include <assert.h> /* assert */
#include <stdio.h> /* fprintf */
#include <stdlib.h> /* malloc, free */

#include "task.h"

#define INVALID_TIME -1

enum 
{
	IS_NOT_TASK_BEFORE = 0,
	IS_TASK_BEFORE = 1
};

struct task 
{
	uid_ty task_id;
	task_op_func_ty op_func;
	void *op_params;
	time_t interval_in_sec;
	time_t time_to_run;
};

task_ty *TaskCreate(time_t interval_in_sec, task_op_func_ty op_func, 
                                                                void *op_params)
{
	task_ty *task = NULL;
	time_t curr_time = 0;
	
	/* input check */
	assert(NULL != op_func);
	assert(0 < interval_in_sec);
	
	/* allocate memory for the task */
	task = (task_ty *) malloc(sizeof(task_ty));
	
	/* cheack if the memory allocation for the task success */
	if (NULL == task)
	{
		fprintf(stderr, "\nThe memory allocation for the task failed.\n");
		
		return NULL;
	}
	
	/* get the current time for calculating the first task run time */
	curr_time = time(NULL);
	
	/* check if we success to get the current time */
	if (INVALID_TIME == curr_time)
	{
		fprintf(stderr, "\nThere was an error in task run time.\n");
		
		/* if we failed to get the current time, we didn't now the first run time
		   of the task and we can't run her, so we free her memory */
		free(task);
		task = NULL;
		
		return NULL;
	}
	
	/* if all success we initial the task fields */
	/* generate uid for the task */
	task->task_id = UIDGenerate();
	task->op_func = op_func;
	task->op_params = op_params;
	task->interval_in_sec = interval_in_sec;
	/* calculating the first task run time */
	task->time_to_run = interval_in_sec + curr_time;
	   
	return task;
}

void TaskDestroy(task_ty *task)
{
	/* input check */
	assert(NULL != task);
	assert(NULL != task->op_func);
	
	/* free the memory for the task, and assign to "task" pointer NULL 
	   to mark not to use it anymore */
	free(task);
	task = NULL;
}

task_status_ty TaskUpdateTimeToRun(task_ty *task)
{
	time_t curr_time = 0;

	/* input check */
	assert(NULL != task);
	assert(NULL != task->op_func);
	
	/* get the current time for calculating the next task run time */
	curr_time = time(NULL);
	
	/* check if we success to get the current time */
	if (INVALID_TIME == curr_time)
	{
		fprintf(stderr, "\nThere was an error in task run time.\n");
		
		return TASK_ERROR;
	}
	
	/* calculating the next task run time */
	task->time_to_run = curr_time + task->interval_in_sec;
	
	return TASK_NO_ERROR;
}

time_t TaskGetTimeToRun(const task_ty *task)
{
	/* input check */
	assert(NULL != task);
	assert(NULL != task->op_func);
	
	/* return the run time of those task */
	return task->time_to_run;
}

uid_ty TaskGetUID(const task_ty *task)
{
	/* input check */
	assert(NULL != task);
	assert(NULL != task->op_func);
	
	/* return the uid of those task */
	return task->task_id;
}

int TaskIsMatch(const void *task, const void *task_id)
{
	/* input check */
	assert(NULL != task);
	assert(NULL != ((task_ty *)task)->op_func);
	assert(NULL != task_id);
	
	/* use the function UIDIsSame() to check if the "task_id" in "task" match to
	   "task_id" we recive as paramter, and return her return value */
	return UIDIsSame(((task_ty *)task)->task_id, *(uid_ty *)task_id);
}

task_repeat_status_ty TaskRun(task_ty *task)
{
	task_repeat_status_ty is_repeat = NO_REPEAT;

	/* input check */
	assert(NULL != task);
	assert(NULL != task->op_func);
	
	/* run task operation with her parameters, and save in is_repeat variable
	   if we need to repeat this task or not */
	is_repeat = task->op_func(task->op_params);
	
	/* return is_repeat status */
	return is_repeat;
}

int TaskIsBefore(const void *first_task, const void *second_task)
{
	/* input check */
	assert(NULL != first_task);
	assert(NULL != ((task_ty *)first_task)->op_func);
	assert(NULL != second_task);
	assert(NULL != ((task_ty *)second_task)->op_func);
	
	/* return the difference of the run time in those tasks.
	   if the first_task run time is before the second - return negitive number,
	   if they equal - return 0,
	   if the second_task run time is before the first one - return positive 
	   number */
	return ((((task_ty*)first_task)->time_to_run) - (((task_ty*)second_task)->time_to_run));
}