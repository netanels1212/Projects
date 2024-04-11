/*******************************************************************************
Project: Priority Queue
Name: Netanel Shmuel 
Reviewer: Haytham 
Date: 13/11/23
Version: 1.0
*******************************************************************************/
#ifndef __PRIORITY_QUEUE_H__
#define __PRIORITY_QUEUE_H__

#include <stddef.h> /*size_t*/


typedef enum 
{
    PRIORITY_NO_ERROR = 0,
    PRIORITY_MALLOC_ERROR = 1,
    PRIORITY_GENERAL_ERR = 2
} priority_queue_status_ty;

typedef struct priority_queue priority_ty;
typedef int(*cmp_func_ty)(const void *data1, const void *data2);


/*
Function Description: Create new priority queue

Return value: pointer to the new priority queue

Undefined behaviour: compare_func is NULL

Time Complexity: O(1)

on failure: Return NULL.
*/
priority_ty *PriorityCreate(cmp_func_ty compare_func);


/*
Function Description: Destroy the priority queue

Return value: None

Undefined behaviour: p_queue is NULL

Time Complexity: O(n)

on failure: None.
*/
void PriorityDestroy(priority_ty *p_queue);


/*
Function Description: Add to priority queue according to priority

Return value: priority_queue_status_ty -  PRIORITY_NO_ERROR(0)

Undefined behaviour: p_queue is NULL
                     data is NULL

Time Complexity: O(1)

on failure: priority_queue_status_ty - PRIORITY_GENERAL_ERR(2).
*/
priority_queue_status_ty PriorityEnqueue(priority_ty *p_queue, void *data);


/*
Function Description:remove from top of the priority queue

Return value: pointer to the removed data

Undefined behaviour: p_queue is NULL

Time Complexity: O(1)

on failure: None.
*/
void *PriorityDequeue(priority_ty *p_queue);


/*
Function Description:Peek the top of the priority queue

Return value: Data pointer

Undefined behaviour: p_queue is NULL

Time Complexity: O(1)

on failure: None.
*/
void *PriorityPeek(priority_ty *p_queue);


/*
Function Description: Check if the priority queue is empty

Return value: TRUE(1) or FALSE(0)

Undefined behaviour: p_queue is NULL

Time Complexity: O(1)

on failure: None.
*/
int PriorityIsEmpty(const priority_ty *p_queue);


/*
Function Description: Check the size of the priority queue

Return value: The size of the priority list

Undefined behaviour: p_queue is NULL

Time Complexity: O(n)

on failure: None.
*/
size_t PrioritySize(const priority_ty *p_queue);


/*
Function Description: empty the priority queue - deques all nodes  

Return value: None.

Undefined behaviour: p_queue is NULL

Time Complexity: O(n)

on failure: None.
*/
void PriorityClear(priority_ty *p_queue);


/*
Function Description: Erase the first element which satisfies specific 
        criteria according to the provided is_match function that match to data

Return value:priority_queue_status_ty - PRIORITY_NO_ERROR(0)

Undefined behaviour: p_queue is NULL
                     data is NULL

Time Complexity: O(n)

on failure: priority_queue_status_ty - PRIORITY_GENERAL_ERR(2).
*/
void *PriorityErase(priority_ty *p_queue, cmp_func_ty is_match, void *data);


#endif /*__PRIORITY_QUEUE_H__*/
