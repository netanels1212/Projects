/*******************************************************************************
Project: Fixed size alloc
Name: Netanel Shmuel
Reviewer: Amram 
Date: 16/11/23
Version: 1.0
*******************************************************************************/
#ifndef __FSA_H__
#define __FSA_H__

#include <stddef.h> /* size_t */

typedef struct fsa fsa_ty;



/*
Function Description: suggest the size of memory needed 
                      for initialization the FSA (Fixed-Size Allocator)

Return value: size of the memory needed

Undefined behaviour: num_of_blocks or block_size less or equal to zero 

Time Complexity: O(1)
*/
size_t FSAllocSuggestSize(size_t num_of_blocks, size_t block_size);


/*
Function Description: initialize the FSA

Return value: pointer to the FSA

Undefined behaviour: "allocated_memory" NULL ptr, "memory_size" less than 16,
                     "block_size less" or equal to zero               

Time Complexity: O(n)                                                                                      
*/
fsa_ty *FSAInit(void *allocated_memory, size_t memory_size, size_t block_size);


/*
Function Description: Function to the possible allocated block from the FSA

Return value: pointer to the start of the allocated block                                                                         

Undefined behaviour: "fsa" NULL ptr, if all the memory of the FSA is in use                                             

Time Complexity: O(1)

on failure: NULL
*/
void *FSAAlloc(fsa_ty *fsa);


/*
Function Description: free a block in the FSA

Return value: void

Undefined behaviour: "fsa" NULL ptr or "block" NULL ptr                                                      

Time Complexity: O(1)
*/
void FSAFree(fsa_ty *fsa, void *block);


/*
Function Description: count the number of free blocks in the FSA

Return value: number of the free blocks

Undefined behaviour: fsa NULL ptr

Time Complexity: O(n)
*/
size_t FSACountFree(const fsa_ty *fsa);

#endif /*__FSA_H__*/