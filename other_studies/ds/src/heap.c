#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "heap.h"
#include "vector.h"
#include "utils.h"


struct heap_queue
{
	vector_ty *vector;
	cmp_func_ty cmp;
	const void *cmp_params;
};


heap_ty *HeapCreate(cmp_func_ty cmp_, const void *cmp_params_, size_t init_capacity_)
{
	heap_ty *heap = NULL;

	/* assert */
	assert(NULL != cmp_);

	/* allocate memory for the heap */
	heap = (heap_ty*)malloc(sizeof(heap_ty));

	/* if allocation failed */
	if (NULL == heap)
	{
		/* return NULL */
		return NULL;
	}

	/* create the vector within the heap (each element is pointer) */
	heap->vector = VectorCreate(init_capacity_, sizeof(void*));

	/* if creation failed */
	if (NULL == heap->vector)
	{
		/* corrupt and free heap */
		DEBUG_ONLY(heap->vector = BAD_MEME(vector_ty););
		DEBUG_ONLY(heap->cmp = BAD_MEME(cmp_func_ty););
		DEBUG_ONLY(heap->cmp_params = BAD_MEME(const void););
		free(heap);

		/* return NULL */
		return NULL;
	}

	/* init cmp_ and cmp_params_ in heap */
	heap->cmp = cmp_;
	heap->cmp_params = cmp_params_;

	/* return heap */
	return heap;
}

void HeapDestroy(heap_ty *heap_)
{
	if (NULL == heap_)
	{
		return;
	}

	/* destroy vector */
	VectorDestroy(heap_->vector);

	/* corrupt and free heap_ */
	DEBUG_ONLY(heap->vector = BAD_MEME(vector_ty););
	DEBUG_ONLY(heap->cmp = BAD_MEME(cmp_func_ty););
	DEBUG_ONLY(heap->cmp_params = BAD_MEME(const void););
	free(heap_);
}

static void Swap(void **data1_, void **data2_)
{
	void *temp = NULL;

	/* assert */
	assert(NULL != data1_);
	assert(NULL != data2_);

	/* swap */
	temp = *data1_;
	*data1_ = *data2_;
	*data2_ = temp; 
}

static void HeapifyUpIMP(heap_ty *heap_, size_t node_indx_)
{
	void *arr = NULL;
	void *curr_node_value = NULL;
	void *parent_value = NULL;

	/* assert */
	assert(NULL != heap_);

	/* get array from vector */
	arr = VectorGetValue(heap_->vector, 0);

	/* one step backward */
	arr = (void **)arr - 1;

	curr_node_value = ((void **)arr + node_indx_);
	parent_value = ((void **)arr + node_indx_ / 2);

	/* while not top and curr < parent */
	while (1 != node_indx_ && 0 > heap_->cmp(curr_node_value, parent_value, heap_->cmp_params))
	{

		/* swap data with parent */
		Swap(curr_node_value, parent_value);

		/* advanve curr to parent */
		node_indx_ /= 2;

		curr_node_value = ((void **)arr + node_indx_);
		parent_value = ((void **)arr + node_indx_ / 2);
	}
}

int HeapEnqueue(heap_ty *heap_, const void *data_)
{
	status_ty push_status = SUCCESS;

	/* assert */
	assert(NULL != heap_);
	assert(NULL != data_);

	/* push back the address of data_ to the vector */
	push_status = VectorPushBack(heap_->vector, &data_);

	/* push failure handler */
	if (SUCCESS != push_status)
	{
		return ERROR;
	}

	/* heapify up if needed */
	HeapifyUpIMP(heap_, VectorGetSize(heap_->vector));

	/* return no error */
	return NO_ERROR;
}

static size_t SmallestSon(heap_ty *heap_, void *arr_, size_t node_indx_)
{
	void *left_node_value = NULL;
	void *right_node_value = NULL;
	void *parent_value = NULL;
	int left_child_cmp = 0;
	int right_child_cmp = 0;
	size_t ret_indx = node_indx_;
	
	/* assert */
	assert(NULL != heap_);

	/* get value of left child */
	left_node_value = (void **)arr_ + ((node_indx_) * 2);

	/* get value of the parent */
	parent_value = (void **)arr_ + node_indx_ ;

	/* cmp between parent and left child */
	left_child_cmp = heap_->cmp(left_node_value, parent_value, heap_->cmp_params);

	if (0 > left_child_cmp)
	{
		ret_indx = node_indx_ * 2;
	}

	/* if there is right child */
	if (VectorGetSize(heap_->vector) >= (node_indx_ * 2) + 1)
	{
		/* get right child value */
		right_node_value = ((void **)arr_ + (node_indx_ * 2) + 1);

		/* cmp with smallest node */
		right_child_cmp = heap_->cmp(right_node_value, ((void **)arr_ + ret_indx), heap_->cmp_params);

		/* if right child is the smallest - set ret_indx to his index */
		if (0 > right_child_cmp)
		{
			ret_indx = (node_indx_ * 2) + 1;
		}
	}

	/* return ret_indx */
	return ret_indx;
}

static void HeapifyDwanIMP(heap_ty *heap_, size_t node_indx_)
{
	void *arr = NULL;
	size_t smallest_son = 0;

	/* assert */
	assert(NULL != heap_);

	/* get array from vector */
	arr = VectorGetValue(heap_->vector, 0);

	/* one step backward */
	arr = (void **)arr - 1;

	smallest_son = SmallestSon(heap_, arr, node_indx_);

	/* while not bottom and not smallest in trio */
	while (node_indx_ != smallest_son && node_indx_*2 <= VectorGetSize(heap_->vector))
	{
		/* swap curr data with smallest son */
		Swap(((void **)arr + node_indx_), ((void **)arr + smallest_son));
		/* advance curr to smallest son */
		node_indx_ = smallest_son;

		if (node_indx_*2 > VectorGetSize(heap_->vector))
		{
			break;
		}

		smallest_son = SmallestSon(heap_, arr, node_indx_);
	}
}

void *HeapDequeue(heap_ty *heap_)
{
	void *ret = NULL;
	/* assert */
	assert(NULL != heap_);

	/* ret = first vector element */
	ret = VectorGetValue(heap_->vector, 0);

	/* override the address in the peek element with the address in the last element */
	VectorSetValue(heap_->vector, 0, VectorGetValue(heap_->vector, VectorGetSize(heap_->vector) - 1));

	/* decreament size */
	VectorPopBack(heap_->vector);

	/* heapify dwan if needed */
	HeapifyDwanIMP(heap_, 1);

	/* return ret */
	return ret;
}

void *HeapPeek(heap_ty *heap_)
{
	/* assert */
	assert(NULL != heap_);

	/* return first element */
	return VectorGetValue(heap_->vector, 0);
}

size_t HeapSize(const heap_ty *heap_)
{
	/* assert */
	assert(NULL != heap_);

	/* return vector size */
	return VectorGetSize(heap_->vector);
}

int HeapIsEmpty(const heap_ty *heap_)
{
	/* assert */
	assert(NULL != heap_);

	/* return if vector is empty */
	return VectorIsEmpty(heap_->vector);
}

void *HeapRemove(heap_ty *heap_, int (*is_match)(const void *data_, const void *params_), const void *is_match_params_)
{
	/* assert */

	/* find the index of the element with th
}

/* 
void HeapSort(int arr_[], size_t size_)
{
	/* assert */
	
	/* create heap */
	/* Enqueue each element to the heap */
	/* while there is more then one element in the heap */
		/* swap first element with last element */
		/* decreament size */
		/* heapify down the peek element */
	/* reverae the vector if you want */
/*}*/
/*