#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* memcpy */

#include "utils.h"

enum 
{
	IS_NOT_SWAP = 0,
	 IS_SWAP = 1
};

enum
{
    NO_ERROR = 0,
    MEM_ERROROR
};

static void Swap(int *i, int *j)
{
	int temp = *i;
	*i = *j;
	*j = temp;
}

static size_t FindSubArrSmallestElement(int *array, size_t index, size_t len)
{
	size_t smallest_element_index = index;
	size_t i = 0;

	assert(NULL != array);
	assert(0 < len);

	for (i = index + 1; i < len; ++i)
	{
		if (array[i] < array[smallest_element_index])
		{
			smallest_element_index = i;
		}
	}

	return smallest_element_index;
}

void BubbleSort(int *array, size_t len)
{
	size_t i = 0;
	size_t j = 0;

	assert(NULL != array);
	assert(0 < len);


	for (i = 0; i < len; ++i)
	{
		for (j = 0; j < len - 1; ++j)
		{
			if ( array[j] > array[j + 1])
			{
				Swap(array + j, array + j + 1);
			}
		}
	}
}

void SelectionSort(int *array, size_t len)
{
	size_t smallest_element_index = 0;
	size_t i = 0;

	assert(NULL != array);
	assert(0 < len);

	for (i = 0; i < len; ++i)
	{
		smallest_element_index = FindSubArrSmallestElement(array, i, len);

		Swap(array + i, array + smallest_element_index);
	}
}

void InsertionSort(int *array, size_t len)
{
	int key = 0;
	int i = 0;
	int j = 0;

	assert(NULL != array);
	assert(0 < len);

	for (i = 1; i < (int)len; ++i)
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

void BubbleSortOptimized(int *array, size_t len)
{
	int is_swap = IS_SWAP;
	size_t i = 0;
	size_t j = 0;

	assert(NULL != array);
	assert(0 < len);


	for (i = 0; i < len && IS_SWAP == is_swap; ++i)
	{
		is_swap = IS_NOT_SWAP;

		for (j = 0; j < len - 1; ++j)
		{
			if ( array[j] > array[j + 1])
			{
				Swap(array + j, array + j + 1);

				is_swap = IS_SWAP;
			}
		}
	}
}


int BinarySearch(int arr_[], size_t len_, int key_)
{
	size_t start = 0;
	size_t end = 0; 
	size_t mid = 0; 

	/* assert */
	assert(NULL != arr_);

	end = len_ - 1;
	mid = (start + end) / 2;

	/* while mid != start */
	while (mid != start)
	{
		/* if arr_[mid] == key_ */
		if (arr_[mid] == key_)
		{
			/* return mid */
			return mid;
		}

		/* else if arr_[mid] < key_ */
		else if (arr_[mid] < key_)
		{
			/* start = mid */
			start = mid;
		}

		/* else */
		else
		{
			/* end = mid */
			end = mid;
		}

		/* calculate mid */
		mid = (start + end) / 2;
	}

	/* return not found */
	return -1;
}

int *BinarySearchRec(int *begin_, int *end_, int key_)
{
	size_t mid = 0;

	/* assert */
	assert(NULL != begin_);
	assert(NULL != end_);

	/* if begin == end */
	if (begin_ == end_)
	{
		/* return NULL */ 
		return NULL;
	}

	/* mid = (end - begin) / 2 */
	mid = (end_ - begin_) / 2;

	/* if key_ == mid */
	if (begin_[mid] == key_)
	{
		/* return mid */
		return (begin_ + mid);
	}

	/* else if key_ > mid */
	else if (key_ > begin_[mid])
	{
		/* return BinarySearchRec(begin_ + mid + 1, end_, key_) */
		return BinarySearchRec(begin_ + mid + 1, end_, key_);
	}

	/* else */
	else
	{
		/* return BinarySearchRec(begin_, begin_ + mid - 1, key_) */
		return BinarySearchRec(begin_, begin_ + mid, key_);
	}
}

static int MergeIMP(int *arr1_, size_t len1_, int *arr2_, size_t len2_)
{
    int *new_arr = NULL;
    size_t index1 = 0;
    size_t index2 = 0;
    size_t i = 0;

    /*assert*/
    assert(NULL != arr1_);
    assert(NULL != arr2_);

    /* memory allocation for new arr in length of len1_+len2_ */
    new_arr = (int *) malloc(sizeof(int)*(len1_ + len2_));

	/* NULL failure handler */
    RET_IF_BAD(NULL != new_arr, -1);

    /* for len1_+len2_ */
    for(i = 0; i< (len1_ + len2_); ++i)
    {
        /* if the smallest value in arr1_ or arr2_ is empty */
        if((index1 < len1_ && arr1_[index1] < arr2_[index2]) || index2 == len2_)
        {
            /* new_arr[i] =  arr1_[index1] */
            new_arr[i] = arr1_[index1];

            /* ++index1 */
            ++index1;

            continue;
        }

        /* else */
        else
        {
            /* new_arr[i] =  arr2_[index2] */
            new_arr[i] = arr2_[index2];

            /* ++index2 */
            ++index2;

            continue;
        }
    }

    /* for len1_+len2_ */
    for(i = 0; i < len1_ + len2_; ++i)
    {
        /* copy elements from new_arr to original arr */
        *(arr1_ + i) = *(new_arr + i);
    }

    /* free new_arr */
    free(new_arr);

    /* return SUCESS */
    return NO_ERROR;
}

int MergeSort(int *arr_to_sort_, size_t num_elements_)
{
	size_t mid = 0;

	/* assert */
	assert(NULL != arr_to_sort_);

	/* if 1 == num_elements_ */
	if (1 == num_elements_)
	{
		/* return no error */
		return NO_ERROR;
	}

	/* mid = num_elements_ / 2 */
	mid = num_elements_ / 2;

	/* MergeSort(arr_to_sort_, mid) */
	MergeSort(arr_to_sort_, mid);

	/* MergeSort(arr_to_sort_ + mid, num_elements_ - mid) */
	MergeSort(arr_to_sort_ + mid, num_elements_ - mid);

	/* return MergeIMP(arr_to_sort_, mid, num_elements_ - mid) */
	return MergeIMP(arr_to_sort_, mid, arr_to_sort_ + mid, num_elements_ - mid);
}

static void SwapIMP(void *left_, void *right_, size_t size_)
{
    char temp[sizeof(size_t)];

    /* assert */
    assert(NULL != left_);
    assert(NULL != right_);
    assert(0 < size_);

    /* copy to temp size_ elements from left_ */
    memcpy(temp, left_, size_);

    /* copy to left_ size_ elements from right_ */
    memcpy(left_, right_, size_);

    /* copy to right_ size_ elements from temp_ (left_) */
    memcpy(right_, temp, size_);
}

static size_t PartitionIMP(void *base_, size_t nmemb_, size_t size_, int (*compar)(const void *data1_, const void *data2_)) 
{
    char *left = base_;
    char *right = (char *)base_ + (nmemb_ - 1) * size_;
    char *pivot = NULL;
    
    /* assert */
    assert(NULL != base_);
    assert(NULL != compar);

    /* set pivot */
    pivot = base_;

    /* while left<=right */
    while (left <= right)
     {
     	/* While *left < *pivot */
        while (compar(left, pivot) <= 0) 
        {
        	/* left++ */
            left += size_;
        }

        /* While *right > *pivot */
        while (compar(right, pivot) > 0) 
        {
        	/* --right */
            right -= size_;
        }

        /* swap */
        if (left <= right) 
        {
            SwapIMP(left, right, size_);

            left += size_;
            right -= size_;
        }
    }

    if(0 > compar((char*)left - size_, pivot))
    {
    	SwapIMP(base_, (char*)left - size_, size_);
    	left = (char*)left - size_;
    }
    else
    {
    	SwapIMP(base_, left, size_);
    }

    return ((char*)left-(char *)base_)/size_;
}

void QSort(void *base_, size_t nmemb_, size_t size_, int(*compar)(const void *data1, const void *data2))
{
    size_t partition = 0;

    /* assert */
    assert(NULL != base_);
    assert(NULL != compar);

    /* if 1 == nmemb */
    if(1 == nmemb_)
    {
        /* empty return */
        return;
    }

    /* PartitionSort(base, nmemb, size, compar) */
    partition = PartitionIMP(base_, nmemb_, size_, compar);

    /* QSort(base, nmemb/2, size) */
    QSort(base_, partition, size_, compar);

    /* QSort(base + (size*nmemb/2),  nmemb_ - partition - 1, size, compar) */
    QSort((char *)base_ + ((partition + 1)*size_), nmemb_ - partition - 1, size_, compar);
}
