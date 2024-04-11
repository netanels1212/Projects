/*******************************************************************************
Project: Task
Name: Netanel Shmuel
Reviewer: Shahar 
Date: 14/11/2023
Version: 1.0
*******************************************************************************/
#ifndef __TASK_H__
#define __TASK_H__

#include <time.h> /* time_t */

#include "uid.h"

typedef enum
{
	TASK_NO_ERROR = 0,
	TASK_ERROR = 1
} task_status_ty;

typedef enum 
{
	RUN_ERROR = -1,
	NO_REPEAT = 0,
	REPEAT = 1
} task_repeat_status_ty;

typedef int(*task_op_func_ty)(void *op_params);
typedef struct task task_ty;

/*
Function Description: Create a new task

Return value: pointer to the new task

Undefined behaviour: Interval_in_sec <= 0, op_func NULL

Time Complexity: O(1)

on failure: NULL.
*/
task_ty *TaskCreate(time_t interval_in_sec, task_op_func_ty op_func,
                                                               void *op_params);

/*
Function Description: Destroy a new task

Return value: void

Undefined behaviour: task is NULL

Time Complexity: O(1)

on failure: None.
*/
void TaskDestroy(task_ty *task);

/*
Function Description: Update the time_to_run

Return value: status, 0 for no error

Undefined behaviour: task is NULL

Time Complexity: O(1)

on failure: 1 for error.
*/
task_status_ty TaskUpdateTimeToRun(task_ty *task);

/*
Function Description: Return timetorun

Return value: time_to_run

Undefined behaviour: task is NULL

Time Complexity: O(1)

on failure: None.
*/
time_t TaskGetTimeToRun(const task_ty *task);

/*
Function Description: Return task id

Return value: task id

Undefined behaviour: task is NULL

Time Complexity: O(1)

on failure: BAD_UID.
*/
uid_ty TaskGetUID(const task_ty *task);

/*
Function Description: Return true or false

Return value: True(1) or False(0)

Undefined behaviour: task is NULL

Time Complexity: O(1)

on failure: None.
*/
int TaskIsMatch(const void *task, const void *task_id);

/*
Function Description: Executes task

Return value: Status of whether to repeat or not the task (add back to queue) 
			 (0) for not and (1) otherwise.

Undefined behaviour: task is NULL

Time Complexity: O(1)

on failure: None.
*/
task_repeat_status_ty TaskRun(task_ty *task);

/*
Function Description: Compares task priority by time

Return value: Returns True (1) if first_task should happen before second_task
			  and False(0) otherwise.

Undefined behaviour: first_task and or second_task is NULL

Time Complexity: O(1)

on failure: None.
*/
int TaskIsBefore(const void *first_task, const void *second_task);

#endif /*__TASK_H__*/
