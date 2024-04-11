/*******************************************************************************
Project: Sorted Linked List 
Name: Netanel Shmuel
Reviewer: Najeeb 
Date: 31/10/23 
Version: 1.0
*******************************************************************************/
#ifndef __SORTED_LINKED_LIST_H__
#define __SORTED_LINKED_LIST_H__

#include <stddef.h> /*size_t*/

#include "doublylinkedlist.h"

enum {IS_CMP_FUNC_EQUAL = 0, IS_CMP_FUNC_NOT_EQUAL = 1};

typedef enum {SORTED_NO_ERROR = 0, 
              SORTED_MALLOC_ERROR = 1,
              SORTED_GENERAL_ERR = 2} sorted_list_status_ty;

typedef struct sorted_list sorted_list_ty; 
typedef struct sorted_iter sorted_list_iter_ty;

typedef int(*cmp_func_ty)(const void *data1, const void *data2);
typedef int(*op_func_ty)(void *data, void *func_param);

struct sorted_iter
{
  dlist_iter_ty internal_itr;
};

/*
Function Description: Create a list.

Return value: Pointer to the list

Undefined behaviour:cmp_func is NULL;

Time Complexity: O(1)

on failure: NULL.
*/
sorted_list_ty *SortedListCreate(cmp_func_ty cmp_func);


/*
Function Description: Destroy the list and free its resources.

Return value: void

Undefined behaviour: if pointer is NULL.

Time Complexity: O(n)

on failure: None.
*/
void SortedListDestroy(sorted_list_ty *sorted_list);


/*
Function Description: Removes the first element of the list

Return value: Status - 0 SUCCESS, 2 if empty

Undefined behaviour: if list is NULL

Time Complexity: O(1)

on failure: Error code.
*/
sorted_list_status_ty SortedListPopFront(sorted_list_ty *sorted_list);


/*
Function Description: Removes the last element of the list

Return value: Status - 0 SUCCESS, 2 if empty

Undefined behaviour: if list is NULL

Time Complexity: O(1)

on failure: Error code.
*/
sorted_list_status_ty SortedListPopBack(sorted_list_ty *sorted_list);


/*
Function Description: Count the number of elements in the list

Return value: Number of elements

Undefined behaviour: pointer is NULL;

Time Complexity: O(n)

on failure: None.
*/
size_t SortedListLength(const sorted_list_ty *sorted_list);


/*
Function Description: Check if list is empty

Return value: 1 - empty, 0 - not empty

Undefined behaviour: sorted_list is NULL;

Time Complexity: O(1)

on failure: None.

*/
int SortedListIsEmpty(const sorted_list_ty *sorted_list);


/*
Function Description: Head to list

Return value: head iterator, 
              if the list is empty return iterator to invalid element.

Undefined behaviour: if dlist_ty is NULL

Time Complexity: O(1)

on failure: None.
*/
sorted_list_iter_ty SortedListBegin(const sorted_list_ty *sorted_list);


/*
Function Description: Tail of the list

Return value: Tail iterator,
              if the list is empty return iterator to invalid element.

Undefined behaviour: if dlist_ty is NULL

Time Complexity: O(1)

on failure: None.
*/
sorted_list_iter_ty SortedListEnd(const sorted_list_ty *sorted_list);


/*
Function Description: the next iterator;

Return value: the next iterator,
              if iter is the end of the list return iterator to invalid element.

Undefined behaviour: if dlist_iter_ty holds null address

Time Complexity: O(1)

on failure: None.
*/
sorted_list_iter_ty SortedListNext(sorted_list_iter_ty iter);


/*
Function Description: Find prev iterator

Return value: Pointer to prev iterator,
            if iter is the begin of the list return iterator to invalid element.

Undefined behaviour: if dlist_iter_ty holds null address

Time Complexity: O(1)

on failure: None.
*/
sorted_list_iter_ty SortedListPrev(sorted_list_iter_ty iter);


/*
Function Description: Get data from a specific iterator in the list

Return value: Pointer to the data stored in the iterator

Undefined behaviour: if dlist_iter_ty holds null address

Time Complexity: O(1)

on failure: None.
*/
void *SortedListGetData(sorted_list_iter_ty iter);


/*
Function Description: Insert into sorted list 

Return value: sorted_list_iter_ty of the element that inserted.

Undefined behaviour: if 'list' ,'data' is NULL or if where holds null address

Time Complexity: O(n)

on failure: NULL.

*/
sorted_list_iter_ty SortedListInsert(sorted_list_ty *sorted_list, void *data);


/*
Function Description: Remove a specific iterator from the list

Return value: sorted_list_iter_ty

Undefined behaviour: if 'list' is NULL or 'to_remove' holds null address

Time Complexity: O(1)

on failure: NULL.

*/
sorted_list_iter_ty SortedListRemove(sorted_list_ty *sorted_list, 
                                                sorted_list_iter_ty to_remove);


/*
Function Description:  Find node that contains element element that 
                       is equal according to default compare function

Return value: sorted_list_iter_ty of the next element after to_remove.

Undefined behaviour: if list, compare function or element,
                     output are NULL pointers.
Note:The function expects the is_match function to return 1 in case of success

Time Complexity: O(n)

on failure: return the to iterator.
*/
sorted_list_iter_ty SortedListFind(const sorted_list_ty *sorted_list, 
          sorted_list_iter_ty from,sorted_list_iter_ty to ,const void *element);


/*
Function Description: Find node that contains element element that is equal
                      according to the provided compare function.

Return value: sorted_list_iter_ty

Undefined behaviour: if list, compare function or element,
                     output are NULL pointers.
Note:The function expects the is_match function to return 1 in case of success

Time Complexity: O(n)

on failure: return the to iterator.
*/
sorted_list_iter_ty SortedListFindIf(sorted_list_iter_ty from,
               sorted_list_iter_ty to ,cmp_func_ty cmp_func ,const void *param);


/*
Function Description: check if first_iter and second_iter is equal

Return value: true or false (1 or 0)

Undefined behaviour: if first_iter or second_iter is NULL.

Time Complexity: O(1);
*/
int SortedListIterIsEqual(sorted_list_iter_ty first_iter,
                                               sorted_list_iter_ty second_iter);
                                               
                                               
/*
Function Description: Apply a given action to a range of iterators in the list

Return value: same error codes as the ones returned by action

Undefined behaviour: 'from', 'to', or 'Action' is NULL.
Note: We expect the action to return 0 for success and non zero for failures.

Time Complexity: O(n)

on failure: Error code.
*/
int SortedListForEach(sorted_list_iter_ty from, sorted_list_iter_ty to,
                                          op_func_ty op_func, void *func_param);

/*
Function Description: merge two list src and dest into one -> dest 
                      src is empty after performing this function

Unspecified behavior: dest or src NULL

Return value: void

Time Complexity: O(n)

on failure: None.
*/
void SortedListMerge(sorted_list_ty *dest,sorted_list_ty *src);

#endif /*__SORTED_LINKED_LIST_H__*/
