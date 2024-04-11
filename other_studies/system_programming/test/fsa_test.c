#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* printf, fprintf */

#include "fsa.h"

#define TESTS_NUM 3

typedef enum 
{
	FSA_NO_ERROR = 0,
	FSA_ERROR = 1
} fsa_status_ty;

typedef fsa_status_ty (*fsa_test_function_ty)();

/*************** Automate Tests Functions **********************/
static fsa_status_ty FSAllocSuggestSizeTest();
static fsa_status_ty FSAInitTest();
static fsa_status_ty FSAAllocFreeCountTest();

static fsa_status_ty AutomateTests(void);

int main()
{
	fsa_status_ty fsa_status = FSA_NO_ERROR;
	
	fsa_status = AutomateTests();
	
	if (FSA_NO_ERROR != fsa_status)
	{
		printf("\n *** There is an ERROR in the program ***\n");
		
		return FSA_ERROR;
	}
	
	printf("\n *** All the tests passed, the program work fine ***\n");
	
	return FSA_NO_ERROR;
}

static fsa_status_ty FSAllocSuggestSizeTest()
{
	size_t num_of_blocks[] = {1, 3, 6, 8};
	size_t block_size[] = {1, 7, 21, 32};
	size_t expected_size[] = {16, 32, 152, 264};
	int num_of_tests = 4;
	int i = 0;

	for (i = 0; i < num_of_tests; ++i)
	{
		if (expected_size[i] != 
			                 FSAllocSuggestSize(num_of_blocks[i], block_size[i]))
		{
			fprintf(stderr, "\nThe function FSAllocSuggestSize() didn't work \
properly.\n");
			fprintf(stderr, "Fall in case %d.\n", i);

			return FSA_ERROR;
		}
	}

	return FSA_NO_ERROR;
}

static fsa_status_ty FSAInitTest()
{
	fsa_status_ty fsa_init_status = FSA_NO_ERROR;
	void *allocated_memory = NULL;
	size_t memory_size = 1000;
	size_t block_size[] = {1, 8, 12, 21};
	size_t expected_num_of_blocks[] = {124, 124, 62, 41};
	int num_of_tests = 4;
	int i = 0;

	for (i = 0; i < num_of_tests; ++i)
	{
		allocated_memory = (void *)malloc(memory_size);

		if (NULL == allocated_memory)
		{
			fprintf(stderr, "\nThe memory allocation weithin failed.\n");

			return FSA_ERROR;
		}

		FSAInit(allocated_memory, memory_size, block_size[i]);

		if (expected_num_of_blocks[i] != FSACountFree(allocated_memory))
		{
			fprintf(stderr, "\nThe function FSAInit() didn't work properly.\n");
			fprintf(stderr, "Fall in case %d.\n", i);

			fsa_init_status = FSA_ERROR;
		}

		free(allocated_memory);
		allocated_memory = NULL;
	}

	return fsa_init_status;
}

static fsa_status_ty FSAAllocFreeCountTest()
{
	fsa_status_ty status = FSA_NO_ERROR;
	void *allocated_memory = NULL;
	void *block_to_free = NULL;
	void *new_alloc_block_after_free = NULL;
	size_t memory_size = 1000;
	size_t block_size = 8;

	allocated_memory = (void *)malloc(memory_size);

	if (NULL == allocated_memory)
	{
		fprintf(stderr, "\nThe memory allocation weithin failed.\n");

		return FSA_ERROR;
	}

	FSAInit(allocated_memory, memory_size, block_size);

	FSAAlloc(allocated_memory);
	block_to_free = FSAAlloc(allocated_memory);
	FSAAlloc(allocated_memory);

	FSAFree(allocated_memory, block_to_free);
	new_alloc_block_after_free = FSAAlloc(allocated_memory);

	if (new_alloc_block_after_free != block_to_free ||
	                                       121 != FSACountFree(allocated_memory))
	{
		fprintf(stderr, "\nThe function FSAAlloc() or FSAFree() or FSACount()\
didn't work properly.\n");   

		status = FSA_ERROR;
	}

	free(allocated_memory);
	allocated_memory = NULL;

	return status;
}

static fsa_status_ty AutomateTests(void)
{
	fsa_status_ty fsa_status = 0;
	int i = 0;

	fsa_test_function_ty tests[] = 
	{
		FSAllocSuggestSizeTest,
		FSAInitTest,
		FSAAllocFreeCountTest
	};
	
	const char* test_func_names[] =
	{
       "FSAllocSuggestSizeTest",
       "FSAInitTest",
       "FSAAllocFreeCountTest"
    };

	for (i = 0; i < TESTS_NUM; ++i)
    {
    	printf("\nTest N.%d - %s:", i, test_func_names[i]);
        fsa_status = tests[i]();
        printf("\n-- Test function return value: %d, ", fsa_status);
        
        if (FSA_NO_ERROR != fsa_status)
        {
        	printf("not passed --\n\n");

        	return FSA_ERROR;
        }

        else
        {
            printf("passed! --\n\n");
        }
    } 

    return FSA_NO_ERROR;
}



