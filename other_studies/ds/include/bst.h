/*******************************************************************************
Project: Binary Tree
Name: Netanel Shmuel
Reviewer:  
Date: 27/11/2023
Version: 1.0
*******************************************************************************/
#ifndef __BST_H__
#define __BST_H__

#include <stddef.h> /* size_t */

typdef struct
{
	bst_node_ty *where;	
	bst_ty *bst; /* my TODO: add to utils DEBUG_ONLY() */
} bst_iter_ty;

typedef enum
{
	BST_NO_ERROR = 0,
	BST_ERROR = 1
} status_ty;

typedef struct bst_node bst_node_ty;
typedef struct bst bst_ty;

typedef int(*is_before_func_ty)(const void *data1_, const void *data2_, 
	                                                        const void *params_);
typedef int(*op_func_ty)(void *data_, void *func_param_);

/*
Undefined behaviour: is_before_func_ are NULL pointer.
notes: if the function failed - return NULL.
time complexity: O(1)
*/
bst_ty *BtsCreate(is_before_func_ty is_before_func_, const void *params_);

/*
notes: allowed NULL pointer
time complexity: O(n)
*/
void BstDestroy(bst_ty *bst_);

/*
time complexity: O(n)
*/
size_t BstSize(const bst_ty *bst_);

/*
return value: 1 if is empty, otherwise - 0
time complexity: O(1)
*/
int BstIsEmpty(const bst_ty *bst_);

/*
return value: return BST_NO_ERROR (0) if success, otherwise return BST_ERROR (1)
time complexity: O(n)
*/
status_ty BstInsert(bst_ty *bst_, const void *data_);

/*
return value: if we delete the biggest value node - return iter to invalid node
notes: call to those function invalidates all existing iterators
time complexity: O(n)
*/
bst_iter_ty BstRemove(bst_ty *bst_, bst_iter_ty_ to_remove_);

/*
return value: return the smallest value node
time complexity: O(1)
*/
bst_iter_ty BstBegin(bst_ty *bst_);

/*
return value: return iter to invalid node
time complexity: O(1)
*/
bst_iter_ty BstEnd(bst_ty *bst_);

/*
return value: if the current iter is the smallest value node - 
              return iter to invalid node
time complexity: O(1)
*/
bst_iter_ty BstPrev(bst_iter_ty bst_iter_);

/*
return value: if the current iter is the biggest value node -
		      return iter to invalid node

time complexity: O(1)
*/
bst_iter_ty BstNext(bst_iter_ty bst_iter_);

/*
time complexity: O(1)
*/
status_ty BstIsSameIter(bst_ty *bst_, bst_iter_ty bst_first_iter_,
                                                   bst_iter_ty bst_secend_iter_);
/*
time complexity: O(n)
*/
void *BstGetData(bst_iter_ty bst_iter_);

/*
return value: if the data not found - return iter to invalid node
time complexity: O(n)
*/
bst_iter_ty BstFind(bst_ty *bst_, const void *data_);

/*
notes: func_param_ can be NULL
time complexity: O(n)
*/
status_ty BstForEach(bst_iter_ty from_, bst_iter_ty to_,
				                         op_func_ty op_func_, void *func_param_);


#endif /*__BST_H__*/
