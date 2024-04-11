#include <assert.h> /* assert */
#include <stdio.h> /* printf */
#include <stdio.h> /* fprintf */

#include "doublylinkedlist.h"

#define TESTS_NUM 13

enum {IS_NOT_EMPTY_LIST = 0, IS_EMPTY_LENGTH = 0, IS_EMPTY_LIST = 1};

typedef dlist_status_ty (*dlist_test_function_ty)();

/*********** Automate Tests Functions **************/
static int PrintDListElementAction(void *data, void *param);
static void PrintDList(dlist_ty *dlist, int param);

static dlist_status_ty DListCreateDestroyTest(void);

static dlist_status_ty DListIsEmptyTest(void);
static dlist_status_ty DListLengthTest(void);

static dlist_status_ty DListPushFrontTest(void);
static dlist_status_ty DListPushBackTest(void);
static dlist_status_ty DListPopFrontTest(void);
static dlist_status_ty DListPopBackTest(void);

static dlist_status_ty DListGetDataTest(void);
static dlist_status_ty DListSetDataTest(void);

static dlist_status_ty DListInsertTest(void);
static dlist_status_ty DListRemoveTest(void);

int IsMatch(const void *left, const void *right);
static int PrintDListElementAddressAction(void *data, void *param);
static dlist_status_ty DListMultiFindTest(void);
static dlist_status_ty DListSpliceTest(void);

static dlist_status_ty AutomateTests(void);

int main()
{
	dlist_status_ty dlist_status = 0;
	
	dlist_status = AutomateTests();
	
	if (NO_ERROR != dlist_status)
	{
		printf("\n *** There is an ERROR in the program ***\n");
		
		return GENERAL_ERR;
	}
	
	printf("\n *** All the tests passed, the program work fine ***\n");
	
	return NO_ERROR;
}

static int PrintDListElementAction(void *data, void *param)
{
	assert(NULL != data);
	assert(NULL != param);

	printf("%d ", *((int *) data));
	
	return NO_ERROR;
}

static void PrintDList(dlist_ty *dlist, int param)
{
	assert(NULL != dlist);

	DListForEach(dlist, DListBegin(dlist), DListEnd(dlist),
	                                   PrintDListElementAction,(void *) &param);
	printf("\n");                                                           
}

static dlist_status_ty DListCreateDestroyTest(void)
{
	dlist_ty *dlist = DListCreate();
	
	if (NULL == dlist)
	{
		printf("\n*** The creation of the doubly linked list failed ***\n");
			
		return GENERAL_ERR;
	}
		    
 	DListDestroy(dlist);
	dlist = NULL;
	
	return NO_ERROR; 
}

static dlist_status_ty DListIsEmptyTest(void)
{
	dlist_ty *dlist = NULL;
    int is_empty = IS_NOT_EMPTY_LIST;

	dlist = DListCreate();

	if (NULL == dlist)
	{
		return GENERAL_ERR;
	}
	
    is_empty = DListIsEmpty(dlist);

	DListDestroy(dlist);
	dlist = NULL;
	
    if (IS_EMPTY_LIST != is_empty)
    {
    	fprintf(stderr, "\nDListIsEmpty() function doesn't work properly.\n");
    			
    	return GENERAL_ERR;
    } 

    return NO_ERROR;
}

static dlist_status_ty DListLengthTest(void)
{
    dlist_ty *dlist =  NULL;
    size_t dlist_length = 0;
    
	dlist = DListCreate();

	if (NULL == dlist)
	{
		return GENERAL_ERR;
	}
	
    dlist_length = DListLength(dlist);

	DListDestroy(dlist);
	dlist = NULL;
	    	
    if(IS_EMPTY_LENGTH != dlist_length)
    {
    	fprintf(stderr, "\nDListLength() function doesn't work properly.\n");
    
    	return GENERAL_ERR;
    }

    return NO_ERROR;
}

static dlist_status_ty DListPushFrontTest(void)
{
	dlist_ty* dlist = NULL;
	dlist_status_ty status = 0;
	int elements_to_push[] = {1, 2, 3};
	int param = 0;
	size_t elements_arr_size = 3;
	size_t i = 0;
	
	dlist = DListCreate();

	if (NULL == dlist)
	{
		return GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		status = DListPushFront(dlist, (void *) &elements_to_push[i]);
		
		if (NO_ERROR != status ||
		    elements_to_push[i] != *((int *)DListGetData(DListBegin(dlist))))
		{
			fprintf(stderr, "\nDListPush() function doesn't work properly.\n");
			
			DListDestroy(dlist);
			dlist = NULL;
			
			return GENERAL_ERR;
		}
		
		printf("\nThe size of the list after the push front is: %lu", 
			                                                DListLength(dlist));
		
		printf("\nThe elements of the list after push front %d are:\n",
									                       elements_to_push[i]);
		PrintDList(dlist, param);
	} 
      
    DListDestroy(dlist);
    dlist = NULL;

	return NO_ERROR;	
}

static dlist_status_ty DListPushBackTest(void)
{
	dlist_ty* dlist = NULL;
	dlist_status_ty status = 0;
	int elements_to_push[] = {4, 5, 6};
	int param = 0;
	size_t elements_arr_size = 3;
	size_t i = 0;
	
	dlist = DListCreate();

	if (NULL == dlist)
	{	
		return GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		status = DListPushBack(dlist, (void *) &elements_to_push[i]);
		
		if (NO_ERROR != status ||
		    elements_to_push[i] != *((int *)DListGetData(DListEnd(dlist))))
		{
			fprintf(stderr, "\nDListPushBack() function doesn't work properly.\
			\n");
			
			DListDestroy(dlist);
			dlist = NULL;
			
			return GENERAL_ERR;
		}
		
		printf("\nThe size of the list after the push back is: %lu", 
			                                                DListLength(dlist));
		
		printf("\nThe elements of the list after push back %d are:\n",
														   elements_to_push[i]);
		PrintDList(dlist, param);
	} 
      
    DListDestroy(dlist);
    dlist = NULL;

	return NO_ERROR;	
}

static dlist_status_ty DListPopFrontTest(void)
{
    dlist_ty *dlist = NULL;
    dlist_status_ty status = 0;
	int elements_to_push[] = {7, 8, 9};
	int param = 0;
	size_t elements_arr_size = 3;
	size_t i = 0;
	
	dlist = DListCreate();

	if (NULL == dlist)
	{	
		return GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		DListPushBack(dlist, (void *) &elements_to_push[i]);
	} 
	
	printf("\nThe element of the list after the push are:\n");
	PrintDList(dlist, param);
	
	for (i = 0; i < elements_arr_size - 1; ++i)
	{
		status = DListPopFront(dlist);
		
		if (NO_ERROR != status ||
		    elements_to_push[i+1] != *((int *)DListGetData(DListBegin(dlist))))		
		{
			fprintf(stderr, "\nDListPopFront() function doesn't work properly.\
			\n");
			
			DListDestroy(dlist);
			dlist = NULL;

			return GENERAL_ERR;
		}
		
		printf("\nThe size of the list after the pop front is: %lu", 
			                                                DListLength(dlist));
		
		printf("\nThe element of the list after pop front are:\n");
		PrintDList(dlist, param);
	} 
	
	DListDestroy(dlist);
	dlist = NULL;
	
	return NO_ERROR;
}

static dlist_status_ty DListPopBackTest(void)
{
    dlist_ty *dlist = NULL;
    dlist_status_ty status = 0;
	int elements_to_push[] = {10, 11, 12};
	int param = 0;
	size_t elements_arr_size = 3;
	size_t i = 0;
	
	dlist = DListCreate();

	if (NULL == dlist)
	{	
		return GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		DListPushBack(dlist, (void *) &elements_to_push[i]);
	} 
	
	printf("\nThe element of the list after the push are:\n");
	PrintDList(dlist, param);
	
	for (i = elements_arr_size; i > 1; --i)
	{
		status = DListPopBack(dlist);
		
		if (NO_ERROR != status ||
		    elements_to_push[i - 2] != *((int *)DListGetData(DListEnd(dlist))))
		{
			fprintf(stderr, "\nDListPopBack() function doesn't work properly.\
			\n");
			
			DListDestroy(dlist);
			dlist = NULL;

			return GENERAL_ERR;
		}
		
		printf("\nThe size of the list after the pop back is: %lu", 
			                                                DListLength(dlist));
		
		printf("\nThe element of the list after pop back are:\n");
		PrintDList(dlist, param);
	} 
	
	DListDestroy(dlist);
	dlist = NULL;
	
	return NO_ERROR;
}

static dlist_status_ty DListGetDataTest(void)
{
    dlist_ty *dlist = NULL;
    int head_element_data = 1;
    
    dlist = DListCreate();

	if (NULL == dlist)
	{	
		return GENERAL_ERR;
	}
    
	DListPushBack(dlist, (void *) &head_element_data);
	
	if (head_element_data != *((int *)DListGetData(DListBegin(dlist))))
	{
		fprintf(stderr, "\nDListGetData() function doesn't work properly.\n");
		
		DListDestroy(dlist);
		dlist = NULL;
		
    	return GENERAL_ERR;
    }
	
	DListDestroy(dlist);
	dlist = NULL;
	
	return NO_ERROR;
}

static dlist_status_ty DListSetDataTest(void)
{
    dlist_ty *dlist = NULL;
    int head_element_data = 1;
    int data_to_set = 0;
    
    dlist = DListCreate();

	if (NULL == dlist)
	{	
		return GENERAL_ERR;
	}
	
	DListPushBack(dlist, (void *) &head_element_data);
	DListSetData(DListBegin(dlist), (void *) &data_to_set);
	
	if (data_to_set != *((int *)DListGetData(DListBegin(dlist))))
	{
		fprintf(stderr, "\nDListSetData() function doesn't work properly.\n");
		
		DListDestroy(dlist);
		dlist = NULL;
		
    	return GENERAL_ERR;
    }
	
	DListDestroy(dlist);
	dlist = NULL;
	
	return NO_ERROR;                                                          	
}

static dlist_status_ty DListInsertTest(void)
{
    dlist_ty *dlist = NULL;
	int elements_to_push[] = {13, 14, 15};
	int data_to_insert = 1;
	int param = 0;
	size_t elements_arr_size = 3;
	size_t i = 0;

	dlist = DListCreate();

	if (NULL == dlist)
	{	
		return GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		DListPushBack(dlist, (void *) &elements_to_push[i]);
	}
	
	printf("\nThe element of the list after the push are:\n");
	PrintDList(dlist, param);
	
	DListInsert(dlist, DListNext(DListBegin(dlist)), (void *) &data_to_insert);
	
	if (data_to_insert != *(int *)DListGetData(DListNext(DListBegin(dlist))))
	{
		fprintf(stderr,"\nDListInsert() function doesn't work properly.\n");		

    	DListDestroy(dlist);
    	dlist = NULL;
    	
    	return GENERAL_ERR;	
	}
	
	printf("\nAfter insert before the next element from the head, element with\
 data %d, the elements are:\n", data_to_insert);
	PrintDList(dlist, param); 
	
	DListDestroy(dlist);
	dlist = NULL;
		                                                           
	return NO_ERROR;	
}

static dlist_status_ty DListRemoveTest(void)
{
    dlist_ty *dlist = NULL;
    dlist_iter_ty runner;
	int elements_to_push[] = {16, 17, 18, 19};
	int expected_list_after_remove[] = {16, 17, 19};
	int param = 0;
	size_t elements_arr_size = 4;
	size_t i = 0;

	dlist = DListCreate();

	if (NULL == dlist)
	{	
		return GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		DListPushBack(dlist, (void *) &elements_to_push[i]);
	}
	
	printf("\nThe element of the list after the push are:\n");
	PrintDList(dlist, param);
	
	DListRemove(dlist, DListPrev(DListEnd(dlist)));
	
	runner = DListBegin(dlist);
	i = 0;
	
	while (runner.node != DListNext(DListEnd(dlist)).node)
	{
		if (expected_list_after_remove[i] != *((int *)DListGetData(runner)))
		{
			fprintf(stderr,"\nDListRemove() function doesn't work properly.\n");		
		
			DListDestroy(dlist);
			dlist = NULL;
			
			return GENERAL_ERR;	
		}
		
		runner = DListNext(runner);
		++i;
	}
	
	printf("\nAfter remove the element prev to the tail,\
 the elements in the list are:\n");
	PrintDList(dlist, param); 
	
	DListDestroy(dlist);
	dlist = NULL;
		                                                           
	return NO_ERROR;	                                                        
}

int IsMatch(const void *left, const void *right)
{
	return (*(int *)left == *(int *)right);                                     
}

static int PrintDListElementAddressAction(void *data, void *param)
{
	assert(NULL != data);
	assert(NULL != param);

	printf("%p ", data);
	
	return NO_ERROR;
}

static dlist_status_ty DListMultiFindTest(void)
{
    dlist_ty *dlist = NULL;
    dlist_ty *output = NULL;
    dlist_iter_ty runner;
	int elements_to_push[] = {1, 0, 1, 1};
	int multifind_expected[] = {1, 1, 1};
	int element_to_find = 1;
	int param = 0;
	size_t elements_arr_size = 4;
	size_t i = 0;

	dlist = DListCreate();

	if (NULL == dlist)
	{	
		return GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		DListPushBack(dlist, (void *) &elements_to_push[i]);
	}
	
	output = DListCreate();
	
	if (NULL == output)
	{	
		return GENERAL_ERR;
	}
	
	printf("\n\nThe dlist elements are:\n");
	PrintDList(dlist, param); 
	printf("The dlist elements data addresses are:\n");	
	DListForEach(dlist, DListBegin(dlist), DListEnd(dlist),
	                            PrintDListElementAddressAction,(void *) &param);
	                            
	DListMultiFind(dlist, IsMatch, (void *) &element_to_find, output);
	
	runner = DListBegin(output);
	i = 0;
	
	while (runner.node != DListNext(DListEnd(output)).node)
	{
		if (multifind_expected[i] != *((int *)DListGetData(runner)))
		{
			fprintf(stderr,"\nDListMultiFind() function doesn't work properly.\
			\n");		
			
			DListDestroy(output);
			output = NULL;
			
			DListDestroy(dlist);
			dlist = NULL;
			
			return GENERAL_ERR;	
		}
		
		runner = DListNext(runner);
		++i;
	}
	
	printf("\n\nThe output list elements are:\n");
	PrintDList(output, param); 
	printf("The output elements data addresses are:\n");	
	DListForEach(output, DListBegin(output), DListEnd(output),
	                            PrintDListElementAddressAction,(void *) &param);
	printf("\n");
	
	DListDestroy(output);
	output = NULL;
	
	DListDestroy(dlist);
	dlist = NULL;
	
	return NO_ERROR;
}

static dlist_status_ty DListSpliceTest(void)
{
	dlist_ty *dlist = NULL;
	dlist_iter_ty from;
	dlist_iter_ty to;
	dlist_iter_ty where;
    dlist_iter_ty runner;
	int elements_to_push[] = {20, 21, 22, 23, 24, 25};
	int splice_expected[] ={20, 24, 21, 22, 23, 25};
	int param = 0;
	size_t elements_arr_size = 6;
	size_t i = 0;

	dlist = DListCreate();

	if (NULL == dlist)
	{	
		return GENERAL_ERR;
	}
	
	for (i = 0; i < elements_arr_size; ++i)
	{
		DListPushBack(dlist, (void *) &elements_to_push[i]);
	}
	
	printf("\nThe element of the list after the push are:\n");
	PrintDList(dlist, param);
	
	from = DListNext(DListBegin(dlist));
	to = DListNext(DListNext(from));
	where = DListPrev(DListEnd(dlist));
	
	DListSplice(from, to, where);
	
	runner = DListBegin(dlist);
	i = 0;
	
	while (runner.node != DListNext(DListEnd(dlist)).node)
	{
		if (splice_expected[i] != *((int *)DListGetData(runner)))
		{
			fprintf(stderr,"\nDListSplice() function doesn't work properly.\n");		
		
			DListDestroy(dlist);
			dlist = NULL;
			
			return GENERAL_ERR;	
		}
		
		runner = DListNext(runner);
		++i;
	}
	
	printf("\nAfter splice from element in index 1 to element in index 3, after\
'where' in index 4, the elements are:\n");
	PrintDList(dlist, param); 
	
	DListDestroy(dlist);
	dlist = NULL;
		                                                           
	return NO_ERROR;
}

static dlist_status_ty AutomateTests(void)
{
	dlist_status_ty dlist_status = 0;
	int i = 0;

	dlist_test_function_ty tests[] = 
	{
	    DListCreateDestroyTest,
        DListPushFrontTest,
        DListPushBackTest,
        DListPopFrontTest,
        DListPopBackTest,
        DListLengthTest,
        DListIsEmptyTest,
        DListGetDataTest,
        DListSetDataTest,
        DListInsertTest,
        DListRemoveTest,
        DListMultiFindTest,
        DListSpliceTest
	};
	
	const char* test_func_names[] =
	{
        "DListCreateDestroyTest",
        "DListPushFrontTest",
        "DListPushBackTest",
        "DListPopFrontTest",
        "DListPopBackTest",
        "DListLengthTest",
        "DListIsEmptyTest",
        "DListGetDataTest",
        "DListSetDataTest",
        "DListInsertTest",
        "DListRemoveTest",
        "DListMultiFindTest",
        "DListSpliceTest"
    };

	
	for (i = 0; i < TESTS_NUM; ++i)
    {
    	printf("\nTest N.%d - %s:", i, test_func_names[i]);
        dlist_status = tests[i]();
        printf("\n-- Test function return value: %d --\n\n", dlist_status);
        
        if (NO_ERROR != dlist_status)
        {
        	return GENERAL_ERR;
        }
    }  

    return NO_ERROR;
}
