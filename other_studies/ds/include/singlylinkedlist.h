/*******************************************************************************
Project: Singly Linked List 
Name: Netanel Shmuel
Reviewer: Yan
Date: 15/10/23
Version: 1.0 Initial
*******************************************************************************/

#ifndef __SINGLY_LINKED_LIST_H__
#define __SINGLY_LINKED_LIST_H__

#include <stddef.h> /*size_t*/

enum return_values_ty {SUCCESS, FAIL, MEM_FAIL};

typedef struct slist_node slist_node_ty;
typedef struct linkedlist slist_ty;
typedef int status_ty;

/*
Function Description: Create a list with a size of element 
                                                    that was stated by the user.

Return value: Pointer to the list

Notes:	If memory allocation for the list fails, returns NULL. 

Time Complexity: O(1)
*/
slist_ty *SListCreate(void);                                              

/*
Function Description: Check if list is empty

Return value: 1 - empty, 0 - not empty

Time Complexity: O(1)
*/
int SListIsEmpty(const slist_ty *list);

/*
Function Description: Removes the first element of the list

Return value: SUCCESS if the pop succeed

Notes: Undefined behaviour if pointer is NULL

Time Complexity: O(1)
*/
status_ty SListPopFront(slist_ty *list);

/*
Function Description: Destroy the list and free its resources.

Return value: void

Notes:	1) If 'list' is NULL, the function has no effect.
		2) After calling this function, the 'list' pointer becomes invalid and 
  		   should not be used.

Time Complexity: O(n)
*/
void SListDestroy(slist_ty *list);

/*
Function Description: Inserts element to the start of the list

Return value: Status - SUCCESS (0), if the allocation within failed return
                       MEM_FAIL (2). 

Notes:	1) If memory allocatin for the node failed, returns Memory Fail.  
		2) Undefined behaviour if pointer is NULL or element is of wrong type.

Time Complexity: O(1)
*/
status_ty SListPushFront(slist_ty *list, void *element);

/*
Function Description: Inserts element to the end of the list

Return value: Status - SUCCESS (0), if the allocation within failed return
                       MEM_FAIL (2).

Notes:	1) If memory allocatin for the node failed, returns Memory Fail. 
		2) Undefined behaviour if pointer is NULL or element is of wrong type.

Time Complexity: O(1)
*/
status_ty SListPushBack(slist_ty *list, void *element);

/*
Function Description: Removes the last element of the list

Return value: SUCCESS if the pop succeed

Notes:	Undefined behaviour if pointer is NULL

Time Complexity: O(n)
*/
status_ty SListPopBack(slist_ty *list);

/*
Function Description: Apply a given action to a range of nodes in the list

Return value: SUCCESS (0) if all actions are successful,
              FAIL (1) if any action fails

Notes: Undefined behaviour when 'list', 'from', 'to', or 'Action' is NULL.

Time Complexity: O(n)
*/
status_ty SListForEach(slist_ty *list, slist_node_ty *from, slist_node_ty *to,
                 status_ty (*action)(void *data, void *param), void *param);
     
/*
Function Description: Healper function for SListLength().
                     Increase the length counter.

Return value: SUCCESS value.

Time Complexity: O(1)
*/    
status_ty CountLengthAction(void *data, void *param);                               
                 
/*
Function Description: Count the number of elements in the list

Return value: Number of elements

Notes:	Undefined behaviour 'list' is NULL.

Time Complexity: O(n)
*/
size_t SListLength(const slist_ty *list);

/*
Function Description: Find first element that is equal to element

Return value: Pointer to node

Notes:	1) IUndefined behaviour when 'list' or 'equals' ptr are NULL.
		2) If element not found return NULL;

Time Complexity: O(n)
*/
slist_node_ty *SListFind(const slist_ty *list, 
 status_ty (*equals)(const void *left, const void *right), const void *element);

/*
Function Description: Head to list

Return value: Pointer to head

Notes:	Undefined behaviour when 'list' is NULL.

Time Complexity: O(1)
*/
slist_node_ty *SListBegin(const slist_ty *list);

/*
Function Description: Find last node

Return value: Pointer to last

Notes:	Undefined behaviour when 'list' is NULL.

Time Complexity: O(1)
*/
slist_node_ty *SListEnd(const slist_ty *list);

/*
Function Description: Find next node

Return value: Pointer to next node

Notes:	Undefined behaviour when'slist_node' is NULL.

Time Complexity: O(1)
*/
slist_node_ty *SListNext(const slist_node_ty *slist_node);

/*
Function Description: Get data from a specific node in the list

Return value: Pointer to the data stored in the node

Notes:	Undefined behaviour when 'slist_node' is NULL.

Time Complexity: O(1)
*/
void *SListGetData(const slist_node_ty *slist_node);

/*
Function Description: Set data in a specific node in the list

Return value: void

Notes:	Undefined behaviour when 'slist_node' is NULL,
        and the function has no effect.

Time Complexity: O(1)
*/
void SListSetData(slist_node_ty *slist_node, void *data);

/*
Function Description: Insert a new node with the specified data 
                                                  after a given node in the list

Return value: Pointer to the updated list

Notes:	Undefined behaviour when 'list' or 'where' is NULL,
        and the function has no effect.

Time Complexity: O(1)
*/
slist_node_ty *SListInsertAfter(slist_ty *list,slist_node_ty *where,void *data);

/*
Function Description: Insert a new node with the specified data
                                                 before a given node in the list

Return value: Pointer to the updated list

Notes:	Undefined behaviour when 'list' or 'where' is NULL,
        and the function has no effect.

Time Complexity: O(1)
*/
slist_node_ty *SListInsert(slist_ty *list, slist_node_ty *where, void *data);

/*
Function Description: Remove a specific node from the list

Return value: Pointer to the updated list

Notes:	Undefined behaviour when 'list' or 'to_remove' is NULL, 
        and the function has no effect.

Time Complexity: O(1)
*/
slist_node_ty *SListRemove(slist_ty *list, slist_node_ty *to_remove);

/*
Function Description: Remove the node after a specified node in the list

Return value: Pointer to the updated list

Notes:	Undefined behaviour when 'list' or 'to_remove' is NULL, 
        and the function has no effect.

Time Complexity: O(1)
*/
slist_node_ty *SListRemoveAfter(slist_ty *list, slist_node_ty *to_remove);

/*
	Function: SListAppend
	return value: New concatenated list
	on success: Concatenates dest with src (dest as start), after thet src point
	            only to dmmy node
	undefined behaviour: null pointer
	Time Complexity: O(n)
*/
void SListAppend(slist_ty *dest, slist_ty *src);

#endif /*__SINGLY_LINKED_LIST_H__*/
