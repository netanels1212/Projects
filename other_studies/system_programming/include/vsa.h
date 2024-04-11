/*******************************************************************************
Project: Variable size alloc
Name: Netanel Shmuel
Reviewer: Asem 
Date: 19.11.23
Version: 1.0
*******************************************************************************/
#ifndef __VARIABLE_SIZE_ALLOC_H__
#define __VARIABLE_SIZE_ALLOC_H__

#include <stddef.h> /* size_t */

typedef struct vsa vsa_ty;
typedef struct chunk_header variable_chunk_ty;


/*
Function Description: creates a variable size allocator (vsa).

Return value: Vsa struct that points to the first free chunk

Undefined behaviour: Memory size below 16,
 					 "allocated_memory" equalling none.

Note: The memory size should be big enough for a pointer and at least 
	  one aligned block.

Time Complexity: O(1)

on failure: None.
*/
vsa_ty *VSAInit(void *allocated_memory, size_t memory_size);


/*
Function Description: Rearanges the list to no longer point at the first
		              free chunk and finding the new first free block,
		              returns NULL if there is none.

Return value: address of the chunk to use

Undefined behaviour: vsa is NULL 

Time Complexity: O(n)

on failure: NULL if there are no free blocks.
*/
void *VSAAlloc(vsa_ty *vsa, size_t chunk_size);


/*
Function Description: Rearanges the list to include the sent block as free.

Return value: None

Undefined behaviour: NULL "vsa" or block, or block not part of list.                 my TODO: block? who is he? 

Time Complexity: O(1)

on failure: None
*/
void VSAFree(void *chunk);


/*
Function Description: Finds the largest chunk available.

Return value: amount of free tests.                           my TODO: ????????

Undefined behaviour: vsa is NULL.

Time Complexity: O(n)

on failure: None
*/
long VSALargestChunkAvailable(const vsa_ty *vsa);        

#endif /*__VARIABLE_SIZE_ALLOC_H__*/
