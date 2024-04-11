/********************************************************************************
Project: Heap Queue
Name: Netanel Shmuel
Reviewer: Haytham 
Date: 12.12.2023
Version: 1.0
********************************************************************************/

#ifndef __HEAP_QUEUE_H__
#define __HEAP_QUEUE_H__


#include <stddef.h> /* size_t */

enum
{
	NO_ERROR = 0,
	ERROR
};

typedef struct heap_queue heap_ty;

/*
return value: if key1_ > key2_ - positive number
				 key1_ == key2_ - 0
				 key1_ < key2 - negative number
undefined behaviour: key1_, key2_ is NULL pointer
notes: cmp_params_ can be NULL.
*/
typedef int (*cmp_func_ty)(const void *key1_, const void *key2_, const void *cmp_params_);




/*
undefined behaviour: cmp_ NULL pointer
return value: on failure - return NULL
notes: cmp_params_ can be NULL.
	   init_capacity_ can be 0, and this number didn't block the potential growth of the heap.
	   for good performence give init_capacity_ equal to number of elements you want (if you know specific amount).
 time complexity: O(1)
 */
heap_ty *HeapCreate(cmp_func_ty cmp_, const void *cmp_params_, size_t init_capacity_);


/*
notes: heap_ can be NULL pointer
time complexity: O(1)
 */
void HeapDestroy(heap_ty *heap_);


/* 
undefined behaviour: heap_ and data_ NULL pointers
time complexity: if init_capacity_ is exceeded with this enqueue O(size) - but amortized is still O(log(size))
*/
int HeapEnqueue(heap_ty *heap_, const void *data_);


/* 
undefined behaviour: heap_ NULL pointer,
					 heap is empty.
time complexity: O(log(size))
*/
void *HeapDequeue(heap_ty *heap_);


/* 
undefined behaviour: heap_ NULL pointer,
					 heap is empty.
time complexity: O(1) 
*/
void *HeapPeek(heap_ty *heap_);


/*
undefined behaviour: heap_ NULL pointer
time complexity: O(1) 
*/
size_t HeapSize(const heap_ty *heap_);


/*
undefined behaviour: heap_ NULL pointer
time complexity: O(1) 
*/
int HeapIsEmpty(const heap_ty *heap_);


/*
undefined behaviour: heap_, is_match NULL pointers
return value: pointer to first element match(order of search is unspecified), NULL if not match
notes: is_match_params_ can be NULL
time complexity: O(size)
*/
void *HeapRemove(heap_ty *heap_, int (*is_match)(const void *data_, const void *params_), const void *is_match_params_);


#endif /* __HEAP_QUEUE_H__ */