#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, calloc, free */
#include <stdio.h> /* fprintf */

#include "hash.h"
#include "doublylinkedlist.h"
#include "utils.h"

struct hash_table                                     
{
	size_t capacity;
	dlist_ty **lists;      
	hash_func_ty hash_func;
	const void *hash_params;
	is_same_key_ty is_same;
	const void *is_same_params;
};

struct hash_node
{
	const void *key;
	void *data;
};

struct hsah_stat
{
	size_t capacity;
	size_t size;
	size_t longest_list;
	size_t median_list;
};

struct find_imp_info
{
    const void *key_to_find;
    is_same_key_ty is_same;
    const void *is_same_params;
    pairs_ty *output;
};



/* IMPLEMENTETIONS TODO'S:
1. for move on the list - use foreah
2. use the functions we create in dlist and do not wrote uneccessery code (implement FindHealperIMP)
5. use good variables names
 */


static void CorruptHashIMP(hash_ty *hash_)
{
    /* assert */
    assert(NULL != hash_);

    /* corrupt all fields */
    DEBUG_ONLY(hash_->lists = BAD_MEM(dlist_ty**);)
    DEBUG_ONLY(hash_->hash_params = BAD_MEM(const void);)
    DEBUG_ONLY(hash_->is_same_params = BAD_MEM(const void);)
}

hash_ty *HashCreate(size_t capacity_, hash_func_ty hash_func_, const void *hash_params_,
                                      is_same_key_ty is_same_, const void *is_same_params_)
{
	hash_ty *ret = NULL;
	size_t i = 0;

	/* assert */
	assert(NULL != hash_func_);
	assert(NULL != is_same_);

	/* allocate memory for ret */
	ret = (hash_ty *)malloc(sizeof(hash_ty));

	/* if allocation fail */
	if (NULL == ret)
	{
		DEBUG_ONLY(fprintf(stderr, "\nMemory allocation for hash_ty failed.\n");)

		/* return NULL */
		return NULL;
	}

	/* allocte memory for the lists according to capacity_ with calloc */
	ret->lists = (dlist_ty**)calloc(sizeof(dlist_ty*), capacity_);

	/* if allocation fail */
	if (NULL == ret->lists)
	{
		DEBUG_ONLY(fprintf(stderr, "\nMemory allocation for lists failed.\n");)

		/* corrupt and free ret */
        CorruptHashIMP(ret);
		free(ret);

		/* return NULL */
		return NULL;
	}

	/* run over the lists */
	for (i = 0; i < capacity_; ++i)
	{
		/* create a list in all places */
		ret->lists[i] = DListCreate();

		/* if allocation fail */
		if (NULL == ret->lists[i])            
		{
			DEBUG_ONLY(fprintf(stderr, "\nMemory allocation for dlist failed.\n");)

			/* destroy ret */
			HashDestroy(ret);

			/* return NULL */
			return NULL;
		}
	}

	/* initialize the fields of ret */
	ret->capacity = capacity_;      
	ret->hash_func = hash_func_;
	ret->hash_params = hash_params_;
	ret->is_same = is_same_;
	ret->is_same_params = is_same_params_;

	/* return ret */
	return ret;
}

static int FreePairsIMP(pairs_ty *pair_, void *params_)
{
    /* assert */
    assert(NULL != pair_);

    (void)params_;

    /* corrupt pair_ */
    DEBUG_ONLY(pair_->key = BAD_MEM(const void);)
    DEBUG_ONLY(pair_->data = BAD_MEM(void));

    /* free pair_ */
    free(pair_);

    return HASH_NO_ERROR;
}

void HashDestroy(hash_ty *hash_)
{
	size_t i = 0;

    if (NULL == hash_)
    {
        return;
    }

	/* run over the hash_ */
	for (i = 0; i < hash_->capacity; ++i)
	{
		/* for each element in the current list - corrupt and free the element data */
        DListForEach(hash_->lists[i], DListBegin(hash_->lists[i]), DListEnd(hash_->lists[i]), FreePairsIMP, NULL);
			
		/* destroy the list */
        DListDestroy(hash_->lists[i]);
	}

	/* free hash_->lists */
    free(hash_->lists);

	/* corrupt and free hash_  */
    CorruptHashIMP(hash_);
    free(hash_);
}

static int ToIdxIMP(hash_ty *hash_, const void *key_)
{
    /* assert */
    assert(NULL != hash_);
    assert(NULL != key_);

    /* return the correct hash value according to functions parameters */
    return hash_->hash_func_(key_) % hash_->capacity;
}

static int FindHelperIMP(pairs_ty *pair_, struct find_imp_info *params_)
{
    /* assert */
    assert(NULL != pair_);

    /* if the keys is_same */
    if (1 == params_->is_same(pair_->key_, params_->key_to_find, params_->is_same_params))
    {
        /* save current pairs information in params_->ret */
        params_->output = pair_;

        /* return error */
        return HASH_ERROR;
    }

    /* return success */
    return HASH_NO_ERROR;
}

pairs_ty HashRemove(hash_ty *hash_, const void *key_)
{
    pairs_ty ret = {NULL, NULL};
    pairs_ty invalid_pair = {NULL, NULL};
    struct find_imp_info params = {key_, hash_->is_same, hash_->is_same_params, &invalid_pair};
    int hash_value = 0;

	/* assert */
    assert(NULL != hash_);
    assert(NULL != key);

	/* find the correct hash value using ToIdxIMP() */
    hash_value = ToIdxIMP(hash_, key_);

	/* for each element of the list in this hash value - operate FindHelperIMP() */
    DListForEach(hash_->lists[hash_value], DListBegin(hash_->lists[hash_value]), 
                        DListEnd(hash_->lists[hash_value]), FindHelperIMP, &params);

    /* copy the information from output to ret */
    ret.key = params.output->key;
    ret.data = params.output->data;    

    /* remove this node from the list */
    DListRemove(hash_->lists[hash_value], params->output);

	/* return ret */
    return ret;
}

int HashInsert(hash_ty *hash_, const void *key_, const void *data_)
{
    pairs_ty *new_pair = NULL;
    dlist_status_ty push_status = NO_ERROR;
    int hash_value = 0;

	/* assert */
    assert(NULL != hash_);
    assert(NULL != key_);
    assert(NULL != data_);

	/* create new_pair with key_ and data_ information */
    new_pair = (pairs_ty*)malloc(sizeof(pairs_ty));

	/* NULL failure handler */
    if (NULL == new_pair)
    {
        DEBUG_ONLY(fprintf(stderr, "\nMemory allocation for new_pair failed.\n");)

        return HASH_ERROR;
    }

    /* set the new_pair with the information from the function parameters */
    new_pair->key = key_;
    new_pair->data = data_;

	/* find the correct hash value using ToIdxIMP() */
    hash_value = ToIdxIMP(hash_, key_);

	/* add the node to the correct list with PushBack */
    push_status = DListPushBack(hash_->lists[hash_value], new_pair);

    /* push failure handler */
    if (NO_ERROR != push_status)
    {
        DEBUG_ONLY(fprintf(stderr, "\nThe push of the new_pair to the list failed.\n");)
      
        return HASH_ERROR;
    }

	/* return success */
    return HASH_NO_ERROR;
}

size_t HashSize(hash_ty *hash_)
{
    size_t ret = 0;
    size_t i = 0;

	/* assert */
    assert(NULL != hash_);

	/* run over the hash_ */
    for (i = 0; i < hash_->capacity_; ++i)
    {
		/* add to ret the size of each list */
        ret += DListLength(hash_->lists[i]);
    }

	/* return ret */
    return ret;
}

int HashIsEmpty(hash_ty *hash_)
{
    size_t i = 0;

	/* assert */
    assert(NULL != hash_);

	/* run over the hash_ */
    for (i = 0; i < hash_->capacity_; ++i)
    {
		/* if current list isn't empty */
        if (!DListIsEmpty(hash_->lists[i]))
        {
			/* return false */
            return 0;
        }
    }

	/* return true */
    return 1;
}

pairs_ty HashFind(hash_ty *hash_, const void *key_)
{
    pairs_ty ret = {NULL, NULL};
    pairs_ty invalid_pair = {NULL, NULL};
    struct find_imp_info params = {key_, hash_->is_same, hash_->is_same_params, &invalid_pair};
    int hash_value = 0;

	/* assert */
    assert(NULL != hash_);
    assert(NULL != key_);

    /* find the correct hash value using ToIdxIMP() */
    hash_value = ToIdxIMP(hash_, key_);

    /* for each element of the list in this hash value - operate FindHelperIMP() */
    DListForEach(hash_->lists[hash_value], DListBegin(hash_->lists[hash_value]), 
                        DListEnd(hash_->lists[hash_value]), FindHelperIMP, &params);

    /* copy the information from output to ret */
    ret.key = params.output->key;
    ret.data = params.output->data;


	/* return ret */
    return ret;	
}

int HashForEach(hash_ty *hash_, op_func_ty oper_, void *func_param_)
{
    dlist_status_ty dlist_foreach_status = NO_ERROR;
    size_t i = 0;

	/* assert */
    assert(NULL != hash_);
    assert(NULL != oper_);

	/* run over the hash_ */
    for (i = 0; i < hash_->capacity_; ++i)
    {
		/* for each element in the current list - execute oper_ on him */
        dlist_foreach_status = DListForEach(hash_->lists[i], DListBegin(hash_->lists[i]), DListEnd(hash_->lists[i]), oper_, func_param_);

		/* if oper_ failed */
        if (NO_ERROR != list_foreach_status)
        {
			/* return failure */
            return HASH_ERROR;
        }
    }

	/* return success */
    return HASH_NO_ERROR;
}

static void InsertionSort(size_t *array, size_t len)
{
    size_t key = 0;
    size_t i = 0;
    size_t j = 0;

    assert(NULL != array);
    assert(0 < len);

    for (i = 1; i < len; ++i)
    {
        key = array[i];

        j = i - 1; 

        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            
            --j;
        }

        array[j + 1] = key;
    }
}

hash_stats_ty HashStats(const hash_ty *hash_)
{
    hash_stats_ty stat = {0, 0, 0, 0};
    size_t *lists_sizes = NULL;
    size_t healp_arr_size = 0;
    size_t curr_list_size = 0;
    size_t longest_list_size = 0;
    size_t i = 0;

	/* assert */
    assert(NULL != hash_);

	/* initialize the size and the capacity */
    stat.capacity = hash_->capacity;
    stat.size = HashSize(hash_);

	/* create healp array in size of capacity */
    lists_sizes = (size_t*)malloc(capacity * sizeof(size_t));

	/* run over the hash_ */
    for (i = 0; i < hash_->capacity_; ++i)
    {
		/* if current list isn't empty */
        if (!DListIsEmpty(hash_->lists[i]))
        {
			/* ++arr_size */
            ++healp_arr_size;

            curr_list_size = DListLength(hash_->lists[i]);

			/* if her size grater then longest_list_size */
            if (curr_list_size > longest_list_size)
            {
				/* longest_list_size = current list size */
                longest_list_size = curr_list_size;
            }

			/* insert the list size to the healp array */
            lists_sizes[healp_arr_size] = curr_list_size;
        }
    }

	/* sort the healp array */
    InsertionSort(lists_sizes, healp_arr_size);

	/* stat.longest_list = longest_list_size */
    stat.longest_list = longest_list_size;

	/* stat.median_list = healp_array[arr_size/2] */
    stat.median_list = lists_sizes[healp_arr_size/2];

    /* free healp_array */
    free(lists_sizes);

	/* return hash_stats */
    return stat;
}