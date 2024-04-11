#include<assert.h> /* assert */
#include <stdio.h> /* printf */
#include <stdio.h> /* fprintf */

#include "sortedlist.h"

#define TESTS_NUM 12

enum {IS_NOT_EMPTY_LIST = 0, IS_EMPTY_LENGTH = 0, IS_EMPTY_LIST = 1};

typedef sorted_list_status_ty (*sorted_list_test_function_ty)();


static int IntCmpFunc(const void *data1, const void *data2);
static int FindIfCmpFunc(const void *data1, const void *data2);
static int PrintSortedListElementOp(void *data, void *func_param);
static void PrintSortedList(sorted_list_ty *sorted_list, int func_param);

static sorted_list_status_ty SortedListCreateDestroyTest(void);

static sorted_list_status_ty SortedListPopFrontTest(void);
static sorted_list_status_ty SortedListPopBackTest(void);

static sorted_list_status_ty SortedListLengthTest(void);
static sorted_list_status_ty SortedListIsEmptyTest(void);

static sorted_list_status_ty SortedListGetDataTest(void);
static sorted_list_status_ty SortedListInsertTest(void);
static sorted_list_status_ty SortedListRemoveTest(void);

static sorted_list_status_ty SortedListIterIsEqualTest(void);
static sorted_list_status_ty SortedListFindTest(void);
static sorted_list_status_ty SortedListFindIfTest(void);

static sorted_list_status_ty SortedListMergeTest(void);

static sorted_list_status_ty AutomateTests(void);

int main()
{
	sorted_list_status_ty sorted_list_status = 0;
	
	sorted_list_status = AutomateTests();
	
	if (SORTED_NO_ERROR != sorted_list_status)
	{
		printf("\n *** There is an ERROR in the program ***\n");
		
		return SORTED_GENERAL_ERR;
	}
	
	printf("\n *** All the tests passed, the program work fine ***\n");
	
	return SORTED_NO_ERROR;
}

static int IntCmpFunc(const void *data1, const void *data2)
{
	assert(NULL != data1);
	assert(NULL != data2);
	
	return ((*(const int *) data1) - (*(const int *) data2)); 
}

static int FindIfCmpFunc(const void *data1, const void *data2)
{
	assert(NULL != data1);
	assert(NULL != data2);
	
	if (2*(*(const int *) data2) == (*(const int *) data1)) /* TODO: magic? */
	{
		return IS_CMP_FUNC_EQUAL;
	}
	
	return IS_CMP_FUNC_NOT_EQUAL;
}

static int PrintSortedListElementOp(void *data, void *func_param)
{
	assert(NULL != data);
	assert(NULL != func_param);

	printf("%d ", *((int *) data));
	
	return SORTED_NO_ERROR;
}

static void PrintSortedList(sorted_list_ty *sorted_list, int func_param)
{
	sorted_list_iter_ty from;
	sorted_list_iter_ty to;
		
	assert(NULL != sorted_list);
	
	from = SortedListBegin(sorted_list);
	to = SortedListNext(SortedListEnd(sorted_list));

	SortedListForEach(from, to, PrintSortedListElementOp, (void *) &func_param);
	printf("\n");                                                           
}

static sorted_list_status_ty SortedListCreateDestroyTest(void)
{
	sorted_list_ty *sorted_list = SortedListCreate(IntCmpFunc);
	
	if (NULL == sorted_list)
	{
		printf("\n*** The creation of the sorted linked list failed ***\n");
			
		return SORTED_GENERAL_ERR;
	}
	
	SortedListDestroy(sorted_list);
	sorted_list = NULL; 
	
	return SORTED_NO_ERROR;
}

static sorted_list_status_ty SortedListPopFrontTest(void)
{
	sorted_list_ty *sorted_list = NULL;
	sorted_list_iter_ty runner;
	sorted_list_iter_ty loop_end;
	sorted_list_status_ty status = 0;	
	int elements_to_insert[] = {7, 8, 9, 10};
	int func_param = 0;
	size_t elements_arr_size = 4;
	size_t elements_to_pop = 2;
	size_t size_after_pop = 0;
	size_t i = 0;
	
	sorted_list = SortedListCreate(IntCmpFunc);
	
	if (NULL == sorted_list)
	{	
		return SORTED_GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		SortedListInsert(sorted_list, (void *) &elements_to_insert[i]);
	} 
	
	printf("\nThe size of the sorted list before the pop front is: %lu\n", 
	                                             		     elements_arr_size);
	printf("The elements in the sorted list are:\n");
	PrintSortedList(sorted_list, func_param); 

	for (i = 1; i <= elements_to_pop; ++i)
	{
		status = SortedListPopFront(sorted_list);
		size_after_pop = SortedListLength(sorted_list);

		if (SORTED_NO_ERROR != status ||
		                              (elements_arr_size - i) != size_after_pop)
		{
			fprintf(stderr, "\nSortedListPopFront() function doesn't work \
properly.\n");
			
			SortedListDestroy(sorted_list);
			sorted_list = NULL;
			
			return SORTED_GENERAL_ERR;
		}
	}

	runner = SortedListBegin(sorted_list);
	loop_end = SortedListNext(SortedListEnd(sorted_list));
	i = 2;
	
	while (!SortedListIterIsEqual(runner, loop_end))
	{
		if (elements_to_insert[i] != *((int *)SortedListGetData(runner)))
		{
			fprintf(stderr, "\nSortedListPopFront() function doesn't work \
properly.\n");
			
			SortedListDestroy(sorted_list);
			sorted_list = NULL;
			
			return SORTED_GENERAL_ERR;
		}	
		
		runner = SortedListNext(runner);
		++i;
	} 
	                                           		     	
	printf("\nThe size of the sorted list after pop front %lu elements is: %lu\
										  \n", elements_to_pop, size_after_pop);
	printf("The elements in the sorted list are:\n");
	PrintSortedList(sorted_list, func_param);                                           				
	                                             				
	SortedListDestroy(sorted_list);
	sorted_list = NULL;                                            				
	                                             				
	return SORTED_NO_ERROR;	
}

static sorted_list_status_ty SortedListPopBackTest(void)
{
	sorted_list_ty *sorted_list = NULL;
	sorted_list_iter_ty runner;
	sorted_list_iter_ty loop_end;
	sorted_list_status_ty status = 0;	
	int elements_to_insert[] = {7, 8, 9, 10};
	int func_param = 0;
	size_t elements_arr_size = 4;
	size_t elements_to_pop = 2;
	size_t size_after_pop = 0;
	size_t i = 0;
	
	sorted_list = SortedListCreate(IntCmpFunc);
	
	if (NULL == sorted_list)
	{	
		return SORTED_GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		SortedListInsert(sorted_list, (void *) &elements_to_insert[i]);
	} 
	
	
	printf("\nThe size of the sorted list before the pop back is: %lu\n", 
	                                             		     elements_arr_size);
	printf("The elements in the sorted list are:\n");
	PrintSortedList(sorted_list, func_param); 

	for (i = 1; i <= elements_to_pop; ++i)
	{
		status = SortedListPopBack(sorted_list);
		size_after_pop = SortedListLength(sorted_list);

		if (SORTED_NO_ERROR != status || 
		                              (elements_arr_size - i) != size_after_pop)
		{
			fprintf(stderr, "\nSortedListPopBack() function doesn't work \
properly.\n");
			
			SortedListDestroy(sorted_list);
			sorted_list = NULL;
			
			return SORTED_GENERAL_ERR;
		}
	}

	runner = SortedListBegin(sorted_list);
	loop_end = SortedListNext(SortedListEnd(sorted_list));
	i = 0;
	
	while (!SortedListIterIsEqual(runner, loop_end))
	{
		if (elements_to_insert[i] != *((int *)SortedListGetData(runner)))
		{
			fprintf(stderr, "\nSortedListPopBack() function doesn't work \
properly.\n");
			
			SortedListDestroy(sorted_list);
			sorted_list = NULL;
			
			return SORTED_GENERAL_ERR;
		}	
		
		runner = SortedListNext(runner);
		++i;
	} 
	                                           		     	
	printf("\nThe size of the list after pop back %lu elements is: %lu\n", 
	                                           elements_to_pop, size_after_pop);
	printf("The elements in the sorted list are:\n");
	PrintSortedList(sorted_list, func_param);                                           				
	                                             				
	SortedListDestroy(sorted_list);
	sorted_list = NULL;                                            				
	                                             				
	return SORTED_NO_ERROR;	
}

static sorted_list_status_ty SortedListIsEmptyTest(void)
{
	sorted_list_ty *sorted_list = NULL;
    int is_empty = IS_NOT_EMPTY_LIST;

	sorted_list = SortedListCreate(IntCmpFunc);

	if (NULL == sorted_list)
	{	
		return SORTED_GENERAL_ERR;
	}
	
    is_empty = SortedListIsEmpty(sorted_list);

	SortedListDestroy(sorted_list);
	sorted_list = NULL;
	
    if (IS_EMPTY_LIST != is_empty)
    {
    	fprintf(stderr, "\nSortedListIsEmpty() function doesn't work properly.\
    	\n");
    			
    	return SORTED_GENERAL_ERR;
    } 

    return SORTED_NO_ERROR;
}

static sorted_list_status_ty SortedListLengthTest(void)
{
	sorted_list_ty *sorted_list = NULL;
    int is_empty = 0;

	sorted_list = SortedListCreate(IntCmpFunc);

	if (NULL == sorted_list)
	{	
		return SORTED_GENERAL_ERR;
	}
	
    is_empty = SortedListLength(sorted_list);

	SortedListDestroy(sorted_list);
	sorted_list = NULL;
	
    if (IS_EMPTY_LENGTH != is_empty)
    {
    	fprintf(stderr, "\nSortedListLength() function doesn't work properly.\
    	\n");
    			
    	return SORTED_GENERAL_ERR;
    } 

    return SORTED_NO_ERROR;
}

static sorted_list_status_ty SortedListGetDataTest(void)
{
	sorted_list_ty *sorted_list = NULL;
    int head_element_data = 1;
    int head_get_data = 0;
    
	sorted_list = SortedListCreate(IntCmpFunc);

	if (NULL == sorted_list)
	{	
		return SORTED_GENERAL_ERR;
	}
    
	SortedListInsert(sorted_list, (void *) &head_element_data);
	head_get_data = *((int *)SortedListGetData(SortedListBegin(sorted_list)));
	
	SortedListDestroy(sorted_list);
	sorted_list = NULL;
		
	if (head_element_data != head_get_data)
	{
		fprintf(stderr, "\nSortedListGetData() function doesn't work \
properly.\n");
		
    	return SORTED_GENERAL_ERR;
    }
	
	return SORTED_NO_ERROR;
}

static sorted_list_status_ty SortedListInsertTest(void)
{
	sorted_list_ty *sorted_list = NULL;
	sorted_list_iter_ty runner;
	sorted_list_iter_ty loop_end;
	sorted_list_iter_ty status;
	int elements_to_insert[] = {1, 7, 4, 3};
	int insert_expected[] = {1, 3, 4, 7};
	int func_param = 0;
	size_t elements_arr_size = 4;
	size_t size_after_insert = 0;
	size_t i = 0;
	
	sorted_list = SortedListCreate(IntCmpFunc);

	if (NULL == sorted_list)
	{	
		return SORTED_GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		status = SortedListInsert(sorted_list, (void *) &elements_to_insert[i]);
		size_after_insert = SortedListLength(sorted_list);
				
		if (NULL == SortedListGetData(status) || (i + 1) != size_after_insert)
		{
			fprintf(stderr, "\nSortedListInsert() function doesn't work \
properly.\n");
			
			SortedListDestroy(sorted_list);
			sorted_list = NULL;
			
			return SORTED_GENERAL_ERR;
		}
	} 
	
	runner = SortedListBegin(sorted_list);
	loop_end = SortedListNext(SortedListEnd(sorted_list));
	i = 0;
	
	while (!SortedListIterIsEqual(runner, loop_end))
	{
		if (insert_expected[i] != *((int *)SortedListGetData(runner)))
		{
			fprintf(stderr, "\nSortedListInsert() function doesn't work \
properly.\n");
			
			SortedListDestroy(sorted_list);
			sorted_list = NULL;
			
			return SORTED_GENERAL_ERR;
		}	
		
		runner = SortedListNext(runner);
		++i;
	} 
	
	printf("\nThe size of the sorted list after insert %lu elements is: %lu\n", 
	                                      elements_arr_size, size_after_insert);
	printf("The elements in the sorted list are:\n");
	PrintSortedList(sorted_list, func_param);
      
	SortedListDestroy(sorted_list);
	sorted_list = NULL;

	return SORTED_NO_ERROR;	
}

static sorted_list_status_ty SortedListRemoveTest(void)
{
	sorted_list_ty *sorted_list = NULL;
    sorted_list_iter_ty runner;
    sorted_list_iter_ty loop_end;
	int elements_to_insert[] = {1, 2, 3, 4};
	int expected_list_after_remove[] = {1, 2, 4};
	int func_param = 0;
	size_t elements_arr_size = 4;
	size_t size_after_remove = 0;
	size_t i = 0;

	sorted_list = SortedListCreate(IntCmpFunc);

	if (NULL == sorted_list)
	{
		return SORTED_GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		SortedListInsert(sorted_list, (void *) &elements_to_insert[i]);
	} 
	
	printf("\nThe size of the sorted list before the remove is: %lu\n", 
	                                             		     elements_arr_size);
	printf("The elements in the sorted list are:\n");
	PrintSortedList(sorted_list, func_param);
	
	SortedListRemove(sorted_list, SortedListPrev(SortedListEnd(sorted_list)));
	size_after_remove = SortedListLength(sorted_list);
	
	runner = SortedListBegin(sorted_list);
	loop_end = SortedListNext(SortedListEnd(sorted_list));
	i = 0;
	
	while (!SortedListIterIsEqual(runner, loop_end))
	{
		if (expected_list_after_remove[i] !=*((int *)SortedListGetData(runner)))
		{
			fprintf(stderr,"\nSortedListRemove() function doesn't work \
properly.\n");		
		
			SortedListDestroy(sorted_list);
			sorted_list = NULL;
			
			return SORTED_GENERAL_ERR;	
		}
		
		runner = SortedListNext(runner);
		++i;
	}
	
	printf("\nThe size of the sorted list after remove 1 element is: %lu\n", 
	                                                         size_after_remove);
	printf("The elements in the sorted list are:\n");
	PrintSortedList(sorted_list, func_param);
	
	SortedListDestroy(sorted_list);
	sorted_list = NULL;
		                                                           
	return SORTED_NO_ERROR;	                                                        
}

static sorted_list_status_ty SortedListIterIsEqualTest(void)
{
	sorted_list_ty *sorted_list = NULL;
    sorted_list_iter_ty iter1;
    sorted_list_iter_ty iter2;
    int head_element_data = 1;
    
    sorted_list = SortedListCreate(IntCmpFunc);

	if (NULL == sorted_list)
	{
		return SORTED_GENERAL_ERR;
	}
	
	SortedListInsert(sorted_list, (void *) &head_element_data);
	
	iter1 = SortedListBegin(sorted_list);
	iter2 = SortedListBegin(sorted_list);
		
	if (!SortedListIterIsEqual(iter1, iter2))
	{
		fprintf(stderr,"SortedListIterIsEqual() function doesn't work \
properly.\n");

		SortedListDestroy(sorted_list);
		sorted_list = NULL;	
		
		return SORTED_GENERAL_ERR;
	}
	
	SortedListDestroy(sorted_list);
	sorted_list = NULL;	
	
	return SORTED_NO_ERROR;
}

static sorted_list_status_ty SortedListFindTest(void)
{
	sorted_list_ty *sorted_list = NULL;
	sorted_list_iter_ty from;
	sorted_list_iter_ty to;
	sorted_list_iter_ty element_to_find_iter;
	sorted_list_iter_ty find_func_iter;
	int elements_to_insert[] = {1, 2, 3, 4};
	int element_to_find_data = 3;
	size_t elements_arr_size = 4;
	size_t i = 0;

	sorted_list = SortedListCreate(IntCmpFunc);

	if (NULL == sorted_list)
	{
		return SORTED_GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		SortedListInsert(sorted_list, (void *) &elements_to_insert[i]);
	} 
	
	from = SortedListBegin(sorted_list);
	to = SortedListNext(SortedListEnd(sorted_list));
	element_to_find_iter = SortedListPrev(SortedListEnd(sorted_list));
	find_func_iter = SortedListFind(sorted_list, from, to, 
	                                             (void *)&element_to_find_data);
	
	if (!SortedListIterIsEqual(find_func_iter, element_to_find_iter))
	{
		fprintf(stderr,"SortedListFind() function doesn't work properly.\n");

		SortedListDestroy(sorted_list);
		sorted_list = NULL;	
		
		return SORTED_GENERAL_ERR;
	}
	
		
	SortedListDestroy(sorted_list);
	sorted_list = NULL;
	
	return SORTED_NO_ERROR;
}

static sorted_list_status_ty SortedListFindIfTest(void)
{
	sorted_list_ty *sorted_list = NULL;
	sorted_list_iter_ty from;
	sorted_list_iter_ty to;
	sorted_list_iter_ty element_to_find_iter;
	sorted_list_iter_ty find_func_iter;
	int elements_to_insert[] = {1, 2, 4, 6};
	int element_to_find_data = 3;
	size_t elements_arr_size = 4;
	size_t i = 0;

	sorted_list = SortedListCreate(IntCmpFunc);

	if (NULL == sorted_list)
	{
		return SORTED_GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		SortedListInsert(sorted_list, (void *) &elements_to_insert[i]);
	} 
	
	from = SortedListBegin(sorted_list);
	to = SortedListNext(SortedListEnd(sorted_list));
	element_to_find_iter = SortedListEnd(sorted_list);
	find_func_iter = SortedListFindIf(from, to, FindIfCmpFunc, 
	                                             (void *)&element_to_find_data);
	
	if (!SortedListIterIsEqual(find_func_iter, element_to_find_iter))
	{
		fprintf(stderr,"SortedListFindIf() function doesn't work properly.\n");

		SortedListDestroy(sorted_list);
		sorted_list = NULL;	
		
		return SORTED_GENERAL_ERR;
	}
	
		
	SortedListDestroy(sorted_list);
	sorted_list = NULL;
	
	return SORTED_NO_ERROR;
}

static sorted_list_status_ty SortedListMergeTest(void)
{
	sorted_list_ty *dest = NULL;
	sorted_list_ty *src = NULL;
	sorted_list_iter_ty runner;
	int dest_elements_to_insert[] = {1, 2, 4, 6};
	int src_elements_to_insert[] = {3, 5, 7, 8};
	int expected_list_after_merge[] = {1, 2, 3, 4, 5, 6, 7, 8};
	int func_param = 0;
	size_t dest_elements_arr_size = 4;
	size_t src_elements_arr_size = 4;
	size_t dest_size_after_merge = 0;
	size_t src_size_after_merge = 0;
	size_t i = 0;

	dest = SortedListCreate(IntCmpFunc);

	if (NULL == dest)
	{
		return SORTED_GENERAL_ERR;
	}
	
	src = SortedListCreate(IntCmpFunc);

	if (NULL == src)
	{
		return SORTED_GENERAL_ERR;
	}
	
	for (i = 0; i < dest_elements_arr_size; ++i)
	{
		SortedListInsert(dest, (void *) &dest_elements_to_insert[i]);
	}
	
	printf("\nBefore the merge the elements in dest are:\n");
	PrintSortedList(dest, func_param);
	
	for (i = 0; i < src_elements_arr_size; ++i)
	{
		SortedListInsert(src, (void *) &src_elements_to_insert[i]);
	}
	
	printf("\nBefore the merge the elements in src are:\n");
	PrintSortedList(src, func_param);
	
	SortedListMerge(dest, src);
	
	dest_size_after_merge = SortedListLength(dest); 
	src_size_after_merge = SortedListLength(src);
	
	runner = SortedListBegin(dest);
	i = 0;
	
	while (!SortedListIterIsEqual(runner, SortedListNext(SortedListEnd(dest))))
	{
		if (expected_list_after_merge[i] !=*((int *)SortedListGetData(runner)))
		{
			fprintf(stderr,"\nSortedListMerge() function doesn't work \
properly.\n");	

			SortedListDestroy(dest);
			dest = NULL;	
		
			SortedListDestroy(src);
			src = NULL;
			
			return SORTED_GENERAL_ERR;	
		}
		
		runner = SortedListNext(runner);
		++i;
	}

	if ((dest_elements_arr_size + src_elements_arr_size)!= dest_size_after_merge
	    || 0 != src_size_after_merge)
	{
		fprintf(stderr,"\nSortedListMerge() function doesn't work \
properly.\n");	

		SortedListDestroy(dest);
		dest = NULL;	
		
		SortedListDestroy(src);
		src = NULL;
			
		return SORTED_GENERAL_ERR;	
	}
	
	printf("\nAfter the merge the elements in dest are:\n");
	PrintSortedList(dest, func_param);
	printf("\nAfter the merge the size of src is: %lu\n", src_size_after_merge);
	
			
	SortedListDestroy(dest);
	dest = NULL;	
		
	SortedListDestroy(src);
	src = NULL;
	
	return SORTED_NO_ERROR;
}

static sorted_list_status_ty AutomateTests(void)
{
	sorted_list_status_ty sorted_list_status = 0;
	int i = 0;

	sorted_list_test_function_ty tests[] = 
	{
		SortedListCreateDestroyTest,
		SortedListPopFrontTest,
		SortedListPopBackTest,
		SortedListLengthTest,
		SortedListIsEmptyTest,
		SortedListGetDataTest,
		SortedListInsertTest,
		SortedListRemoveTest,
		SortedListIterIsEqualTest,
		SortedListFindTest,
		SortedListFindIfTest,
		SortedListMergeTest
	};
	
	const char* test_func_names[] =
	{
        "SortedListCreateDestroyTest",
        "SortedListPopFrontTest",
        "SortedListPopBackTest",
        "SortedListLengthTest",
        "SortedListIsEmptyTest",
        "SortedListGetDataTest",
        "SortedListInsertTest",
		"SortedListRemoveTest",
		"SortedListIterIsEqualTest",
		"SortedListFindTest",
		"SortedListFindIfTest",
		"SortedListMergeTest"
    };

	for (i = 0; i < TESTS_NUM; ++i)
    {
    	printf("\nTest N.%d - %s:", i, test_func_names[i]);
        sorted_list_status = tests[i]();
        printf("\n-- Test function return value: %d --\n\n",sorted_list_status);
        
        if (SORTED_NO_ERROR != sorted_list_status)
        {
        	return SORTED_GENERAL_ERR;
        }
    } 

    return SORTED_NO_ERROR;
}
