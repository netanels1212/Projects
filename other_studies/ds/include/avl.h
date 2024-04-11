/********************************************************************************
Project: AVL
Name: Netanel Shmuel
Reviewer: Yan
Date: 5.11.2023
Version: 1.0
********************************************************************************/

#ifndef __AVL_H__
#define __AVL_H__

#include <stddef.h> /* size_t */

enum
{
	AVL_NO_ERROR = 0,
       MEM_ERROR,
	AVL_GENERAL_ERROR
};

typedef struct avl_tree avl_ty;



/*
description: Check if data1 is before data2
return value: 1 true, 0 false
note: params can be NULL value
*/
typedef int(*is_before_func_ty)(const void *data1_, const void *data2_, 
                                                            const void *params_);

/*
undefined behaviour: is_before_ is NULL pointer
notes: if the function failed - return NULL
       params_ can be NULL
time complexity: O(1)
*/
avl_ty *AvlCreate(is_before_func_ty is_before_, const void *params_);


/*
notes: allowed NULL pointer parameter
time complexity: O(nlogn)
*/
void AvlDestroy(avl_ty *avl_);


/*
return value: return AVL_NO_ERROR (0) if success, otherwise return AVL_GENERAL_ERROR (non-zero)
time complexity: O(log n)
IMPORTANT: do not insert existing data
*/
int AvlInsert(avl_ty *avl_, void *data_);


/*
return value: if we delete the biggest value node - return NULL
notes: call to those function invalidates all existing pointers to nodes
time complexity: O(log n)
*/
void *AvlRemove(avl_ty *avl_, const void *key_);


/*
time complexity: O(log n)
*/
size_t AvlHeight(const avl_ty *avl_);


/*
time complexity: O(n)
*/ 
size_t AvlSize(const avl_ty *avl_);


/*
time complexity: O(1)
*/
int AvlIsEmpty(const avl_ty *avl_);


/*
return value: if the data not found - return NULL
time complexity: O(log n)
IMPORTANT: don't change the fields that influence order, the object is still
           in the set
*/
void *AvlFind(avl_ty *avl_, const void *key_);



typedef enum 
{
       PRE_ORDER,
       IN_ORDER,
       POST_ORDER
} traversal_ty;

typedef int(*op_func_ty)(void *data_, void *func_param_);

/*
undefined behaviour: avl_, data_ NULL pointers
                     invalid traverse_mode_
notes: func_param_ can be NULL,
       oper_ returns zero for success and non-zero for failure
time complexity: O(log n)
IMPORTANT: with oper_ don't change the fields that influence order,
           the object is still in the set
*/
int AvlForEach(avl_ty *avl_, traversal_ty traverse_mode_,
                                            op_func_ty oper_, void *func_param_);

#endif /* __AVL_H__ */