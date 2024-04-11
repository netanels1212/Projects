#include <stdio.h> /* printf */
#include <stdio.h> /* fprintf */
#include <assert.h> /* assert */

#include "singlylinkedlist.h" 
#include "queue.h"

#define TESTS_NUM 7

enum {NOT_EMPTY_QUEUE = 0, IS_EMPTY_LENGTH = 0, EMPTY_QUEUE = 1};

typedef status_ty (*queue_test_function_ty)();

/********************************************************************************** TODO: remove check after create, and return the correct values */

/*********** Automate Tests Functions **************/
static status_ty QCreateDestroyTest(void);
static status_ty QEnqueueTest(void);
static status_ty QDequeueTest(void);
static status_ty QIsEmptyTest(void);
static status_ty QPeekTest(void);
static status_ty QSizeTest(void);
static status_ty QAppendTest(void);

static status_ty AutomateTests(void);

int main()
{
	status_ty queue_status = 0;
	
	queue_status = AutomateTests();
	
	if (QSUCCESS != queue_status)
	{
		printf("\n *** There is an ERROR in the program ***\n");
		
		return QFAIL;
	}
	
	printf("\n *** All the tests passed, the program work fine ***\n");
	
	return QSUCCESS;
}

static status_ty QCreateDestroyTest(void)
{
	queue_ty *queue = QCreate();
	
	if (NULL == queue)
	{
		printf("\n*** The creation of the queue failed ***\n");
			
		return QFAIL;
	}

	    
 	QDestroy(queue);
	queue = NULL; 
	
	return QSUCCESS;
}

static status_ty QEnqueueTest(void)
{
	queue_ty *queue = NULL;
	status_ty status = SUCCESS;
	int elements_to_enqueue[] = {1, 2, 3};
	int peek_value = 0;
	size_t elements_arr_size = 3;
	size_t i = 0;
	
	queue = QCreate();
	
	if (NULL == queue)
	{
		printf("\nThe creation of the queue failed\n");
			
		return QMEM_FAIL;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		status = QEnqueue(queue, (void *) &elements_to_enqueue[i]);
		
		if (SUCCESS != status)
		{
			fprintf(stderr, "\nQEnqueue() function doesn't work properly.\n");
			
			QDestroy(queue);
			queue = NULL; 
	
			return status;
		}
		
		printf("\nThe size of the queue after the enqueue is: %lu",
		                                                          QSize(queue));
	}
	
	i = 0;
	
	while(!QIsEmpty(queue))
	{
		peek_value = *(int *)QPeek(queue);
		
		if (peek_value != elements_to_enqueue[i])
		{
			fprintf(stderr, "\nQEnqueue() function doesn't work properly.\n");
			
			QDestroy(queue);
			queue = NULL; 
	
			return QFAIL;
		}	
		
		QDequeue(queue);
		++i;
	}	
	
	QDestroy(queue);
	queue = NULL; 
	
	return QSUCCESS;	
}

static status_ty QDequeueTest(void)
{
	queue_ty *queue = NULL;
	int elements_to_enqueue[] = {1, 2, 3};
	int peek_value = 0;
	size_t elements_arr_size = 3;
	size_t i = 0;
	
	queue = QCreate();
	
	if (NULL == queue)
	{
		printf("\nThe creation of the queue failed\n");
			
		return QMEM_FAIL;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		QEnqueue(queue, (void *) &elements_to_enqueue[i]);
	}
	
	printf("\nThe size of the queue after the enqueue %lu elements is: %lu",
		                                       elements_arr_size, QSize(queue));
	QDequeue(queue);
	printf("\nThe size of the queue after the dequeue is: %lu", QSize(queue));
	
	QDequeue(queue);
	printf("\nThe size of the queue after the dequeue is: %lu", QSize(queue));
	
	peek_value = *(int *)QPeek(queue);
		
	QDestroy(queue);
	queue = NULL; 
	
	if (peek_value != elements_to_enqueue[elements_arr_size - 1])
	{
		fprintf(stderr, "\nQDequeue() function doesn't work properly.\n");
				
		return QFAIL;
	}
	
	return QSUCCESS;		
}

static status_ty QIsEmptyTest(void)
{
	queue_ty *queue = NULL;
	int is_empty = NOT_EMPTY_QUEUE;
	
	queue = QCreate();
	
	if (NULL == queue)
	{
		printf("\nThe creation of the queue failed\n");
			
		return QMEM_FAIL;
	}
	
    is_empty = QIsEmpty(queue);

	QDestroy(queue);
	queue = NULL;
	
    if (EMPTY_QUEUE != is_empty)
    {
    	fprintf(stderr, "\nQIsEmpty() function doesn't work properly.\n");
    			
    	return QFAIL;
    } 

    return QSUCCESS;
}

static status_ty QPeekTest(void)
{
	queue_ty *queue = NULL;
	int element = 1;
	int peek_value = 0;
	
	queue = QCreate();
	
	if (NULL == queue)
	{
		printf("\nThe creation of the queue failed\n");
			
		return QMEM_FAIL;
	}
	
	QEnqueue(queue, (void *) &element);
	peek_value = *(int *)QPeek(queue);
	
	QDestroy(queue);
	queue = NULL;
	
	if (element != peek_value)
	{
	    fprintf(stderr, "\nQPeek() function doesn't work properly.\n");
    			
    	return QFAIL;
    } 	
	
	printf("\nThe data in the front of the queue is: %d", peek_value);
	
	return QSUCCESS;
}

static status_ty QSizeTest(void)
{
	queue_ty *queue = NULL;
	int is_empty = 0;
	
	queue = QCreate();
	
	if (NULL == queue)
	{
		printf("\nThe creation of the queue failed\n");
			
		return QMEM_FAIL;
	}
	
    is_empty = QSize(queue);

	QDestroy(queue);
	queue = NULL;
	
    if (IS_EMPTY_LENGTH != is_empty)
    {
    	fprintf(stderr, "\nQSize() function doesn't work properly.\n");
    			
    	return QFAIL;
    } 

    return QSUCCESS;
}

static status_ty QAppendTest(void)
{
	queue_ty *src_queue = NULL;
	queue_ty *dest_queue = NULL;
	int first_element = 2;
	int second_element = 3;
	size_t dest_queue_size = 0;
	size_t src_queue_size = 0;
	size_t size_after_append = 0;
	
	dest_queue = QCreate();
	
	if (NULL == dest_queue)
	{
		printf("\nThe creation of the dest queue failed.\n");
			
		return MEM_FAIL;
	}
	
	QEnqueue(dest_queue, (void *) &second_element);
	QEnqueue(dest_queue, (void *) &first_element);
	dest_queue_size = QSize(dest_queue);
	
	src_queue = QCreate();
	
	if (NULL == src_queue)
	{
		printf("\nThe creation of the source queue failed.\n");
		
		QDestroy(dest_queue);
		dest_queue = NULL;
			
		return MEM_FAIL;
	}
	
	QEnqueue(src_queue, (void *) &first_element);
	QEnqueue(src_queue, (void *) &second_element);
	src_queue_size = QSize(src_queue);
	
	QAppend(dest_queue, src_queue);
	size_after_append = QSize(dest_queue);
	
	printf("\nAfter append queue with size 2 to the original queue,\
 the size of the queue is: %lu", size_after_append);
	
	QDestroy(dest_queue);
	dest_queue = NULL;
		
	QDestroy(src_queue);
	src_queue = NULL;
	
	if ((dest_queue_size + src_queue_size) != size_after_append)
    {
    	fprintf(stderr, "\nQAppend() function doesn't work properly.\n");
    			
    	return QFAIL;
    } 
	
	return QSUCCESS;
}

static status_ty AutomateTests(void)
{
	status_ty status = 0;
	int i = 0;
	
	queue_test_function_ty tests[] = 
	{
		QCreateDestroyTest,
		QEnqueueTest,
		QDequeueTest,
		QIsEmptyTest,
		QPeekTest,
		QSizeTest,
		QAppendTest	
	};
	
	const char* test_func_names[] =
	{
        "QCreateDestroyTest",
        "QEnqueueTest",
        "QDequeueTest",
        "QIsEmptyTest",
        "QPeekTest",
        "QSizeTest",
        "QAppendTest"
    };
	
	for (i = 0; i < TESTS_NUM; ++i)
    {
    	printf("\nTest N.%d - %s:", i, test_func_names[i]);
        status = tests[i]();
        printf("\n-- Test function return value: %d --\n\n", status);
        
        if (QSUCCESS != status)
        {
        	return QFAIL;
        }
    }  

    return QSUCCESS;
}

