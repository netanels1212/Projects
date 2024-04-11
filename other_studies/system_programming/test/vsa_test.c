#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* printf, fprintf */

#include "vsa.h"

#define TESTS_NUM 4

typedef enum 
{
	VSA_NO_ERROR = 0,
	VSA_ERROR = 1
} vsa_status_ty;

typedef vsa_status_ty (*vsa_test_function_ty)();

/*************** Automate Tests Functions **********************/
static vsa_status_ty VSAInitTest(void);
static vsa_status_ty VSAAllocTest(void);
static vsa_status_ty VSAFreeTest(void);
static vsa_status_ty VSALargestChunkAvailableTest(void);

static vsa_status_ty AutomateTests(void);

int main()
{
	vsa_status_ty vsa_status = VSA_NO_ERROR;
	
	vsa_status = AutomateTests();
	
	if (VSA_NO_ERROR != vsa_status)
	{
		printf("\n *** There is an ERROR in the program ***\n");
		
		return VSA_ERROR;
	}
	
	printf("\n *** All the tests passed, the program work fine ***\n");
	
	return VSA_NO_ERROR;
}

static vsa_status_ty VSAInitTest(void)
{
	size_t memory_size = 30;
	void *allocated_memory = (void *)malloc(memory_size);
	vsa_ty *vsa = NULL;
	vsa_status_ty init_status = VSA_NO_ERROR;

	if (NULL == allocated_memory)
	{
		fprintf(stderr, "\nThe memory allocation weithin failed.\n");

		return VSA_ERROR;
	}

	vsa = VSAInit(allocated_memory, memory_size);

	if (NULL == vsa)
	{
		fprintf(stderr, "\nThe function VSAInit() didn't work properly.\n");

		init_status = VSA_ERROR;	
	}

	free(allocated_memory);
	allocated_memory = NULL;

	return init_status;
}

static vsa_status_ty VSAAllocTest(void)
{
	size_t memory_size = 100;
	void *allocated_memory = (void *)malloc(memory_size);
	void *second_alloc_chunk = NULL;
	void *third_alloc_chunk = NULL;
	void *test_output_alloc_chunk = NULL;
	vsa_ty *vsa = NULL;
	vsa_status_ty alloc_status = VSA_NO_ERROR;

	if (NULL == allocated_memory)
	{
		fprintf(stderr, "\nThe memory allocation weithin failed.\n");

		return VSA_ERROR;
	}

	vsa = VSAInit(allocated_memory, memory_size);

	VSAAlloc(vsa, 5);
	second_alloc_chunk = VSAAlloc(vsa, 7);
	third_alloc_chunk = VSAAlloc(vsa, 10);	
	VSAAlloc(vsa, 4);	
	VSAAlloc(vsa, 16);	

	VSAFree(second_alloc_chunk);
	VSAFree(third_alloc_chunk);

	test_output_alloc_chunk = VSAAlloc(vsa, 8);

	if (test_output_alloc_chunk != second_alloc_chunk)
	{
		fprintf(stderr, "\nThe function VSAAlloc() didn't work properly.\n");

		alloc_status = VSA_ERROR;	
	}

	free(allocated_memory);
	allocated_memory = NULL;

	return alloc_status;
}

static vsa_status_ty VSAFreeTest(void)
{
	size_t memory_size = 100;
	void *allocated_memory = (void *)malloc(memory_size);
	void *alloc_chunk_ptr = NULL;
	vsa_ty *vsa = NULL;
	vsa_status_ty free_status = VSA_NO_ERROR;

	if (NULL == allocated_memory)
	{
		fprintf(stderr, "\nThe memory allocation weithin failed.\n");

		return VSA_ERROR;
	}

	vsa = VSAInit(allocated_memory, memory_size);

	alloc_chunk_ptr	= VSAAlloc(vsa, 5);
	VSAFree(alloc_chunk_ptr);

	if (92 != VSALargestChunkAvailable(vsa))
	{
		fprintf(stderr, "\nThe function VSAFree() didn't work properly.\n");
	
		free_status = VSA_ERROR;
	}

	free(allocated_memory);
	allocated_memory = NULL;

	return free_status;	
}

static vsa_status_ty VSALargestChunkAvailableTest(void)
{
	size_t memory_size = 100;
	void *allocated_memory = (void *)malloc(memory_size);
	void *second_alloc_chunk = NULL;
	void *third_alloc_chunk = NULL;
	vsa_ty *vsa = NULL;
	vsa_status_ty largest_chunk_status = VSA_NO_ERROR;

	if (NULL == allocated_memory)
	{
		fprintf(stderr, "\nThe memory allocation weithin failed.\n");

		return VSA_ERROR;
	}

	vsa = VSAInit(allocated_memory, memory_size);

	if (92 != VSALargestChunkAvailable(vsa))
	{
		fprintf(stderr, "\nThe function VSALargestChunkAvailable() didn't work\
 properly.\n");
	
		free(allocated_memory);
		allocated_memory = NULL;

		return VSA_ERROR;
	}

	VSAAlloc(vsa, 5);
	second_alloc_chunk = VSAAlloc(vsa, 7);
	third_alloc_chunk = VSAAlloc(vsa, 10);	
	VSAAlloc(vsa, 4);	
	VSAAlloc(vsa, 16);	

	VSAFree(second_alloc_chunk);
	VSAFree(third_alloc_chunk);

	if (33 != VSALargestChunkAvailable(vsa))
	{
		fprintf(stderr, "\nThe function VSALargestChunkAvailable() didn't work\
 properly.\n");

		largest_chunk_status = VSA_ERROR;	
	}

	free(allocated_memory);
	allocated_memory = NULL;

	return largest_chunk_status;
}

static vsa_status_ty AutomateTests(void)
{
	vsa_status_ty vsa_status = 0;
	int i = 0;

	vsa_test_function_ty tests[] = 
	{
		VSAInitTest,
		VSAAllocTest,
		VSAFreeTest,
		VSALargestChunkAvailableTest
	};
	
	const char* test_func_names[] =
	{
		"VSAInitTest",
		"VSAAllocTest",
		"VSAFreeTest",
		"VSALargestChunkAvailableTest"
    };

	for (i = 0; i < TESTS_NUM; ++i)
    {
    	printf("\nTest N.%d - %s:", i, test_func_names[i]);
        vsa_status = tests[i]();
        printf("\n-- Test function return value: %d, ", vsa_status);
        
        if (VSA_NO_ERROR != vsa_status)
        {
        	printf("not passed --\n\n");

        	return VSA_ERROR;
        }

        else
        {
            printf("passed! --\n\n");
        }
    } 

    return VSA_NO_ERROR;
}
