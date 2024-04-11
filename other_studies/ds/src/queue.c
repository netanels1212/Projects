#include <stddef.h> /*size_t*/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* fprintf */

#include "singlylinkedlist.h" 
#include "queue.h"

struct queue
{
	slist_ty *slist;
};

queue_ty *QCreate()
{
    queue_ty *queue = NULL; 
    queue = (queue_ty *) malloc(sizeof(queue_ty));
    
	if (NULL == queue)
	{
		fprintf(stderr, "\nThe memory allocation for the queue failed.\n");
		
		return NULL;
	}
    
    queue -> slist = SListCreate();
    
    if (NULL == queue -> slist)
	{
		fprintf(stderr, "\nThe memory allocation for the list failed.\n");
		
		free(queue);
		queue = NULL;
	}
    
    return queue;
}

void QDestroy(queue_ty *queue)
{
    assert(NULL != queue);
    assert(NULL != queue -> slist);
    
    SListDestroy(queue -> slist);
    free(queue);
    queue = NULL;
}

status_ty QEnqueue(queue_ty *queue, const void *data)
{
	status_ty slist_push_status = SUCCESS;

 	assert(NULL != queue);
 	assert(NULL != queue -> slist);
 	assert(NULL != data);
 	
 	slist_push_status = SListPushBack(queue -> slist, (void *) data);
 	
 	if (SUCCESS != slist_push_status)
 	{
 		return QFAIL;
 	}
 	
 	return QSUCCESS;
}

void QDequeue(queue_ty *queue)
{
 	assert(NULL != queue);
 	assert(NULL != queue -> slist);
 	
 	SListPopFront(queue -> slist); 
}

status_ty QIsEmpty(const queue_ty *queue)
{
	assert(NULL != queue);
	assert(NULL != queue -> slist);
	
	return SListIsEmpty(queue -> slist); 
}

void *QPeek(const queue_ty *queue)
{
	assert(NULL != queue);
	assert(NULL != queue -> slist);
	
	return SListGetData(SListBegin(queue -> slist));
}

size_t QSize(const queue_ty *queue)
{
	assert(NULL != queue);
	assert(NULL != queue -> slist);

	return SListLength(queue -> slist);
}

void *QAppend(queue_ty *dest, queue_ty *src)
{
	assert(NULL != dest);
	assert(NULL != src);
	assert(NULL != dest -> slist);
	assert(NULL != src -> slist);
	
	SListAppend(dest -> slist, src -> slist);
	
	return SListBegin(dest -> slist);
}
