#include <stddef.h> /*size_t*/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* fprintf */

#include "priority.h"
#include "sortedlist.h"


struct priority_queue
{
    sorted_list_ty *sorted_list;
};

priority_ty *PriorityCreate(cmp_func_ty compare_func)
{
	priority_ty *p_queue = NULL;
	
	assert(NULL != compare_func);
	
	p_queue = (priority_ty *) malloc(sizeof(priority_ty));
	
	if (NULL == p_queue)
	{
		fprintf(stderr, "\nThe memory allocation for the priority queue failed.\
		\n");
		
		return NULL;
	}
	
	p_queue -> sorted_list = SortedListCreate(compare_func);
	
	if (NULL == (p_queue -> sorted_list))
	{
		fprintf(stderr,"\nThe memory allocation for the sorted list failed.\n");
		
		free(p_queue);
		p_queue = NULL;
		
		return NULL;
	}
	
	return p_queue;
}

void PriorityDestroy(priority_ty *p_queue)
{
	assert(NULL != p_queue);
	assert(NULL != p_queue -> sorted_list);
	
	SortedListDestroy(p_queue -> sorted_list);
	
	free(p_queue);
	p_queue = NULL;	
}

int PriorityIsEmpty(const priority_ty *p_queue)
{
	assert(NULL != p_queue);
	assert(NULL != p_queue -> sorted_list);
	
	return SortedListIsEmpty(p_queue -> sorted_list);
}

priority_queue_status_ty PriorityEnqueue(priority_ty *p_queue, void *data)
{
	assert(NULL != p_queue);
	assert(NULL != p_queue -> sorted_list);
	assert(NULL != data);
	
	SortedListInsert(p_queue -> sorted_list, data);
	
	if (PriorityIsEmpty(p_queue))
	{
		return PRIORITY_GENERAL_ERR;
	}
	
	return PRIORITY_NO_ERROR;	
}

void *PriorityPeek(priority_ty *p_queue)
{
	assert(NULL != p_queue);
	assert(NULL != p_queue -> sorted_list);
	
	return SortedListGetData(SortedListBegin(p_queue -> sorted_list));
}

void *PriorityDequeue(priority_ty *p_queue)
{
	void *remove_data = NULL;
	sorted_list_status_ty sorted_list_popfront_status = SORTED_NO_ERROR;

	assert(NULL != p_queue);
	assert(NULL != p_queue -> sorted_list);
	
	remove_data = PriorityPeek(p_queue);
	sorted_list_popfront_status = SortedListPopFront(p_queue -> sorted_list);
	
	if (SORTED_NO_ERROR != sorted_list_popfront_status)
	{
		return NULL;
	}
	
	return remove_data;
}

size_t PrioritySize(const priority_ty *p_queue)
{
	assert(NULL != p_queue);
	assert(NULL != p_queue -> sorted_list);

	return SortedListLength(p_queue -> sorted_list);
}

void PriorityClear(priority_ty *p_queue)
{
	assert(NULL != p_queue);
	assert(NULL != p_queue -> sorted_list);
	
	while (!PriorityIsEmpty(p_queue))
	{
		PriorityDequeue(p_queue);
	}
}

void *PriorityErase(priority_ty *p_queue,cmp_func_ty is_match, void *data)
{
	sorted_list_iter_ty element_to_erase;
	sorted_list_iter_ty from;
	sorted_list_iter_ty to;
	void *to_erase_data = NULL;
	
	assert(NULL != p_queue);
	assert(NULL != p_queue -> sorted_list);
	assert(NULL != is_match);
	assert(NULL != data);
	
	from = SortedListBegin(p_queue -> sorted_list);
	to = SortedListNext(SortedListEnd(p_queue -> sorted_list));
	
	element_to_erase = SortedListFindIf(from, to, is_match, data);
	
	to_erase_data = SortedListGetData(element_to_erase);
	SortedListRemove(p_queue -> sorted_list, element_to_erase);
	
	return to_erase_data;
}                                                                    
