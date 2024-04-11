#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "trie.h"
#include "utils.h"

#define MARK_AS_USED 0xDEADBEEF

typedef struct t_node
{
	int is_full;
	struct t_node *childrens[2];
} t_node_ty;

struct trie
{
	t_node_ty root;
	size_t depth;
};

enum
{
	ZERO = 0,
	ONE = 1,
	BITS_IN_BYTE = 8
};

enum
{
	IS_TAKEN,
	MEM_ERROR,
	FOUND
};


static t_node_ty *CreateNodeIMP()
{
	/* memory allocation for the node */
	t_node_ty *ret = (t_node_ty *)malloc(sizeof(t_node_ty));

	/* if allocation failed */
	RET_IF_BAD(NULL != ret, NULL);

	/* return ret */
	return ret;
}

static void InitNodeIMP(t_node_ty *node_)
{
	/* init node fields */
	node_->is_full = 0;                     
	node_->childrens[ZERO] = NULL;
	node_->childrens[ONE] = NULL;
}

trie_ty *TrieCreate(size_t depth_)
{
	trie_ty *ret = NULL; /* TODO: ret */

	/* assert */
	assert(0 != depth_);
	assert(depth_ <= BITS_IN_BYTE * sizeof(unsigned long));

	/* allocate memory for trie instance */
	ret = (trie_ty *)malloc(sizeof(trie_ty));

	/* if allocation failed */
	RET_IF_BAD(NULL != ret, NULL);

	/* init trie fields */
	InitNodeIMP(ret->root);
	trie->depth = depth_;

	/* return trie */
	return ret;
}

static void DestroyIMP(t_node_ty *node_)
{
	/* if 0 == depth_ */
	if (MARK_AS_USED == *node_ || NULL == node_)
	{
		/* return */
		return;
	}

	DestroyIMP(node_->childrens[ZERO]);
	DestroyIMP(node_->childrens[ONE]);

	DEBUG_ONLY(node_->childrens[ZERO] = BAD_MEM(t_node_ty *););
    DEBUG_ONLY(node_->childrens[ONE] = BAD_MEM(t_node_ty *););

	/* free node_ */
	free(node_);
}

void TrieDestroy(trie_ty *trie_)
{
	/* NULL handler */
	if (NULL == trie_)
	{
		return;
	}

	/* free all pointers in trie_ */
	DestroyIMP(trie_->root.childrens[ZERO], trie_->depth);
	DEBUG_ONLY(node_->childrens[ZERO] = BAD_MEM(t_node_ty *););

	DestroyIMP(trie_->root.childrens[ONE], trie_->depth);
	DEBUG_ONLY(node_->childrens[ONE] = BAD_MEM(t_node_ty *););

	/* free trie_ */
	free(trie_);
}

static unsigned long OnBitAndZeroRest(unsigned long num_, size_t bit_idx_)
{
	const unsigned long lit_bit = 1UL << bit_idx_;
	unsigned long ret = num_;

	/* assert for check turning a zero to one */
	assert(0 == num_ & lit_bit);

	/* flip the bit number depth_ from the end (LSB) to 1 */
	ret |= lit_bit;

	/* set all the bits after to 0 */
	ret &= ~(lit_bit - 1);
	
	/* return new number */
	return ret;
}

static int IsLeafIMP(t_node_ty *node_)
{
	if (NULL == node_)
	{
		return 0;
	}

	return 1;
}

static int CalcSideIMP(unsigned long num_, size_t depth_)
{
	/* return calc side */
	return (num_ >> (depth_-1) & 0x1);
}

static int GetIMP(t_node_ty **node_, unsigned long *num_, size_t depth_) /* TODO: change it */
{
	size_t side = 0;
	int status = 0;

	/* assert */
	assert(NULL != node_);

	/* if is_full */
	if (*node_ && (MARK_AS_USED == *node_ || (*node_)->is_full))
	{
		/* return IS_TAKEN */
		return IS_TAKEN;
	}

	/* if available */
	if (NULL == *node_)
	{
		/* if 0 == depth_ */
		if (0 == depth_)
		{
			/* mark as used */
			*node_ = MARK_AS_USED;

			/* return FOUND */
			return FOUND;
		}

		/* allocate memory for new node and init his feilds, NULL failure check, if not - attach him */
		RET_IF_BAD(NULL != (*node_ = CreateNodeIMP()), MEM_ERROR, "GetIMP() memory allocation failed");

		/* init new node */
		InitNodeIMP(*node_);
	}

	/* side = calculate which side to go based on the current bit in wanted_ip_ */
	side = CalcSideIMP(*num_, depth_); 

	/* status = GetIMP(node_ children in side place, num_, depth_ - 1) */
	status = GetIMP(&(*node_)->childrens[side], num_, depth_ - 1);

	/* if address is not available */
	if (IS_TAKEN == status && ONE == side)
	{
		/* new_num = flip current bit to 1, and rest of the bits after to 0 */
		OnBitAndZeroRest(num_, depth_ - 1);

		/* status = GetIMP(node_ ONE child, new_num, depth_ - 1) */
		status = GetIMP(&(*node_)->childrens[ONE], num_, depth_ - 1);
	}

	/* if status is IS_TAKEN or MEM_ERROR */
	RET_IF_BAD(FOUND == status, status, "");



	/* if both childrens are full */
	if (IsLeafIMP((*node_)->childrens[ZERO]) && IsLeafIMP((*node_)->childrens[ONE]))
	{
			(*node_)->is_full = 1;
	}

	/* return FOUND */
	return FOUND;
}

unsigned long TrieGet(trie_ty *trie_, unsigned long wanted_num_) /* TODO: change it */
{
	/* assert */
	assert(NULL != trie_);

	/* return GetIMP(&trie_->trie, &wanted_num_, trie_->depth) */
	return GetIMP(&trie_->root, &wanted_num_, trie_->depth);
}

void TrieFreeIMP(t_node_ty **node_, unsigned long num_, size_t depth_)
{
	size_t side = 0;

	/* assert */
	assert(NULL != node_);

	/* if 0 == depth_ */
	if (0 == depth_)
	{
		/* *node = NULL */
		*node_ = NULL;

		/* empty return */
		return;
	}

	if (NULL == *node_ || NULL == node_) /* TODO: dont do it - do assert */
	{
		return;
	}

	side = (num_ >> (depth_-1) & 0x1); /* TODO: do it in function - WhichSide() */

	/* TrieFreeIMP(next bit, num_, depth - 1) */
	TrieFreeIMP(&(*node_)->childrens[side], num_, depth_ - 1);

	/* (*node_)->is_full = 0 */
	(*node_)->is_full = 0;
}

void TrieFree(trie_ty *trie_, unsigned long num_)
{
	assert(NULL != trie_);

	/* TrieFreeIMP(&trie_->trie, num_, trie_->depth) */
	TrieFreeIMP(&trie_->root, num_, trie_->depth);
}

size_t CountIMP(t_node_ty *node_, size_t depth_)
{ 
	/* if NULL == node_ */
	if(NULL == node_)
	{
		/* return 0 */
		return 0;
	}

	/*if 0 == depth_ */
	if (0 == depth_ && NULL != node_)
	{
		/* return 1 */
		return 1;
	}
                                                  /* TODO: add optimization of calculate only the is_full sub tries */

	/*return CountIMP(ZERO sub tree, depth_ - 1) + CountIMP(ONE sub tree, depth_ - 1) */
	return CountIMP(node_->childrens[ZERO], depth_ - 1) + CountIMP(node_->childrens[ONE], depth_ - 1);
}

size_t TrieCountAvailable(const trie_ty *trie_)
{
	/* assert */
	assert(NULL != trie_);

	/* return (2^depth - CountIMP(&trie_->trie, trie_->depth)) */
	return ((1<<trie_->depth) - CountIMP(trie_->root, trie_->depth));
}

int TrieIsAllTaken(const trie_ty *trie_)
{
	/* assert */
	assert(NULL != trie_);

	/* return if root is_full == 1 */
	return trie_->root->is_full;
}