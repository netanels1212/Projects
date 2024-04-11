/*******************************************************************************
Project: Queue
Name: Netanel Shmuel
Reviewer: Diana 
Date: 24/10/23
Version: 1.0
*******************************************************************************/
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h> /*size_t*/

enum queue_return_value_ty {QSUCCESS = 0, QFAIL = 1, QMEM_FAIL = 2};

typedef struct queue queue_ty;
typedef int status_ty;

/*
	Function: QueueCreate
	return value: pointer to the queue, if the memory allocation within failed -
	              return NULL.
	Time Complexity: O(1)
*/
queue_ty *QCreate();

/*
	Function:QueueDestroy
	return value: None (void)
	on success: Deletes queue
	undefined behaviour: null pointer
	on failure: Shouldn't fail
	Time Complexity: O(n)
*/
void QDestroy(queue_ty *queue);


/*
	Function: Enqueue
	return value: QSUCCESS (0) when success, QMEM_FAIL(2) the memory allocation
	              within failed 
	on success: Adds element to start of queue
	undefined behaviour: if "queue" or "element" are NULL pointers
	Time Complexity: O(1)
*/
status_ty QEnqueue(queue_ty *queue, const void *data);

/*
	Function:Dequeue
	return value: None (void)
	on success: Removes an element from the end of the queue
	undefined behaviour: null pointer
	Time Complexity: O(1)
*/
void QDequeue(queue_ty *queue);

/*
	Function: QueueIsEmpty
	return value: boolean value
	on success: Returns whether the queue is empty or not
	undefined behaviour: null pointer
	on failure: returns zero
	Time Complexity: O(1)
*/
status_ty QIsEmpty(const queue_ty *queue);

/*
	Function: QueuePeekFront
	return value: void *
	on success: reads element at the front of the queue
	undefined behaviour: null pointer
	on failure: returns null
	Time Complexity: O(1)
*/
void *QPeek(const queue_ty *queue);

/*
	Function: QueueSize
	return value: size_t
	on success: returns the size of the queue
	undefined behaviour: queue null
	on failure: returns zero
	Time Complexity: O(n)
*/
size_t QSize(const queue_ty *queue);


/*
	Function: QueueAppend
	return value: New concatenated list
	on success: Concatenates dest with src (dest as start), after thet src point
	            only to dmmy node
	undefined behaviour: null pointer
	Time Complexity: O(n)
*/
void *QAppend(queue_ty *dest, queue_ty *src);

#endif /*__QUEUE_H__*/
