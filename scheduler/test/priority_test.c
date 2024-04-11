#include<assert.h> /* assert */
#include <stdio.h> /* printf */
#include <stdio.h> /* fprintf */

#include "priority.h"

#define TESTS_NUM 8

enum {NOT_EMPTY_QUEUE = 0, IS_EMPTY_LENGTH = 0, EMPTY_QUEUE = 1};

typedef priority_queue_status_ty (*piority_queue_test_function_ty)();

/*************** Automate Tests Functions **********************/
static int PriorityCmpFunc(const void *data1, const void *data2);
static int IsMatch(const void *data1, const void *data2);

static priority_queue_status_ty PriorityCreateDestroyTest(void);
static priority_queue_status_ty PriorityEnqueueTest(void);
static priority_queue_status_ty PriorityDequeueTest(void);
static priority_queue_status_ty PriorityPeekTest(void);
static priority_queue_status_ty PriorityIsEmptyTest(void);
static priority_queue_status_ty PrioritySizeTest(void);
static priority_queue_status_ty PriorityClearTest(void);
static priority_queue_status_ty PriorityEraseTest(void);

static priority_queue_status_ty AutomateTests(void);

int main()
{
	priority_queue_status_ty priority_queue_status = 0;
	
	priority_queue_status = AutomateTests();
	
	if (PRIORITY_NO_ERROR != priority_queue_status)
	{
		printf("\n *** There is an ERROR in the program ***\n");
		
		return PRIORITY_GENERAL_ERR;
	}
	
	printf("\n *** All the tests passed, the program work fine ***\n");
	
	return PRIORITY_NO_ERROR;
}

static int PriorityCmpFunc(const void *data1, const void *data2)
{
	assert(NULL != data1);
	assert(NULL != data2);
	
	return ((*(const int *) data1) - (*(const int *) data2)); 
}

static int IsMatch(const void *data1, const void *data2)
{
	assert(NULL != data1);
	assert(NULL != data2);
	
	if ((*(const int *) data2) == (*(const int *) data1))
	{
		return IS_CMP_FUNC_EQUAL;
	}
	
	return IS_CMP_FUNC_NOT_EQUAL;
}

static priority_queue_status_ty PriorityCreateDestroyTest(void)
{
	priority_ty *p_queue = NULL;
	
	p_queue = PriorityCreate(PriorityCmpFunc);
	
	if (NULL == p_queue)
	{
		printf("\n*** The creation of the priority queue failed ***\n");
		
		return PRIORITY_GENERAL_ERR;
	}
	
	PriorityDestroy(p_queue);
	p_queue = NULL;
	
	return PRIORITY_NO_ERROR;
}

static priority_queue_status_ty PriorityEnqueueTest(void)
{
	priority_ty *p_queue = NULL;
	priority_queue_status_ty status = PRIORITY_NO_ERROR;
	int elements_to_enqueue[] = {1, 7, 3};
	int expected_enqueue[] = {1, 3, 7};
	int peek_value = 0;
	size_t elements_arr_size = 3;
	size_t i = 0;
	
	p_queue = PriorityCreate(PriorityCmpFunc);
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		status = PriorityEnqueue(p_queue, (void *) &elements_to_enqueue[i]);
		
		if (PRIORITY_NO_ERROR != status)
		{
			fprintf(stderr,"\nPriorityEnqueue() function doesn't work properly.\
			\n");
			
			PriorityDestroy(p_queue);
			p_queue = NULL;
	
			return status;
		}
		
		printf("\nThe size of the priority queue after the enqueue is: %lu",
		                                                 PrioritySize(p_queue));
	}
	
	i = 0;
	
	while(!PriorityIsEmpty(p_queue))
	{
		peek_value = *(int *)PriorityPeek(p_queue);
		
		if (peek_value != expected_enqueue[i])
		{
			fprintf(stderr,"\nPriorityEnqueue() function doesn't work properly.\
			\n");
			
			PriorityDestroy(p_queue);
			p_queue = NULL;
	
			return PRIORITY_GENERAL_ERR;
		}	
		
		PriorityDequeue(p_queue);
		++i;
	}	
	
	PriorityDestroy(p_queue);
	p_queue = NULL;
	
	return PRIORITY_NO_ERROR;	
}

static priority_queue_status_ty PriorityDequeueTest(void)
{
	priority_ty *p_queue = NULL;
	int elements_to_enqueue[] = {1, 8, 3};
	int peek_value = 0;
	int expected_peek = 8;
	size_t elements_arr_size = 3;
	size_t i = 0;
	
	p_queue = PriorityCreate(PriorityCmpFunc);
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		PriorityEnqueue(p_queue, (void *) &elements_to_enqueue[i]);
	}
	
	printf("\nThe size of the queue after enqueue %lu elements is: %lu",
		                              elements_arr_size, PrioritySize(p_queue));
	
	printf("\nThe size of the queue after dequeue element with data %d is: %lu",
	                   *(int *)PriorityDequeue(p_queue), PrioritySize(p_queue));
	
	printf("\nThe size of the queue after dequeue element with data %d is: %lu",
	                   *(int *)PriorityDequeue(p_queue), PrioritySize(p_queue));
	
	peek_value = *(int *)PriorityPeek(p_queue);
		
	PriorityDestroy(p_queue);
	p_queue = NULL;
	
	if (peek_value != expected_peek)
	{
		fprintf(stderr,"\nPriorityDequeue() function doesn't work properly.\n");
				
		return PRIORITY_GENERAL_ERR;
	}
	
	return PRIORITY_NO_ERROR;		
}

static priority_queue_status_ty PriorityPeekTest(void)
{
	priority_ty *p_queue = NULL;
	int element = 1;
	int peek_value = 0;
	
	p_queue = PriorityCreate(PriorityCmpFunc);
	
	PriorityEnqueue(p_queue, (void *) &element);
	peek_value = *(int *)PriorityPeek(p_queue);
	
	PriorityDestroy(p_queue);
	p_queue = NULL;
	
	if (element != peek_value)
	{
	    fprintf(stderr, "\nPriorityPeek() function doesn't work properly.\n");
    			
    	return PRIORITY_GENERAL_ERR;
    } 	
	
	printf("\nThe data in the front of the queue is: %d", peek_value);
	
	return PRIORITY_NO_ERROR;
}

static priority_queue_status_ty PriorityIsEmptyTest(void)
{
	priority_ty *p_queue = NULL;
	int is_empty = NOT_EMPTY_QUEUE;
	
	p_queue = PriorityCreate(PriorityCmpFunc);
	
    is_empty = PriorityIsEmpty(p_queue);

	PriorityDestroy(p_queue);
	p_queue = NULL;
	
    if (EMPTY_QUEUE != is_empty)
    {
    	fprintf(stderr, "\nPriorityIsEmpty() function doesn't work properly.\n");
    			
    	return PRIORITY_GENERAL_ERR;
    } 

    return PRIORITY_NO_ERROR;
}

static priority_queue_status_ty PrioritySizeTest(void)
{
	priority_ty *p_queue = NULL;
	int is_empty = 0;
	
	p_queue = PriorityCreate(PriorityCmpFunc);
	
    is_empty = PrioritySize(p_queue);

	PriorityDestroy(p_queue);
	p_queue = NULL;
	
    if (IS_EMPTY_LENGTH != is_empty)
    {
    	fprintf(stderr, "\nPrioritySize() function doesn't work properly.\n");
    			
    	return PRIORITY_GENERAL_ERR;
    } 

    return PRIORITY_NO_ERROR;
}

static priority_queue_status_ty PriorityClearTest(void)
{
	priority_ty *p_queue = NULL;
	int elements_to_enqueue[] = {1, 8, 3};
	size_t elements_arr_size = 3;
	size_t i = 0;
	
	p_queue = PriorityCreate(PriorityCmpFunc);
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		PriorityEnqueue(p_queue, (void *) &elements_to_enqueue[i]);
	}
	
	PriorityClear(p_queue);
	
	if (NULL == p_queue || !PriorityIsEmpty(p_queue))
	{
		return PRIORITY_GENERAL_ERR;
	}
	
	return PRIORITY_NO_ERROR;
}

static priority_queue_status_ty PriorityEraseTest(void)
{
	priority_ty *p_queue = NULL;
	int elements_to_enqueue[] = {1, 8, 3};
	int expected_erase[] = {1, 8};
	int element_to_erase = 3;
	int peek_value = 0;
	size_t elements_arr_size = 3;
	size_t i = 0;
	
	p_queue = PriorityCreate(PriorityCmpFunc);
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		PriorityEnqueue(p_queue, (void *) &elements_to_enqueue[i]);
	}
	
	PriorityErase(p_queue, IsMatch, (void *) &element_to_erase);
	
	printf("\nThe size of the queue after erase is: %lu",PrioritySize(p_queue));
	
	i = 0;
	
	while(!PriorityIsEmpty(p_queue))
	{
		peek_value = *(int *)PriorityPeek(p_queue);
		
		if (peek_value != expected_erase[i])
		{
			fprintf(stderr,"\nPriorityErase() function doesn't work properly.\
			\n");
			
			PriorityDestroy(p_queue);
			p_queue = NULL;
	
			return PRIORITY_GENERAL_ERR;
		}	
		
		PriorityDequeue(p_queue);
		++i;
	}	
	
	PriorityDestroy(p_queue);
	p_queue = NULL;
	
	return PRIORITY_NO_ERROR;	
}

static priority_queue_status_ty AutomateTests(void)
{
	priority_queue_status_ty priority_queue_status = 0;
	int i = 0;

	piority_queue_test_function_ty tests[] = 
	{
		PriorityCreateDestroyTest,
		PriorityEnqueueTest,
		PriorityDequeueTest,
		PriorityPeekTest,
		PriorityIsEmptyTest,
		PrioritySizeTest,
		PriorityClearTest,
		PriorityEraseTest
	};
	
	const char* test_func_names[] =
	{
        "PriorityCreateDestroyTest",
        "PriorityEnqueueTest",
        "PriorityDequeueTest",
        "PriorityPeekTest",
        "PriorityIsEmptyTest",
        "PrioritySizeTest",
        "PriorityClearTest",
        "PriorityEraseTest"
    };

	for (i = 0; i < TESTS_NUM; ++i)
    {
    	printf("\nTest N.%d - %s:", i, test_func_names[i]);
        priority_queue_status = tests[i]();
        printf("\n-- Test function return value: %d --\n\n",
                                                         priority_queue_status);
        
        if (PRIORITY_NO_ERROR != priority_queue_status)
        {
        	return PRIORITY_GENERAL_ERR;
        }
    } 

    return PRIORITY_NO_ERROR;
}
