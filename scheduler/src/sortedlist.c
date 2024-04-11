#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* fprintf */

#include "sortedlist.h"

struct sorted_list
{
  dlist_ty *dlist;
  cmp_func_ty compare_func;
};

sorted_list_ty *SortedListCreate(cmp_func_ty cmp_func)
{
	sorted_list_ty *sorted_list = NULL;

	assert(NULL != cmp_func);
	
	sorted_list = (sorted_list_ty *) malloc(sizeof(sorted_list_ty));
	
	if (NULL == sorted_list)
	{
		fprintf(stderr, "\nThe memory allocation for the sorted linked list\
 failed.\n");
		
		return NULL;
	}
	
	sorted_list->dlist = DListCreate();
	
	if (NULL == sorted_list->dlist)
	{
		fprintf(stderr, "\nThe memory allocation for the double linked list\
 failed.\n");
 
 		free(sorted_list);
 		sorted_list = NULL;
		
		return NULL;
	}
	
	sorted_list->compare_func = cmp_func;
	
	return sorted_list;
}

void SortedListDestroy(sorted_list_ty *sorted_list)
{
	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);
	
	DListDestroy(sorted_list->dlist);
	
	free(sorted_list);
	sorted_list = NULL;
}

sorted_list_status_ty SortedListPopFront(sorted_list_ty *sorted_list)
{
	dlist_status_ty dlist_popfront_status = 0; 

	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);
	
	dlist_popfront_status = DListPopFront(sorted_list->dlist);
	
	if (NO_ERROR != dlist_popfront_status)
	{
		return SORTED_GENERAL_ERR;
	}
	
	return SORTED_NO_ERROR;
}

sorted_list_status_ty SortedListPopBack(sorted_list_ty *sorted_list)
{
	dlist_status_ty dlist_popback_status = 0; 

	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);
	
	dlist_popback_status = DListPopBack(sorted_list->dlist);
	
	if (NO_ERROR != dlist_popback_status)
	{
		return SORTED_GENERAL_ERR;
	}
	
	return SORTED_NO_ERROR;
}

size_t SortedListLength(const sorted_list_ty *sorted_list)
{
	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);
	
	return DListLength(sorted_list->dlist);
}

int SortedListIsEmpty(const sorted_list_ty *sorted_list)
{
	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);
	
	return DListIsEmpty(sorted_list->dlist);
}

sorted_list_iter_ty SortedListBegin(const sorted_list_ty *sorted_list)
{
	sorted_list_iter_ty begin_iter;
	
	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);
	
	begin_iter.internal_itr = DListBegin(sorted_list->dlist);
	
	return begin_iter;
}

sorted_list_iter_ty SortedListEnd(const sorted_list_ty *sorted_list)
{
	sorted_list_iter_ty end_iter;
	
	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);
	
	end_iter.internal_itr = DListEnd(sorted_list->dlist);
	
	return end_iter;
}

sorted_list_iter_ty SortedListNext(sorted_list_iter_ty iter)
{
	assert(NULL != DListIterToNode(iter.internal_itr));
	
	iter.internal_itr = DListNext(iter.internal_itr);
	
	return iter;
}

sorted_list_iter_ty SortedListPrev(sorted_list_iter_ty iter)
{
	assert(NULL != DListIterToNode(iter.internal_itr));
	
	iter.internal_itr =  DListPrev(iter.internal_itr);
	
	return iter;
}

void *SortedListGetData(sorted_list_iter_ty iter)
{
	assert(NULL != DListIterToNode(iter.internal_itr));

	return  DListGetData(iter.internal_itr);
}

int SortedListIterIsEqual(sorted_list_iter_ty first_iter,
                                                sorted_list_iter_ty second_iter)
{
    assert(NULL != DListIterToNode(first_iter.internal_itr));
    assert(NULL != DListIterToNode(second_iter.internal_itr));

    return (DListIterToNode(first_iter.internal_itr) == 
                                     DListIterToNode(second_iter.internal_itr));
} 

sorted_list_iter_ty SortedListInsert(sorted_list_ty *sorted_list, void *data)
{
	sorted_list_iter_ty runner;
	sorted_list_iter_ty loop_end;

	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);
	assert(NULL != data);
	
	runner = SortedListBegin(sorted_list);
	loop_end = SortedListNext(SortedListEnd(sorted_list));
	
	while (!SortedListIterIsEqual(runner, loop_end) && 
	       sorted_list->compare_func(SortedListGetData(runner), data) <= 0)
	{
		runner = SortedListNext(runner);
	}
	
	runner.internal_itr = DListInsert(sorted_list->dlist, 
	                                                 runner.internal_itr, data);                                        

	return runner;
}

sorted_list_iter_ty SortedListRemove(sorted_list_ty *sorted_list, 
                                                  sorted_list_iter_ty to_remove)
{
	sorted_list_iter_ty to_remove_next;

	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);
	assert(NULL != DListIterToNode(to_remove.internal_itr));
	
	to_remove_next.internal_itr = DListRemove(sorted_list->dlist,
	                                                    to_remove.internal_itr);
	                                                   
	return to_remove_next;
}  

int SortedListForEach(sorted_list_iter_ty from, sorted_list_iter_ty to,
                                           op_func_ty op_func, void *func_param)
{
	int op_func_status = 0;

	assert(NULL != DListIterToNode(from.internal_itr));
	assert(NULL != DListIterToNode(to.internal_itr));
	assert(NULL != op_func);
	assert(NULL != func_param);
	
	while (!SortedListIterIsEqual(from, to) &&SORTED_NO_ERROR == op_func_status)
	{
		op_func_status = op_func(SortedListGetData(from), func_param);
		from = SortedListNext(from);
	}
		
	return op_func_status;
} 

sorted_list_iter_ty SortedListFindIf(sorted_list_iter_ty from,
                sorted_list_iter_ty to ,cmp_func_ty cmp_func ,const void *param)
{
	sorted_list_iter_ty from_runner;
	
	assert(NULL != DListIterToNode(from.internal_itr));
	assert(NULL != DListIterToNode(to.internal_itr));
	assert(NULL != cmp_func);
	assert(NULL != param);
	
	from_runner = from;
	
	while (!SortedListIterIsEqual(from_runner, to) &&
           IS_CMP_FUNC_EQUAL != cmp_func(SortedListGetData(from_runner), param))
	{
		from_runner = SortedListNext(from_runner);
	}
		
	return from_runner;
} 

sorted_list_iter_ty SortedListFind(const sorted_list_ty *sorted_list, 
           sorted_list_iter_ty from,sorted_list_iter_ty to ,const void *element)
{
	assert(NULL != sorted_list);
	assert(NULL != sorted_list->dlist);
	assert(NULL != DListIterToNode(from.internal_itr));
	assert(NULL != DListIterToNode(to.internal_itr));
	assert(NULL != element);

	return SortedListFindIf(from, to ,sorted_list->compare_func, element);
} 	

void SortedListMerge(sorted_list_ty *dest, sorted_list_ty *src)
{
	/*sorted_list_iter_ty src_runner;
	sorted_list_iter_ty src_end;*/
	sorted_list_iter_ty where_runner;
	sorted_list_iter_ty from_runner;
	sorted_list_iter_ty to_runner;
	int i = 0;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	/*src_runner = SortedListBegin(src);
	src_end = SortedListNext(SortedListEnd(src));
	
	while (!SortedListIterIsEqual(src_runner, src_end) &&
	       !SortedListIsEmpty(src))
	{
		SortedListInsert(dest, SortedListGetData(src_runner));
		src_runner = SortedListNext(src_runner);
		SortedListPopFront(src);
	}*/
	
	where_runner = SortedListBegin(dest);
	
	while (!SortedListIterIsEqual(where_runner, SortedListNext(SortedListEnd(dest))))
	{
		from_runner = SortedListBegin(src);
		to_runner = from_runner;
		
		while((0 >= dest->compare_func(SortedListGetData(to_runner), SortedListGetData(where_runner))) &&
		       !SortedListIterIsEqual(to_runner, SortedListNext(SortedListEnd(src)))) /* TODO: magic */
		{
			to_runner = SortedListNext(to_runner);
			++i;
		}
		
		if (0 != i)
		{
			DListSplice(from_runner.internal_itr, SortedListPrev(to_runner).internal_itr, SortedListPrev(where_runner).internal_itr);
		}
		
		where_runner = SortedListNext(where_runner);
		i = 0;
	}
	
	if (!SortedListIsEmpty(src))
	{
		DListSplice(SortedListBegin(src).internal_itr, SortedListEnd(src).internal_itr, SortedListPrev(where_runner).internal_itr);	
	}
}






















