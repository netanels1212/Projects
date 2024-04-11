/********************************************************************************
Project: Hash Table
Name: Netanel Shmuel
Reviewer: Asem 
Date: 10.12.2023
Version: 1.0
********************************************************************************/

#ifndef __HASH_H__
#define __HASH_H__

#include <stddef.h> /* size_t */

enum
{
	HASH_NO_ERROR = 0,
	HASH_ERROR
};

typedef struct hash_table hash_ty;
typedef struct hash_stat hash_stats_ty;
typedef struct hash_node pairs_ty;


/*
return value: the appropriate hash value according to the key_
undefined behaviour: key_ is NULL pointer
notes: hash_params_ can be NULL.
       for HashCreate().
*/
typedef int (*hash_func_ty)(const void *key_, const void *hash_params_);

/*
return value: 1 (true) if the keys are the same, otherwise 0 (false)
undefined behaviour: key1_, key2_ is NULL pointer
notes: is_same_params_ can be NULL.
       for HashCreate().
*/
typedef int (*is_same_key_ty)(const void *key1_, const void *key2_, const void *is_same_params_);






/*
undefined behaviour: hash_func_, is_same_ is NULL pointer
notes: it's good for the capacity to be larger then number of elements to insert, and prime number.
       and the hash_func_ output will be in the range of capacity.
       hash_params_, is_same_params_ can be NULL.
time complexity: O(1)
*/
hash_ty *HashCreate(size_t capacity_, hash_func_ty hash_func_, const void *hash_params_,
                                      is_same_key_ty is_same_, const void *is_same_params_);


/*
notes: allowed NULL pointer parameter
time complexity: O(size + capacity)
*/
void HashDestroy(hash_ty *hash_);


/*
undefined behaviour: hash_, key_ is NULL pointer
return value: if key not found - key and data in pair will be NULL
time complexity: O(size), avg - O(1)
*/
pairs_ty HashRemove(hash_ty *hash_, const void *key_);


/*
undefined behaviour: hash_, key_, data_ is NULL pointer
return values: HASH_NO_ERROR (0) on SUCCESS, HASH_ERROR (non-zero) on failure
time complexity: O(1)
*/
int HashInsert(hash_ty *hash_, const void *key_, const void *data_);


/*
undefined behaviour: hash_ is NULL pointer
return value: return number of elements in the hash table
time complexity: O(size + capacity)
*/
size_t HashSize(hash_ty *hash_);


/*
undefined behaviour: hash_ is NULL pointer
time complexity: O(capacity)
*/
int HashIsEmpty(hash_ty *hash_);


/*
undefined behaviour: hash_, key_ is NULL pointer
return value: if key not found - key and data in pair will be NULL
time complexity: O(size)
*/
pairs_ty HashFind(hash_ty *hash_, const void *key_);


/*
undefined behaviour: key_ is NULL pointer
return values: 0 on SUCCESS, non-zero for failure
notes: func_param_ can be NULL.
       for HashForEach().
*/
typedef int (*op_func_ty)(void *key_, void *func_param_);


/*
undefined behaviour: hash_, oper_ is NULL pointer
return values: 0 on SUCCESS, non-zero for failure
notes: func_param_ can be NULL
IMPORTANT: don't change the key!
time complexity: O(size + capacity)
*/
int HashForEach(hash_ty *hash_, op_func_ty oper_, void *func_param_);


/* 
description: return statistics information about the hash table
undefined behaviour: hash_ is NULL pointer
time complexity: O(size + capacity)
*/
hash_stats_ty HashStats(const hash_ty *hash_);

#endif /* __HASH_H__ */