#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdio.h> /* printf, fprintf */

#include "heap.h"

enum
{
	TESTS_NUM = 6
};

typedef int (*heap_test_func_ty)(void);


int CmpFunc(const void *key1_, const void *key2_, const void *cmp_params_);

static int HeapCreateDestroyTest(void);
static int HeapPeekTest(void);
static int HeapEnqueueTest(void);
static int HeapDequeueTest(void);
static int HeapIsEmptyTest(void);
static int HeapSizeTest(void);

static int AutomateTests(void);

int main()
{
	int heap_status = NO_ERROR;
	
	heap_status = AutomateTests();
	
	if (NO_ERROR != heap_status)
	{
		printf("\n *** There is an ERROR in the program ***\n");
		
		return ERROR;
	}
	
	printf("\n *** All the tests passed, the program work fine ***\n");
	
	return NO_ERROR;
}

int CmpFunc(const void *key1_, const void *key2_, const void *cmp_params_)
{
	assert(NULL != key1_);
	assert(NULL != key2_);
	(void)cmp_params_;

	return (**(int **)key1_ - **(int **)key2_);
}

static int HeapCreateDestroyTest(void)
{
	heap_ty *heap = HeapCreate(CmpFunc, NULL, 5);

	if (NULL == heap)
	{
		fprintf(stderr, "\nHeapCreate() function didn't work properly\n");
		
		return ERROR;
	}

	HeapDestroy(heap);

	return NO_ERROR;
}

static int HeapPeekTest(void)
{
	heap_ty *heap = HeapCreate(CmpFunc, NULL, 5);
	int objects[5] = {3, 4, 2, 5, 1};
	int *heap_data[5] = {0};
	size_t obj_num = 5;
	size_t i = 0;

	for (i = 0; i < obj_num; ++i)
	{
		heap_data[i] = objects + i;
	}

	for (i = 0; i < obj_num; ++i)
	{
		HeapEnqueue(heap, heap_data[i]);
	}

	if (1 != **(int **)HeapPeek(heap))
	{
		fprintf(stderr, "\nHeapPeek() function didn't work properly\n");

		HeapDestroy(heap);

		return ERROR;
	}

	HeapDestroy(heap);

	return NO_ERROR;
}

static int HeapEnqueueTest(void)
{
	heap_ty *heap = HeapCreate(CmpFunc, NULL, 5);
	int objects[5] = {3, 4, 2, 5, 1};
	int *heap_data[5] = {0};
	int enq_status = NO_ERROR;
	size_t obj_num = 5;
	size_t size_after_enq = 0;
	size_t i = 0;

	for (i = 0; i < obj_num; ++i)
	{
		heap_data[i] = objects + i;
	}

	for (i = 0; i < obj_num; ++i)
	{
		enq_status = HeapEnqueue(heap, heap_data[i]);

		if (NO_ERROR != enq_status)
		{
			fprintf(stderr, "\nHeapEnqueue() function didn't work properly\n");
			
			HeapDestroy(heap);

			return ERROR;
		}
	}

	size_after_enq = HeapSize(heap);
	HeapDestroy(heap);

	if (size_after_enq != obj_num)
	{
		fprintf(stderr, "\nHeapEnqueue() function didn't work properly\n");

		return ERROR;
	}

	return NO_ERROR;
}

static int HeapDequeueTest(void)
{
	heap_ty *heap = HeapCreate(CmpFunc, NULL, 5);
	int objects[5] = {3, 4, 2, 5, 1};
	int *heap_data[5] = {0};
	size_t obj_num = 5;
	size_t i = 0;

	for (i = 0; i < obj_num; ++i)
	{
		heap_data[i] = objects + i;
	}

	for (i = 0; i < obj_num; ++i)
	{
		HeapEnqueue(heap, heap_data[i]);
	}

	HeapDequeue(heap);

	if (2 != **(int **)HeapPeek(heap))
	{
		fprintf(stderr, "\nHeapDequeue() function didn't work properly\n");

		HeapDestroy(heap);

		return ERROR;
	}

	HeapDequeue(heap);

	if (3 != **(int **)HeapPeek(heap))
	{
		fprintf(stderr, "\nHeapDequeue() function didn't work properly\n");

		HeapDestroy(heap);

		return ERROR;
	}

	HeapDestroy(heap);

	return NO_ERROR;
}

static int HeapIsEmptyTest(void)
{
	heap_ty *heap = HeapCreate(CmpFunc, NULL, 5);
	int objects[5] = {3, 4, 2, 5, 1};
	int *heap_data[5] = {0};
	size_t obj_num = 5;
	size_t i = 0;

	if (1 != HeapIsEmpty(heap))
	{
		fprintf(stderr, "\nHeapIsEmpty() function didn't work properly\n");

		HeapDestroy(heap);

		return ERROR;
	}

	for (i = 0; i < obj_num; ++i)
	{
		heap_data[i] = objects + i;
	}

	for (i = 0; i < obj_num; ++i)
	{
		HeapEnqueue(heap, heap_data[i]);
	}

	if (0 != HeapIsEmpty(heap))
	{
		fprintf(stderr, "\nHeapIsEmpty() function didn't work properly\n");

		HeapDestroy(heap);

		return ERROR;
	}

	return NO_ERROR;
}

static int HeapSizeTest(void)
{
	heap_ty *heap = HeapCreate(CmpFunc, NULL, 5);
	int objects[5] = {3, 4, 2, 5, 1};
	int *heap_data[5] = {0};
	size_t obj_num = 5;
	size_t i = 0;

	for (i = 0; i < obj_num; ++i)
	{
		heap_data[i] = objects + i;
	}

	for (i = 0; i < obj_num; ++i)
	{
		HeapEnqueue(heap, heap_data[i]);
	}

	HeapDequeue(heap);

	if ((obj_num - 1) != HeapSize(heap))
	{
		fprintf(stderr, "\nHeapSize() function didn't work properly\n");

		HeapDestroy(heap);

		return ERROR;
	}

	HeapDestroy(heap);

	return NO_ERROR;
}

static int AutomateTests(void)
{
	int status = 0;
	int i = 0;

	heap_test_func_ty tests[] = 
	{
		HeapCreateDestroyTest,
		HeapPeekTest,
		HeapEnqueueTest,
		HeapDequeueTest,
		HeapIsEmptyTest,
		HeapSizeTest
	};
	
	const char* test_func_names[] =
	{
		"HeapCreateDestroyTest",
		"HeapPeekTest",
		"HeapEnqueueTest",
		"HeapDequeueTest",
		"HeapIsEmptyTest",
		"HeapSizeTest"
    };

	for (i = 0; i < TESTS_NUM; ++i)
    {
    	printf("\nTest N.%d - %s:", i, test_func_names[i]);
        status = tests[i]();
        printf("\n-- Test function return value: %d, ", status);
        
        if (NO_ERROR != status)
        {
        	printf("not passed --\n\n");

        	return ERROR;
        }

        else
        {
            printf("passed! --\n\n");
        }
    } 

    return NO_ERROR;
}

