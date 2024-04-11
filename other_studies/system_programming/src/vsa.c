#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdio.h> /* fprintf */

#include "vsa.h"

#define MIN_MEM_SIZE 24
#define SET_ALLOC -1
#define SET_FREE -1

struct vsa 
{
    size_t allocated_memory_size;
};

 struct chunk_header
{
    long chunk_size;
};

static long SetAsAlloc(long chunk_size)
{
	return (chunk_size * SET_ALLOC);
}

static long SetAsFree(long chunk_size)
{
	return (chunk_size * SET_FREE);
}

vsa_ty *VSAInit(void *allocated_memory, size_t memory_size)
{
	void *chunk_size_place = NULL;

	assert(NULL != allocated_memory);
	assert(MIN_MEM_SIZE <= memory_size);

	chunk_size_place = ((char *)allocated_memory) + sizeof(vsa_ty *);

	((vsa_ty *)allocated_memory)->allocated_memory_size = 
	                                              memory_size - sizeof(vsa_ty *);

	((variable_chunk_ty *)chunk_size_place)->chunk_size = 
	                                              memory_size - sizeof(vsa_ty *);

	return (vsa_ty *)allocated_memory;
}

long VSALargestChunkAvailable(const vsa_ty *vsa)
{
	void *chunks_runner = NULL;
	void *free_chunks_runner = NULL;
	void *memory_allocated_end = NULL;
	long largest_chunk_size = 0;        
	long curr_chunk_size = 0;
	long free_curr_chunk_size = 0;
	long merge_free_chunks_size = 0;

	assert(NULL != vsa);

	chunks_runner = ((char *)vsa) + sizeof(vsa_ty *);
	memory_allocated_end = ((char *)chunks_runner) + vsa->allocated_memory_size;

	while (chunks_runner != memory_allocated_end)
	{
		curr_chunk_size = ((variable_chunk_ty *)chunks_runner)->chunk_size;

		if (0 < curr_chunk_size)
		{
			free_chunks_runner = chunks_runner;
			free_curr_chunk_size = curr_chunk_size;

			while (0 < free_curr_chunk_size && 
				                      free_chunks_runner != memory_allocated_end)
			{
				merge_free_chunks_size += free_curr_chunk_size;

				free_chunks_runner = ((char *)free_chunks_runner) + 
					                                        free_curr_chunk_size; 

				if (free_chunks_runner != memory_allocated_end)
				{
					free_curr_chunk_size = 
						   ((variable_chunk_ty *)free_chunks_runner)->chunk_size;
				}                                                                       
			}

			curr_chunk_size = merge_free_chunks_size;
			merge_free_chunks_size = 0;
		}

		if (curr_chunk_size > largest_chunk_size)
		{
			largest_chunk_size = curr_chunk_size;
		}

		if (0 > curr_chunk_size)
		{
			curr_chunk_size = SetAsFree(curr_chunk_size);
		}

		chunks_runner = ((char *)chunks_runner) + curr_chunk_size;
	}

	return largest_chunk_size;
}

void *VSAAlloc(vsa_ty *vsa, size_t chunk_size)
{
	void *chunks_runner = NULL;
	void *free_chunks_runner = NULL;
	void *memory_allocated_end = NULL;
	void *remain_chunk = NULL;
	long chunk_size_with_meta_data = 0;
	long curr_chunk_size = 0;
	long free_curr_chunk_size = 0;
	long remain_chunk_free_space = 0;
	long merge_free_chunks_size = 0;

	assert(NULL != vsa);
	assert(0 < chunk_size);                        

	chunk_size_with_meta_data = chunk_size + sizeof(variable_chunk_ty *);

	if (VSALargestChunkAvailable(vsa) < chunk_size_with_meta_data)
	{
		fprintf(stderr, "\nThere is no free space for your requested chunk size.\
		\n");

		return NULL;
	}

	chunks_runner = ((char *)vsa) + sizeof(vsa_ty *);
	memory_allocated_end = ((char *)chunks_runner) + vsa->allocated_memory_size;  /*TODO: vsa->allocated_memory_size not all memory block*/

	while (chunks_runner != memory_allocated_end)
	{
		curr_chunk_size = ((variable_chunk_ty *)chunks_runner)->chunk_size;

		if (curr_chunk_size < chunk_size_with_meta_data && 0 < curr_chunk_size)
		{
			free_chunks_runner = chunks_runner;
			free_curr_chunk_size = curr_chunk_size;

			while (0 < free_curr_chunk_size && 
				                      free_chunks_runner != memory_allocated_end)
			{
				merge_free_chunks_size += free_curr_chunk_size;

				free_chunks_runner = ((char *)free_chunks_runner) + 
					                                        free_curr_chunk_size; 

				if (free_chunks_runner != memory_allocated_end)
				{
					free_curr_chunk_size = 
						   ((variable_chunk_ty *)free_chunks_runner)->chunk_size;
				}                                                                       
			}

			((variable_chunk_ty *)chunks_runner)->chunk_size = 
			                                              merge_free_chunks_size;
			curr_chunk_size = merge_free_chunks_size;  
			merge_free_chunks_size = 0;                                                 
		}

		if (curr_chunk_size >= chunk_size_with_meta_data)
		{
			remain_chunk_free_space = curr_chunk_size -chunk_size_with_meta_data;

			((variable_chunk_ty *)chunks_runner)->chunk_size = SetAsAlloc(chunk_size_with_meta_data);

			if (0 < remain_chunk_free_space)
			{
				remain_chunk = (char *)chunks_runner + chunk_size_with_meta_data;

				((variable_chunk_ty *)remain_chunk)->chunk_size	= 
				                                         remain_chunk_free_space;
			}

			return chunks_runner;

		}

		if (0 > curr_chunk_size)
		{
			curr_chunk_size = SetAsFree(curr_chunk_size);
		}

		chunks_runner = ((char *)chunks_runner) + curr_chunk_size;
	}

	fprintf(stderr, "\nThere is no free space for your requested chunk size.\n");

	return NULL;
}

void VSAFree(void *chunk)
{
	long size_before_free = 0;

	assert(NULL != chunk);

	size_before_free = ((variable_chunk_ty *)chunk)->chunk_size; 
	((variable_chunk_ty *)chunk)->chunk_size = SetAsFree(size_before_free);
}