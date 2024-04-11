#include <stddef.h> /* size_t */
#include <assert.h> /* assert */

#include "fsa.h"

#define MIN_MEM_SIZE 16
#define WORD_SIZE sizeof(size_t)

struct fsa
{
	void *first_free_block;
};

typedef struct block_header
{
	void *next_free_block;
} block_header_ty;

size_t FSAllocSuggestSize(size_t num_of_blocks, size_t block_size)
{
	assert(0 < num_of_blocks);
	assert(0 < block_size);             

	if (0 != (block_size % WORD_SIZE))
	{
		block_size = (block_size + WORD_SIZE) - (block_size % WORD_SIZE);
	}

	return (sizeof(fsa_ty *) + (num_of_blocks * block_size));	
}

fsa_ty *FSAInit(void *allocated_memory, size_t memory_size, size_t block_size)
{
	int i = 0;

	assert(NULL != allocated_memory);
	assert(MIN_MEM_SIZE <= memory_size);
	assert(0 < block_size);

	if (0 != (block_size % WORD_SIZE))
	{
		block_size = (block_size + WORD_SIZE) - (block_size % WORD_SIZE);
	}

	((fsa_ty *)allocated_memory)->first_free_block = 
	                       (void *)((char *)allocated_memory + sizeof(fsa_ty *));

	for (i = sizeof(fsa_ty *); (i + block_size) <= (memory_size - block_size)
		                                                       ; i += block_size)
	{
		((block_header_ty *)((char *)allocated_memory + i))->next_free_block = 
		                     (void *)((char *)allocated_memory + i + block_size);
	}

    ((block_header_ty *)((char *)allocated_memory + i))->next_free_block = NULL;
    
	return allocated_memory; 
}

void *FSAAlloc(fsa_ty *fsa)
{

	void *alloc_block = NULL;

	assert(NULL != fsa);

	alloc_block = fsa->first_free_block;

/*	if (NULL == alloc_block)
	{
		return NULL;
	}*/

	fsa->first_free_block = 
	               ((block_header_ty *)(fsa->first_free_block))->next_free_block;
/*	((block_header_ty *)alloc_block)->next_free_block = NULL;*/
	return alloc_block;
}

void FSAFree(fsa_ty *fsa, void *block)
{
	assert(NULL != fsa);
	assert(NULL != block);

	((block_header_ty *)block)->next_free_block = fsa->first_free_block;
	fsa->first_free_block = block;
}

size_t FSACountFree(const fsa_ty *fsa)
{
	size_t free_blocks_count = 0;
	void *free_blocks_runner = NULL;

	assert(NULL != fsa);

	free_blocks_runner = fsa->first_free_block;

	while (NULL != free_blocks_runner)
	{
		++free_blocks_count;

		free_blocks_runner = 
		                ((block_header_ty *)free_blocks_runner)->next_free_block;
	}

	return free_blocks_count;
}