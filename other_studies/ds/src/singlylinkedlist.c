#include <stddef.h> /*size_t*/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* fprintf */

#include "singlylinkedlist.h"

#define EQUAL 1

struct slist_node 
{
    void *data;
    slist_node_ty* next;
};

struct linkedlist 
{
    slist_node_ty* head;
    slist_node_ty* tail;
};

slist_ty *SListCreate(void)
{
	slist_node_ty *dummy = NULL;
	slist_ty *list = NULL;
	
	list = (slist_ty *) malloc(sizeof(slist_ty));
	if (NULL == list)
	{
		fprintf(stderr, "\nThe memory allocation for the list failed.\n");
		
		return NULL;
	}
	
	dummy = (slist_node_ty *) malloc(sizeof(slist_node_ty));
	if (NULL == dummy)
	{
		fprintf(stderr, "\nThe memory allocation for the dummy node failed.\n");
		free(list);
		list = NULL;
		
		return NULL;
	}
	
	dummy -> data = NULL;
	dummy -> next = NULL;
	
	list -> head = dummy;
	list -> tail = dummy;
	
	return list;	
}

int SListIsEmpty(const slist_ty *list)
{
	assert(NULL != list);

	return (list -> head == list -> tail);
}

status_ty SListPopFront(slist_ty *list)
{
	slist_node_ty *new_head = NULL;
	
	assert(NULL != list);
	
	if (SListIsEmpty(list))
    {
        return SUCCESS;
    }
    
	new_head = list -> head -> next;
	/*free(list -> head -> data);*/
	free(list -> head);
	list -> head = new_head;
	
	return SUCCESS;
}

void SListDestroy(slist_ty *list)
{
	assert(NULL != list);
	
	while (!SListIsEmpty(list))
	{
		SListPopFront(list);	
	}
	
	free(list -> tail);
	free(list);
	list = NULL;
}

status_ty SListPushFront(slist_ty *list, void *element)
{
	slist_node_ty *new_node = NULL;
	
	assert(NULL != list);
	assert(NULL != element);
	
	new_node = (slist_node_ty *) malloc(sizeof(slist_node_ty));
	if (NULL == new_node)
	{
		fprintf(stderr, "\nThe memory allocation for the new node failed.\n");
		
		return MEM_FAIL;
	}
	
	new_node -> data = element;
	new_node -> next = list -> head;
	list -> head = new_node;

	return SUCCESS;
}

status_ty SListPushBack(slist_ty *list, void *element)
{
	slist_node_ty *new_dummy = NULL;
	
	assert(NULL != list);
	assert(NULL != element);
	
	new_dummy = (slist_node_ty *) malloc(sizeof(slist_node_ty));
	if (NULL == new_dummy)
	{
		fprintf(stderr, "\nThe memory allocation for the new dummy failed.\n");
		
		return MEM_FAIL;
	}
	
	new_dummy -> data = NULL;
	new_dummy -> next = NULL;
	
	list -> tail -> data = element;      
	list -> tail -> next = new_dummy;
	
	list -> tail = new_dummy;

	return SUCCESS;
}

status_ty SListPopBack(slist_ty *list)        
{
	slist_node_ty *runner = NULL;
	
	assert(NULL != list);
	
	if (SListIsEmpty(list))
    {
        return SUCCESS;
    }
	
	runner = list -> head;
	while (runner -> next != list -> tail)
	{
		runner = runner -> next;
	}
	
	/*free(runner -> data);*/
	runner -> data = NULL;
	free(runner -> next);
	runner -> next = NULL;
	list -> tail = runner;
	
	return SUCCESS;
}

status_ty SListForEach(slist_ty *list, slist_node_ty *from, slist_node_ty *to,          
                      status_ty (*action)(void *data, void *param), void *param)
{
	status_ty status = SUCCESS;
		
	assert(NULL != list);
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != action);
	assert(NULL != param);
		
	while (from != to && SUCCESS == status)
	{
		status = action(from -> data, param);
		from = from -> next;
	}
		
	return status;
}

status_ty CountLengthAction(void *data, void *param)
{
	assert(NULL != data);
	assert(NULL != param);
	
	++(*(size_t *) param);
	
	return SUCCESS;
}

size_t SListLength(const slist_ty *list)
{
	size_t length = 0;

	if (NULL == list)
	{
		return length;
	}
	
	SListForEach((slist_ty*)list, list -> head, list -> tail, CountLengthAction, 
	                                                          (void *) &length);
	                                                           
	return length;
}

slist_node_ty *SListFind(const slist_ty *list, 
  status_ty (*equals)(const void *left, const void *right), const void *element)
{
	slist_node_ty *runner = NULL;

	assert(NULL != list);
	assert(NULL != equals);
	assert(NULL != element);

	runner = list -> head;
	while (runner -> next != list -> tail)
	{
		if (EQUAL == equals(runner -> data, element))
		{
			return runner;
		}
		
		runner = runner -> next;
	}
	
	return NULL;
}

slist_node_ty *SListBegin(const slist_ty *list)
{
	assert(NULL != list);
	
	return list -> head;
}

slist_node_ty *SListEnd(const slist_ty *list)
{
	assert(NULL != list);
	
	return list -> tail;
}

slist_node_ty *SListNext(const slist_node_ty *slist_node)
{
	assert(NULL != slist_node);
	
	return slist_node -> next;
}

void *SListGetData(const slist_node_ty *slist_node)
{
	assert(NULL != slist_node);
	
	return slist_node -> data;
}

void SListSetData(slist_node_ty *slist_node, void *data)
{
	assert(NULL != slist_node);
	assert(NULL != data);
	
	/*free(slist_node -> data);*/
	slist_node -> data = data;
}

slist_node_ty *SListInsertAfter(slist_ty *list, slist_node_ty *where, void *data)
{
	slist_node_ty *new_node = NULL;

	assert(NULL != list);
	assert(NULL != where);
	assert(NULL != data);
	
	new_node = (slist_node_ty *) malloc(sizeof(slist_node_ty));
	if (NULL == new_node)
	{
		fprintf(stderr, "\nThe memory allocation for the new node failed.\n");
		
		return NULL;
	}
	
	new_node -> data = data;
	new_node -> next = where -> next;
	where -> next = new_node;
	
	return new_node;
}

slist_node_ty *SListInsert(slist_ty *list, slist_node_ty *where, void *data)
{
	void *where_data = NULL;
	
	assert(NULL != list);
	assert(NULL != where);
	assert(NULL != data);
	
	where_data = where -> data;
	where -> data = data;

	return SListInsertAfter(list, where, where_data);
}	

slist_node_ty *SListRemove(slist_ty *list, slist_node_ty *to_remove)
{
	slist_node_ty *next_ptr = NULL;
	
	assert(NULL != list);
	assert(NULL != to_remove);
	
	if (to_remove -> next == list -> tail)
    {
        SListPopBack(list);
    }
    else if (to_remove == list -> head)
    {
        SListPopFront(list);
    }
    else
    {
    	/*free(to_remove -> data);*/
		to_remove -> data = to_remove -> next -> data;
		next_ptr = to_remove -> next -> next;
		free(to_remove -> next);
		to_remove -> next = next_ptr;
	}
	
	return to_remove;
}	

slist_node_ty *SListRemoveAfter(slist_ty *list, slist_node_ty *to_remove)
{
	slist_node_ty *remove_next = NULL;
	
	assert(NULL != list);
	assert(NULL != to_remove);
	
	if (to_remove -> next == list -> tail)
    {
        return NULL;
    }
    
	remove_next = to_remove -> next -> next;
	/*free(to_remove -> next -> data);*/
	free(to_remove -> next);
	to_remove -> next = remove_next;
	
	return to_remove;
}

void SListAppend(slist_ty *dest, slist_ty *src)
{
	assert(NULL != dest);
	assert(NULL != src);
	
	dest -> tail -> data = src -> head -> data;
	dest -> tail -> next = src -> head -> next;
	
	src -> head -> data = NULL;
	src -> head -> next = NULL;
	
	dest -> tail = src -> tail;
	src -> tail = src -> head;
}
