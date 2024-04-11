#include <stdio.h> /* fprintf */
#include <ctype.h> /* size_t */
#include <stdlib.h> /* malloc, realloc, free */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */

#include "vector.h"

#define MAX_CAPACITY 1000
#define GROWTH_FACTOR 1.5
#define SHRINK_FACTOR 1.5

struct vector
{
    void* start;         
    size_t element_size;
    size_t size;         
    size_t capacity;           
};

vector_ty *VectorCreate(size_t capacity , size_t element_size)
{
	vector_ty *vector = NULL;
	
	assert(MAX_CAPACITY > capacity && 0 < capacity);
	
	vector = (vector_ty *) malloc(sizeof(vector_ty));
	if (NULL == vector)
	{
		fprintf(stderr, "Memory allocation failed.\n");
		
		return NULL;
	}

	vector -> start = (void *) malloc(capacity * element_size);
	if (NULL == vector -> start)
	{
		fprintf(stderr, "Memory allocation failed.\n");
		free(vector);
		vector = NULL;
		
		return NULL;
	}
	
	vector -> element_size = element_size;
	vector -> size = 0;
	vector -> capacity = capacity;
	
	return vector;
}

void VectorDestroy(vector_ty *vector)
{
	assert(NULL != vector);
	
	free(vector -> start);
	vector -> start = NULL;
	
	free(vector);
	vector = NULL;
}

size_t VectorGetSize(vector_ty *vector)
{
	assert(NULL != vector);
	
	return (vector -> size);
}

size_t VectorGetMaxCapacity(vector_ty *vector)
{
	assert(NULL != vector);
	
	return (vector -> capacity);
}

status_ty VectorIsEmpty(vector_ty *vector)
{
	assert(NULL != vector);
	
	return (0 == vector -> size);
}

void PrintIntVector(vector_ty *vector)
{
	size_t i = 0;
	
	assert(NULL != vector);
	
	if (!VectorIsEmpty(vector))
	{
		printf("\nThe vector elements are:\n"); 
		for (i = 0; i < (vector -> size); ++i)
		{
			printf("%d ", ((int *)(vector -> start))[i]);
		}
		
		printf("\n");
	}
	
	else
	{
		printf("\nThe vector is empty.\n");
	}
}

status_ty VectorReserve(vector_ty *vector, size_t new_capacity)
{
	size_t num_copy_elements = 0;
	
	assert(NULL != vector);
	assert(MAX_CAPACITY > vector->capacity && 0 < vector->capacity);
	
	if (new_capacity != (vector -> capacity))
	{
		
		num_copy_elements = (new_capacity < (vector -> size)) ? new_capacity
		                                                     : (vector -> size);
		vector -> start = (void *)realloc(vector -> start,
		                               new_capacity * (vector -> element_size));
		if (NULL == vector)
		{
			fprintf(stderr, "Memory allocation failed.\n");
		
			return FAIL;
		}
		
		vector -> size = num_copy_elements;
		vector -> capacity = new_capacity;
	}
	
	return SUCCESS;
}

status_ty VectorShrinkToFit(vector_ty *vector)
{
	assert(NULL != vector);
	
	return VectorReserve(vector, (vector -> size));
}

void *VectorGetValue(vector_ty *vector, size_t index)
{
	assert(NULL != vector);
	assert(index < (vector -> size));
	
	return (void*)((index*(vector -> element_size)) + (char*)(vector -> start));	
}

void VectorSetValue(vector_ty *vector, size_t index, void* val)
{
	assert(NULL != vector);
	assert(index < (vector -> size));
	
	memcpy((char *) VectorGetValue(vector, index),
	       (const char *) val, vector -> element_size);
}

status_ty VectorPushBack(vector_ty *vector, void *element)
{
	size_t reserve_grwoth = 0;

	assert(NULL != vector);
	
	if ((vector -> size) == (vector -> capacity))
	{
		reserve_grwoth = (vector -> capacity) * GROWTH_FACTOR;
		VectorReserve(vector, reserve_grwoth);
		
		if (NULL == vector)
		{
			fprintf(stderr, "Memory allocation failed.\n");
			
			return FAIL;
		}	
	}
	
	++(vector -> size);
	VectorSetValue(vector, (vector -> size - 1) ,element);
	
	return SUCCESS;
}

status_ty VectorPopBack(vector_ty *vector)
{
	size_t reserve_shrink = 0;

	assert(NULL != vector);
	
	reserve_shrink = (vector -> capacity) / SHRINK_FACTOR;
	if (0 < (vector -> size))
	{
		if ((vector -> size) < reserve_shrink)
		{
			VectorReserve(vector, reserve_shrink);
			
			if (NULL == vector)
			{
				fprintf(stderr, "Memory allocation failed.\n");
				
				return FAIL;
			}
		}
	
		--(vector -> size);
	}
	
	else
	{
		fprintf(stderr, "Can't pop from empty vector.\n");
				
		return FAIL;
	}
	
	return SUCCESS;	
}
