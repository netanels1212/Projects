#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* printf */

#include "doublylinkedlist.h"

#define IS_MATCH 1

struct dnode
{
    void *data;
    dlist_node_ty *next;
    dlist_node_ty *prev;
};

struct dlinkedlist 
{
    dlist_node_ty* head;
    dlist_node_ty* tail;
};

dlist_ty *DListCreate(void)
{
	dlist_ty *dlist = NULL;
	dlist_node_ty *prev_dummy = NULL;
	dlist_node_ty *next_dummy = NULL;
	
	dlist = (dlist_ty *) malloc(sizeof(dlist_ty));
	
	if (NULL == dlist)
	{
		fprintf(stderr, "\nThe memory allocation for the double linked list\
 failed.\n");
		
		return NULL;
	}
	
	prev_dummy = (dlist_node_ty *) malloc(sizeof(dlist_node_ty));
	
	if (NULL == prev_dummy)
	{
		fprintf(stderr, "\nThe memory allocation for the prev dummy failed.\n");
		free(dlist);
		dlist = NULL;
		
		return NULL;
	}
	
	next_dummy = (dlist_node_ty *) malloc(sizeof(dlist_node_ty));
	
	if (NULL == next_dummy)
	{
		fprintf(stderr, "\nThe memory allocation for the next dummy failed.\n");
		
		free(dlist);
		dlist = NULL;
		
		free(prev_dummy);
		prev_dummy = NULL;
		
		return NULL;
	}
	
	prev_dummy->data = NULL;
	next_dummy->data = NULL;
	
	prev_dummy->prev = NULL;
	prev_dummy->next = next_dummy;
	
	next_dummy->prev = prev_dummy;
	next_dummy->next = NULL;
	
	dlist->head = prev_dummy;
	dlist->tail = next_dummy;
	
	return dlist;
}

int DListIsEmpty(const dlist_ty *dlist)
{
	assert(NULL != dlist);
	
	return (dlist->head->next == dlist->tail);
}

dlist_status_ty DListPopFront(dlist_ty *dlist)
{
	dlist_node_ty *new_first_node = NULL;
	
	assert(NULL != dlist);
	
	if (DListIsEmpty(dlist)) /* TODO: no need it? its undefined behaveior?... just only assert check? */
    {
        return GENERAL_ERR;
    }
    
    new_first_node = dlist->head->next->next;
    /*free(dlist->head->next->data);*/
    free(dlist->head->next);
    dlist->head->next = new_first_node;
    new_first_node->prev = dlist->head;
    
    return NO_ERROR;	
}

void DListDestroy(dlist_ty *dlist)
{
	if (NULL == dlist)
	{
		return;
	}
	
	while (!DListIsEmpty(dlist))
	{
		DListPopFront(dlist);
	}
	
	free(dlist->head);
	free(dlist->tail);
	free(dlist);
	dlist = NULL;	
}

dlist_status_ty DListPushFront(dlist_ty *dlist, void *element)
{
	dlist_node_ty *new_node = NULL;	

	assert(NULL != dlist);
	assert(NULL != element);
	
	new_node = (dlist_node_ty *) malloc(sizeof(dlist_node_ty));
	
	if (NULL == new_node)
	{
		fprintf(stderr, "\nThe memory allocation for the new node failed.\n");
		
		return MALLOC_ERROR;
	}
	
	new_node->data = element;
	new_node->next = dlist->head->next;
	new_node->prev = dlist->head;
	(dlist->head->next)->prev = new_node;
	dlist->head->next = new_node;
	
	return NO_ERROR;
}

dlist_status_ty DListPushBack(dlist_ty *dlist, void *element)
{
	dlist_node_ty *new_node = NULL;	

	assert(NULL != dlist);
	assert(NULL != element);
	
	new_node = (dlist_node_ty *) malloc(sizeof(dlist_node_ty));
	
	if (NULL == new_node)
	{
		fprintf(stderr, "\nThe memory allocation for the new dummy failed.\n");
		
		return MALLOC_ERROR;
	}
	
	new_node->data = element;
	new_node->next = dlist->tail;
	new_node->prev = dlist->tail->prev;
	
	(dlist->tail->prev)->next = new_node;
	dlist->tail->prev = new_node;
	
	return NO_ERROR;
}

dlist_status_ty DListPopBack(dlist_ty *dlist)
{
	dlist_node_ty *element_before_last_element = NULL;

	assert(NULL != dlist);
	
	if (DListIsEmpty(dlist))
    {
        return GENERAL_ERR;
    }
    
	element_before_last_element = dlist->tail->prev->prev;
	
	/*free(element_before_last_element->next->data);*/
	free(element_before_last_element->next);
	element_before_last_element->next = dlist->tail;
	dlist->tail->prev = element_before_last_element;
	
	return NO_ERROR;
}

size_t DListLength(const dlist_ty *dlist)
{
	size_t length = 0;
	dlist_node_ty *runner = NULL;
	
	assert(NULL != dlist);
	
	runner = dlist->head->next;
	
	while (runner != dlist->tail)
	{
		++length;
		runner = runner->next;
	}
	
	return length;
}

int DListForEach(dlist_ty *dlist, dlist_iter_ty from, dlist_iter_ty to,
						    int (*action)(void *data, void *param), void *param)
{
	int status = NO_ERROR;
		
	assert(NULL != dlist);
	assert(NULL != from.node);
	assert(NULL != to.node);
	assert(NULL != action);
	assert(NULL != param);
		
	while (from.node != to.node->next && NO_ERROR == status)
	{
		status = action(from.node->data, param);
		from.node = from.node->next;
	}
		
	return status;
}

dlist_iter_ty DListBegin(const dlist_ty *dlist)
{
	dlist_iter_ty begin_iter;	
	
	assert(NULL != dlist);
	
	begin_iter.node = dlist->head->next;
	
	return begin_iter;
}

dlist_iter_ty DListEnd(const dlist_ty *dlist)
{
	dlist_iter_ty end_iter;	
	
	assert(NULL != dlist);
	
	end_iter.node = dlist->tail->prev;
	
	return end_iter;
}

dlist_iter_ty DListNext(dlist_iter_ty iter)
{
	assert(NULL != iter.node);
	
	iter.node = iter.node->next;
	
	return iter;
}

dlist_iter_ty DListPrev(dlist_iter_ty iter)
{
	assert(NULL != iter.node);
	
	iter.node = iter.node->prev;
	
	return iter;
}

void *DListGetData(dlist_iter_ty iter)
{
	assert(NULL != iter.node);
	
	return iter.node->data;
}

void DListSetData(dlist_iter_ty iter, void *data)
{
	assert(NULL != iter.node);
	assert(NULL != data);
	
	/*free(iter.node->data);*/
	iter.node -> data = data;
}

dlist_iter_ty DListInsert(dlist_ty *dlist, dlist_iter_ty where, void *data)
{
	dlist_node_ty *new_node = NULL;

	assert(NULL != dlist);
	assert(NULL != where.node);
	assert(NULL != data);
	
	if (dlist -> head -> next == where.node)
	{
		DListPushFront(dlist, data);
		
		return DListBegin(dlist);
	}
	
	if (dlist -> tail == where.node)
	{
		DListPushBack(dlist, data);
		
		return DListEnd(dlist);
	}
	
	new_node = (dlist_node_ty *) malloc(sizeof(dlist_node_ty));
	
	if (NULL == new_node)
	{
		fprintf(stderr, "\nThe memory allocation for the new node failed.\n");
		where.node = NULL;
		
		return where;                                                           
	}
	
	new_node->data = data;
	
	new_node->next = where.node;
	new_node->prev = where.node->prev;
	
	(where.node->prev)->next = new_node;
	where.node->prev = new_node;
	
	return DListPrev(where);
}

dlist_iter_ty DListRemove(dlist_ty *dlist, dlist_iter_ty to_remove)
{
	dlist_node_ty *return_node = NULL;	

	assert(NULL != dlist);
	assert(NULL != to_remove.node);
	
	if (dlist->head->next == to_remove.node)
	{
		DListPopFront(dlist);
		
		return DListBegin(dlist);
	}
	
	else if (dlist->tail->prev == to_remove.node)
	{
		DListPopBack(dlist);
		
		return DListNext(DListEnd(dlist));
	} 
	
	else
	{
		(to_remove.node->next)->prev = to_remove.node->prev;
		(to_remove.node->prev)->next = to_remove.node->next;
		return_node = to_remove.node->next;
		
		/*free(to_remove.node->data);*/
		free(to_remove.node);
		to_remove.node = NULL;
	}
	
	to_remove.node = return_node;
	
	return to_remove;
}

dlist_ty *DListMultiFind(const dlist_ty *dlist,int (*is_match)(const void *left,
					  const void *right) ,const void *element, dlist_ty *output)
{
	dlist_node_ty *runner = NULL;
	dlist_status_ty status = NO_ERROR;		
	/* dlist_node_ty *match_node_next = NULL; */

	assert(NULL != dlist);
	assert(NULL != is_match);
	assert(NULL != element);
	assert(NULL != output);
	
	runner = dlist->head->next;
	
	while (runner != dlist->tail)
	{
		if (IS_MATCH == is_match(runner->data, element) && NO_ERROR == status)
		{
			status = DListPushBack(output, runner->data);
			/* status = DListPushFront(output, runner->data);

            runner->prev->next = runner->next;
            runner->next->prev = runner->prev;
            match_node_next = runner->next;

            free(runner);
            runner = match_node_next;
        }
        else
        {
            runner = runner->next;
        } */
		}
		
		runner = runner->next;
	}
	
	return output;
}

void DListSplice(dlist_iter_ty from, dlist_iter_ty to, dlist_iter_ty where)
{
	assert(NULL != from.node);
	assert(NULL != to.node);
	assert(NULL != where.node);
	
	(to.node->next)->prev = from.node->prev;
	(from.node->prev)->next = to.node->next;
	
	from.node->prev = where.node;
	to.node->next = where.node->next;
	
	(where.node->next)->prev = to.node;
	where.node->next = from.node;
}

dlist_node_ty *DListIterToNode(dlist_iter_ty iter)
{
	return iter.node;
}
