#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* fprintf */

#include "avl.h"
#include "utils.h"

typedef struct avl_node
{
	void *data;
	struct avl_node *children[2];       /* TODO: add height ? */
} node_ty;

struct avl_tree
{
	node_ty dummy;
	is_before_func_ty is_before;
	const void *is_before_params;
};

enum
{
	LEFT = 0,
	RIGHT = 1
};


avl_ty *AvlCreate(is_before_func_ty is_before_, const void *params_)
{
	avl_ty *avl = NULL;

	/* assert */
	assert(NULL != is_before_);

	/* allocate memory for avl */
	avl = (avl_ty *)malloc(sizeof(avl_ty));

	/* if allocation failed */
	if (NULL == avl) /* TODO: use RET_IF_BED */
	{
	   DEBUG_ONLY(fprintf(stderr, "\nMemory allocation for the avl failed\n");)

	   /* return NULL */
	   return NULL;
	}

	/* initialize avl->dummy fields */
	DEBUG_ONLY(avl->dummy.data = BAD_MEM(avl->dummy.data));
	DEBUG_ONLY(avl->dummy.children[RIGHT] = BAD_MEM(avl->dummy.children[RIGHT]));
	avl->dummy.children[LEFT] = NULL;

	/* initialize other avl fields */
	avl->is_before = is_before_;
	avl->is_before_params = params_;

	/* return avl */
	return avl;
}

void DestroyIMP(node_ty *node_)
{
	/* if node_ == NULL */
	if (NULL == node_)
	{
		/* empty return */
		return;
	}

	/* DestroyIMP(left child) */
	DestroyIMP(node_->children[LEFT]);

	/* DestroyIMP(right child) */
	DestroyIMP(node_->children[RIGHT]);

	/* free(node_) */
	DEBUG_ONLY(node_->children[LEFT] = BAD_MEM(node_ty)); /* TODO: do function that do all of this - corrupt node*/
	DEBUG_ONLY(node_->children[RIGHT] = BAD_MEM(node_ty));
	DEBUG_ONLY(node_->data = BAD_MEM(void));

	free(node_);
}

void AvlDestroy(avl_ty *avl_) 
{
	/* DestroyIMP(root) */
	DestroyIMP(avl_->dummy.children[LEFT]);

	/* free avl_ */
	DEBUG_ONLY(avl_->dummy.children[LEFT] = BAD_MEM(node_ty));
	DEBUG_ONLY(avl_->is_before = BAD_MEM(is_before_func_ty));
	DEBUG_ONLY(avl_->is_before_params = BAD_MEM(void));

	free(avl_);
}

static void InsertIMP(avl_ty *avl_, node_ty **where_, node_ty *new_node_)
{
	int side = 0;

	/* assert */
	assert(NULL != avl_);
	assert(NULL != where_);
	assert(NULL != new_node_);

	/* if *where_ is NULL */
	if (NULL == *where_)
	{
		/* connect here */
		*where_ = new_node_;

		return;
	}

	/* side = the side we should go to (user func is_before desides) */
	side = avl_->is_before((*where_)->data, new_node_->data, avl_->is_before_params);

	/* InsertIMP(&(*where)->children[side]) */
	InsertIMP(avl_, &((*where_)->children[side]), new_node_);

	/* if out of balance */
	    /* rebalance this level */
}

static void InitNewNodeIMP(node_ty *node_, void *data_)
{
	assert(NULL != node_);

	node_->data = data_;
	node_->children[LEFT] = NULL;
	node_->children[RIGHT] = NULL;
}

int AvlInsert(avl_ty *avl_, void *data_)
{
	node_ty *new_node = NULL;

	/* assert */
	assert(NULL != avl_);
	assert(NULL != data_);

	/* allocate memory for new_node */
	new_node = (node_ty *)malloc(sizeof(node_ty));

	/* NULL failure handler */
	if (NULL == new_node)
	{
	   DEBUG_ONLY(fprintf(stderr, "\nMemory allocation for the new_node failed\n");)

	   return MEM_ERROR;
	}

	/* initialize new_node fields */
	InitNewNodeIMP(new_node, data_);

	/* InsertIMP(avl_, &avl_->children[LEFT], new_node) */
	InsertIMP(avl_, &avl_->dummy.children[LEFT], new_node);

	/* return SUCCESS */
	return AVL_NO_ERROR;
}

void *RemoveIMP(avl_ty *avl_, node_ty **where_, const void *key_, void *ret_data_)
{
	/* assert */

	/* if *where_ == NULL */
		/* return NULL */

	/* if *where_ data != key_ */
		/* side = is_before(*where_ data, key_) */
			/* where_->children[side] = RemoveIMP(side) */
			/* return where_ */

	/* if ret_data_ == NULL */
		/* ret_data_ = where_ data */

	/* if where_ has no childrens */
		/* corrupt and delete where_ */
		/* return NULL */

	/* if where_ has one child */
		/* save_node = child */
		/* corrupt and delete where_ */
		/* return child */

	/* if where_ has two childrens */
		/* where_ data = GoEndLeftLeafIMP(right child)->data */
		/* right child = RemoveIMP(right child) */
 		/* return right_child */

	return NULL; /* temp */
}


void *AvlRemove(avl_ty *avl_, const void *key_)
{
	/* assert */

	/* RemoveIMP(avl_, root, key_, ret_data) */

	/* return ret_data */
}

static size_t MaxIMP(size_t x, size_t y)
{
	if (x > y)
	{
		return x;
	}

	return y;
}

static size_t HeightIMP(node_ty *node_)
{
	/* if node_ == NULL */
	if (NULL == node_)
	{
		/* return 0 */
		return 0;
	}

	/* return 1 + MAX(HeightIMP(left child), HeightIMP(right child)) */
	return 1 + MaxIMP(HeightIMP(node_->children[LEFT]), HeightIMP(node_->children[RIGHT]));
}

size_t AvlHeight(const avl_ty *avl_)
{
	/* assert */
	assert(NULL != avl_);

	/* return HeightIMP(root) */
	return HeightIMP(avl_->dummy.children[LEFT]);
}

static size_t SizeIMP(node_ty *node_)
{
	/* if node_ == NULL */
	if (NULL == node_)
	{
		/* return 0 */
		return 0;
	}

	/* return 1 + SizeIMP(left child) + SizeIMP(right child) */
	return 1 + SizeIMP(node_->children[LEFT]) + SizeIMP(node_->children[RIGHT]);
}

size_t AvlSize(const avl_ty *avl_)
{
	/* assert */
	assert(NULL != avl_);

	/* return SizeIMP(root) */
	return SizeIMP(avl_->dummy.children[LEFT]);
}

int AvlIsEmpty(const avl_ty *avl_)
{
	/* assert */
	assert(NULL != avl_);

	/* return if NULL == root */
	return (NULL == avl_->dummy.children[LEFT]);
}

void *FindIMP(avl_ty *avl_, node_ty *node_, const void *key_)
{
	/* assert */
	assert(NULL != avl_);
	assert(NULL != key_);

	/* if node_ == NULL */
	if (NULL == node_)
	{
		/* return NULL */
		return NULL;
	}

	/* if node_->data == key_ */
	if (node_->data == key_)
	{
		/* return node_ */
		return node_;
	}

	/* return FindIMP(node_->children[IsBefore(node_, key_)], key_) */
	return FindIMP(avl_, node_->children[avl_->is_before(node_->data, key_, avl_->is_before_params)], key_);
}

void *AvlFind(avl_ty *avl_, const void *key_)
{
	/* assert */
	assert(NULL != avl_);
	assert(NULL != key_);

	/* return FindIMP(avl_, root, key_) */
	return FindIMP(avl_, avl_->dummy.children[LEFT], key_);
}

int PreOrderIMP(node_ty *node_, op_func_ty oper_, void *func_param_)
{
	int status = 0;

	/* assert */
	assert(NULL != oper_);

	/* if node_  == NULL */
	if (NULL == node_)
	{
		/* return SUCCESS */
		return AVL_NO_ERROR;
	}

	/* status = oper_(node_ data, func_param_) */
	status = oper_(node_->data, func_param_);

	/* if status == SUCCESS */
	if (AVL_NO_ERROR == status)
	{
		/* status = PreOrderIMP(left child, oper_, func_param_) */
		status = PreOrderIMP(node_->children[LEFT], oper_, func_param_);
	}

	/* if status == SUCCESS */
	if (AVL_NO_ERROR == status)
	{
		/* status = PreOrderIMP(right child, oper_, func_param_) */
		status = PreOrderIMP(node_->children[RIGHT], oper_, func_param_);
	}

	/* return status */
	return status;
}

int InOrderIMP(node_ty *node_, op_func_ty oper_, void *func_param_)
{
	int status = 0;

	/* assert */
	assert(NULL != oper_);

	/* if node_  == NULL */
	if (NULL == node_)
	{
		/* return SUCCESS */
		return AVL_NO_ERROR;
	}

	/* status = InOrderIMP(left child, oper_, func_param_) */
	status = InOrderIMP(node_->children[LEFT], oper_, func_param_);

	/* if status == SUCCESS */
	if (AVL_NO_ERROR == status)
	{
		/* status = oper_(node_ data, func_param_) */
		status = oper_(node_->data, func_param_);
	}

	/* if status == SUCCESS */
	if (AVL_NO_ERROR == status)
	{
		/* status = InOrderIMP(right child, oper_, func_param_) */
		status = InOrderIMP(node_->children[RIGHT], oper_, func_param_);
	}

	/* return status */
	return status;
}

int PostOrderIMP(node_ty *node_, op_func_ty oper_, void *func_param_)
{
	int status = 0;

	/* assert */
	assert(NULL != oper_);

	/* if node_  == NULL  */
	if (NULL == node_)
	{
		/* return SUCCESS */
		return AVL_NO_ERROR;
	}

	/* status = PostOrderIMP(left child, oper_, func_param_) */
	status = PostOrderIMP(node_->children[LEFT], oper_, func_param_);

	/* if status == SUCCESS */
	if (AVL_NO_ERROR == status)
	{
		/* status = PostOrderIMP(right child, oper_, func_param_) */
		status = PostOrderIMP(node_->children[RIGHT], oper_, func_param_);
	}

	/* if status == SUCCESS */
	if (AVL_NO_ERROR == status)
	{
		/* status = oper_(node_ data, func_param_) */
		status = oper_(node_->data, func_param_);
	}

	/* return status */
	return status;
}

int AvlForEach(avl_ty *avl_, traversal_ty traverse_mode_,
                                             op_func_ty oper_, void *func_param_)
{
	/* assert */
	assert(NULL != avl_);
	assert(0 <= traverse_mode_);
	assert(2 >= traverse_mode_);
	assert(NULL != oper_);

	/* switch (traverse_mode_) */
	switch (traverse_mode_)
	{
		/* case PRE_ORDER: */
		case PRE_ORDER:
	        /* return PreOrderIMP(root, oper_, func_param_) */
	        return PreOrderIMP(avl_->dummy.children[LEFT], oper_, func_param_);

		/* case IN_ORDER: */
	    case IN_ORDER:
	        /* return InOrderIMP(root, oper_, func_param_) */
	        return InOrderIMP(avl_->dummy.children[LEFT], oper_, func_param_);

		/* case POST_ORDER: */
	    case POST_ORDER:
	        /* return PostOrderIMP(root, oper_, func_param_) */
	         return PostOrderIMP(avl_->dummy.children[LEFT], oper_, func_param_);
	}
}