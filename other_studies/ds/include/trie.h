/********************************************************************************
Project: Binary Trie
Name: Netanel Shmuel
Reviewer: Jonathan
Date: 14.12.2023
Version: 1.0
********************************************************************************/

#ifndef __BINARY_TRIE__
#define __BINARY_TRIE__

#include <stddef.h> /* size_t */

enum
{
	TRIE_NO_ERROR = 0,
	TRIE_ERROR
};

typedef struct trie trie_ty;


/*
undefined behaviour: depth_ equal to 0 or not in range of unsigned long
return value: on failure - return NULL
time complexity: O(1)
*/
trie_ty *TrieCreate(size_t depth_);


/*
notes: trie_ can be NULL pointer
time complexity: O(size)
*/
void TrieDestroy(trie_ty *trie_);


/*
undefined behaviour: trie_ NULL pointer
                     wanted_num_ is not with good prefix form
return value:  if found - return wanted_num_,
               if there is other number close to it - return those number
               if not found any number or memory failure - return 0
time complexity: O(depth_)
*/
unsigned long TrieGet(trie_ty *trie_, unsigned long wanted_num_);


/*
undefined behaviour: trie_ NULL pointer
                     wanted_num_ is not with good prefix form
time complexity: O(depth_)
*/
void TrieFree(trie_ty *trie_, unsigned long num_);


/*
undefined behaviour: trie_ NULL pointer
time complexity: O(size)
*/
size_t TrieCountAvailable(const trie_ty *trie_);

/*
description: check if all numbers in the trie was taken
undefined behaviour: trie_ NULL pointer
time complexity: O(1)
*/
int TrieIsAllTaken(const trie_ty *trie_);


#endif /* __BINARY_TRIE__ */