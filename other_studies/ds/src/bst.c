#include <assert.h> /* assert */

#include "bst.h"

struct bst_node
{
	void *data;
	bst_node_ty *parent;
	bst_node_ty *children[2];
};

struct bst
{
	bst_node_ty dummy;
	is_before_func_ty is_before;
	const void *is_before_params;
};

enum
{
	LEFT = 0,
	RIGHT = 1
};

enum
{
	IS_NO_BEFORE = 0,
	IS_BEFORE = 1
};

enum
{
	IS_PREV = 0,
	IS_NEXT = 1
};

bst_ty *BtsCreate(is_before_func_ty is_before_func_, const void *params_)
{
	/* assert */

	/* allocate memory for bst pointer */
	/* if NULL */
		/* return NULL */

	/* initialize bst fields */
	/* initialize dummy to BAD_MEM */

	/* return bst pointer */
}

void BstDestroy(bst_ty *bst_)
{
	/* if NULL */
		/* return */

	/* curr_iter = Begin() */
	/* while !BstIsEmpty(bst_) */
		/* curr_iter = Remove(curr_iter) */

	/* DEBUG_ONLY - corrupt bst_ */
	/* free memory of bst_ */
}

size_t BstSize(const bst_ty *bst_)
{
	/* assert */

	/* runner_iter = Begin() */
	/* while runner_iter != End()*/
		/* increament size counter */
		/* runner_iter = Next(runner_iter) */

	/* return size counter */
}

int BstIsEmpty(const bst_ty *bst_)
{
	/* assert */

	/* return (NULL == bst_->dummy.children[LEFT]) */
}

static bst_node_ty **FindIMP(bst_ty *bst_, const void *data_, bst_node_ty **return_parent_)  /* my TODO: use healper functions ? */
{
	/* assert */

	/* ret = root */
	/* while forever */
		/* if ret->node == NULL || ret is match to data_ */
			/* return ret */
		/* if is ret is before */
			/* ret = ret right child */
			/* continue */
		/* ret = ret left child */
}

status_ty BstInsert(bst_ty *bst_, const void *data_) /* my TODO: change the pseudo code */
{
	/* assert */

	/* allocate memory for new_node */
	/* NULL fail handler */

	/* initial new_node childrens */

	/* to_insert = FindIMP(bst_, data_, return_parent) */
	/* to_insert != NULL handler */

	/* new_node->parent = return_parent */
	/* *to_insert = new_node */ 

	/* return BST_NO_ERROR */
}

static void DisconnectIMP(bst_node_ty *to_disconnect_)
{
	/* assert */

	/* to_disconnect_->parent.children[WhichSideIMP(to_disconnect_)] = NULL */
}

static int WhichSideIMP(bst_node_ty *child_)
{
	/* assert */

	/* return (child_->parent.children[RIGHT] == child_) */
}

static void ConnectNodesIMP(bst_node_ty *parent_, bst_node_ty *new_child_, int side_)
{
	/* assert */

	/* parent_->children[side_] = new_child_ */
	/* new_child.parent = parent_ */
}

static void ConnectIMP(bst_node_ty *remove_node_, int side_)
{
	/* assert */

	/* ConnectNodesIMP(remove_node_.parent, remove_node_.children[!to_remove.children[RIGHT]], side_) */
}

static void SimpleRemoveIMP(bst_node_ty *to_remove_)
{
	/* assert */

	/* if no childrens */
		/* DisconnectIMP(to_remove_) */
	/* else if one child */
		/* ConnectIMP(to_remove_, WhichSideIMP(to_remove_)) */

	/* free memory of remove_node_ */
}

bst_iter_ty BstRemove(bst_ty *bst_, bst_iter_ty_ to_remove_)
{

	/* assert */
	/* to_remove_next_iter = Next(to_remove_) */

	/* if two childrens */
		/* to_remove_node.data = to_remove_next_node.data */
		/* SimpleRemoveIMP(to_remove_next_node) */

	/* return to_remove_ */
}

static bst_iter_ty DownUntilIMP(bst_node_ty *node, const int side_) /* my TODO: bst_node_ty *node */
{
	/* assert */

	/* while bst_iter_->where.children[side_] != NULL */
		/* bst_iter_->where = bst_iter_->where.children[side_] */

	/* return bst_iter_ */
}

bst_iter_ty BstEnd(bst_ty *bst_)
{
	/* assert */

	/* return ToItrIMP(&bst_->dummy) */
}

bst_iter_ty BstBegin(bst_ty *bst_)
{
	/* assert */

	/* return DownUntilIMP(BstEnd(bst_), LEFT) */
}

status_ty BstIsSameIter(bst_ty *bst_, bst_iter_ty bst_first_iter_,
                                                    bst_iter_ty bst_secend_iter_)
{
	/* assert */

	/* if bst_first_iter_.where != bst_secend_iter_.where || bst_first_iter_.bst != bst_secend_iter_.bst */
		/* return BST_ERROR */

	/* return  BST_NO_ERROR */
}

void *BstGetData(bst_iter_ty bst_iter_)
{
	/* assert */

	/* return bst_iter_->where.data */
}

bst_iter_ty BstNext(bst_iter_ty bst_iter_)
{
	/* assert */

	/* return PrevNextIMP(IS_NEXT) */
}

bst_iter_ty BstPrev(bst_iter_ty bst_iter_)
{
	/* assert */

	/* return PrevNextIMP(IS_PREV) */
}

bst_iter_ty BstFind(bst_ty *bst_, const void *data_)
{
	/* assert */

	/* find_iter = FindIMP(bst_, data_, return_parent) */
	/* find_iter NULL handler */

	/* return find_iter */
}

status_ty BstForEach(bst_iter_ty from_, bst_iter_ty to_,
				                          op_func_ty op_func_, void *func_param_)
{
	/* assert */

	/* runner_iter = from_ */
	/* while runner_iter != to_ */
		/* status = op_func_(runner_iter data, func_param_)  */
		/* if fail status */
			/* return BST_ERROR */
		/* runner_iter = Next(runner_iter) */

	/* return BST_NO_ERROR */
}

static bst_iter_ty UpUntilIMP(bst_iter_ty bst_iter_, const int child_side_) /* my TODO: bst_node_ty *node */
{
	/* assert */

	/* convert bst_iter_ to node */
	/* while WhichSideIMP(iter_node) != child_side_ */
		/* iter_node = iter_node.parent */

	/* return ToItrIMP(iter_node.parent) */
}

static bst_iter_ty PrevNextIMP(bst_iter_ty bst_iter_, const int is_next_) /* my TODO: bst_node_ty *node */
{
	/* assert */

	/* if bst_iter_->where.children[is_next_] != NULL */
		/* bst_iter_->where = bst_iter_->where.children[is_next_] */
		/* bst_iter_->where = DownUntilIMP(bst_iter_->where, !is_next_) */
		/* return bst_iter_ */

	/* else */
		/* return UpUntilIMP(bst_iter_, !is_next_) */
}

static bst_iter_ty ToItrIMP(bst_node_ty *node_)
{
	/* assert */

	/* return iter to node_ */
}

static int ValidIterIMP(bst_iter_ty bst_iter_)
{
	/* return (NULL != bst_iter_->where.parent.data) */ 
}

static void InitNewNodeParamsImp(bst_node_ty *to_init_)
{
	/* assert */

	/* assign NULL to all fields */
}

