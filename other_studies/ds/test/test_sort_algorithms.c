#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf */
#include <stdlib.h> /* rand */
#include <time.h> /* clock */

#include "sort_algorithms.h"

#define SORTS_NUM 6

typedef enum
{
	IS_SORT = 0,
	IS_NOT_SORT = 1
} sort_status_ty;

typedef enum
{
	NO_ERROR = 0,
	ERROR = 1
} test_status_ty;

typedef void (*sort_func_ty)(int *array, size_t len);

static void InitArr(int *array, size_t len);
static void PrintArr(int *array, size_t len);
static sort_status_ty SortChecking(int *array, size_t len);

static test_status_ty BinarySearchTest(void);
static test_status_ty BinarySearchRecTest(void);

static void MergeSortTest(int *array_, size_t len_);

static int CmpFunc(const void *data1_, const void *data2_);
static void QSortTest(int *array_, size_t len_);

static test_status_ty SortingAlgorithmsTest(void);

int main()
{
	test_status_ty sort_status = NO_ERROR;

	sort_status = SortingAlgorithmsTest();
	
	if (NO_ERROR != sort_status)
	{
		printf("\n *** There is an ERROR in one or more funtions ***\n");
		
		return ERROR;
	}
	
	printf("\n *** All the functions work ***\n");
	
	return NO_ERROR;
}

static void PrintArr(int *array, size_t len)
{
	size_t i = 0;

	assert(NULL != array);
	assert(0 < len);

	for (i = 0; i < len; ++i)
	{
		printf("%d ", array[i]);
	}

	printf("\n");
}

static void InitArr(int *array, size_t len)
{
	size_t i = 0;

	assert(NULL != array);
	assert(0 < len);	

	for (i = 0; i < len; ++i)
	{
		array[i] = rand() % 10000;
	}
}

static sort_status_ty SortChecking(int *array, size_t len)
{
	size_t i = 0;

	assert(NULL != array);
	assert(0 < len);

	for (i = 0; i < len - 1; ++i)
	{
		if (array[i] > array[i + 1])
		{
			return IS_NOT_SORT;
		}
	}

	return IS_SORT;
}

static test_status_ty BinarySearchTest(void)
{
	int arr[15] = {2, 4, 6, 7, 8, 9, 11, 23, 34, 36, 76, 80, 81, 82, 83};
	int key1 = 6;
	int key2 = 0;

	if (2 != BinarySearch(arr, 15, key1))
	{
		printf("\nBinarySearch() don't work properly.\n");

		return ERROR;
	}

	if (-1 != BinarySearch(arr, 15, key2))
	{
		printf("\nBinarySearch() don't work properly.\n");
		
		return ERROR;
	}

	printf("\nThe function work fine!\n");

	return NO_ERROR;
}

static test_status_ty BinarySearchRecTest(void)
{
	int arr[15] = {2, 4, 6, 7, 8, 9, 11, 23, 34, 36, 76, 80, 81, 82, 83};
	int key1 = 6;
	int key2 = 0;

	if ((arr + 2) != BinarySearchRec(arr, arr + 15, key1))
	{
		printf("\nBinarySearchRec() don't work properly.\n");

		return ERROR;
	}

	if (NULL != BinarySearchRec(arr, arr + 15, key2))
	{
		printf("\nBinarySearchRec() don't work properly.\n");
		
		return ERROR;
	}

	printf("\nThe function work fine!\n");

	return NO_ERROR;
}

static void MergeSortTest(int *array_, size_t len_)
{
	assert(NULL != array_);

	MergeSort(array_, len_);
}

static int CmpFunc(const void *data1_, const void *data2_) 
{
   return (*(int *)data1_ - *(int *)data2_);
}

static void QSortTest(int *array_, size_t len_)
{
	assert(NULL != array_);

	QSort(array_, len_, sizeof(int), CmpFunc);
}

static test_status_ty SortingAlgorithmsTest(void)
{
	int array[5000] = {0};
	int i = 0;
	size_t len = 5000;
	clock_t start = 0;
	clock_t end = 0;
	double run_time = 0;
	sort_status_ty sort_status = 0;

	sort_func_ty sort_funcs[] = 
	{
		BubbleSort,
		BubbleSortOptimized,
		SelectionSort,
		InsertionSort,
		MergeSortTest,
		QSortTest
	};
	
	const char* sort_funcs_names[] =
	{
		"BubbleSort",
		"BubbleSortOptimized",
		"SelectionSort",
		"InsertionSort",
		"MergeSort",
		"QSortTest"
    };

	for (i = 0; i < SORTS_NUM; ++i)
    {
    	printf("\nSort N.%d - %s:", i, sort_funcs_names[i]);

    	InitArr(array, len);

    	start = clock();
        sort_funcs[i](array, len);
        end = clock();

        run_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        sort_status = SortChecking(array, len);

        printf("\n-- Sort function return value: %d, ", sort_status);
        
        if (IS_SORT != sort_status)
        {
        	printf("not work --\n\n");

        	return ERROR;
        }

        else
        {
            printf("work! --\n");
            printf("-- Sort function run_time is: %f --\n\n", run_time);

        }
    }

    printf("\nTest for BinarySearch():\n");
    BinarySearchTest();

    printf("\nTest for BinarySearchRec():\n");
    BinarySearchRecTest();

    return NO_ERROR;
}