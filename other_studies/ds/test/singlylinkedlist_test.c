#include <stdio.h> /* printf */
#include <stdio.h> /* fprintf */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "singlylinkedlist.h"

#define TESTS_NUM 25

typedef status_ty (*test_function_ty) (slist_ty *);

/*************************************************************************************** TODO: need to do tests without malloc!!!!! **************/
/*********** Automat Tests Functions **************/
static status_ty PrintSListElementAction(void *data, void *param);
static void PrintSList(slist_ty *list, int param);

static status_ty SListPopFrontTest(slist_ty *list);
static status_ty SListPushFrontTest(slist_ty *list);
static status_ty SListPushBackTest(slist_ty *list);
static status_ty SListPopBackTest(slist_ty *list);

static status_ty SListLengthTest(slist_ty *list);
static status_ty SListIsEmptyTest(slist_ty *list);

status_ty IsFindEqual(const void *left, const void *right);
static status_ty SListFindTest(slist_ty *list);

static status_ty SListBeginTest(slist_ty *list);
static status_ty SListSetDataTest(slist_ty *list);

static status_ty SListInsertAfterTest(slist_ty *list);
static status_ty SListInsertTest(slist_ty *list);
static status_ty SListRemoveTest(slist_ty *list);
static status_ty SListRemoveAfterTest(slist_ty *list);

static status_ty AutomatTests();

int main()
{
	status_ty status = 0;
	
	status = AutomatTests();
	
	if (SUCCESS != status)
	{
		printf("\n *** There is an ERROR in the program ***\n");
		
		return FAIL;
	}
	
	printf("\n *** All the tests works, the program work fine ***\n");
	
	return SUCCESS;
}

static status_ty PrintSListElementAction(void *data, void *param)
{
	assert(NULL != data);
	assert(NULL != param);

	printf("%d ", *((int *) data));
	
	return SUCCESS;
}

static void PrintSList(slist_ty *list, int param)
{
	assert(NULL != list);

	SListForEach(list, SListBegin(list), SListEnd(list),PrintSListElementAction,
	                                                           (void *) &param);
	printf("\n");                                                           
}

static status_ty SListPopFrontTest(slist_ty *list)
{
	status_ty status = 0;
	int param = 0;
	
	assert(NULL != list);
	
	status = SListPopFront(list);
	if (SUCCESS != status)
	{
		fprintf(stderr, "\nPopfront() function doesn't work properly.\n");
		
		return status;
	}
	
	printf("\nThe size of the list after the pop front is: %lu", 
	                                                         SListLength(list));
	
	printf("\nThe element of the list after pop front are:\n");
	PrintSList(list, param); 
	
	return SUCCESS;	
}

static status_ty SListPushFrontTest(slist_ty *list)
{
	status_ty status = 0;
	int param = 0;
	int element = 1;
	int *element_ptr = NULL;
	
	assert(NULL != list);
	
	element_ptr = (int *) malloc(sizeof(int));
	if (NULL == element_ptr)
	{
		fprintf(stderr, "\nThe memory allocation for the element failed.\n");
		
		return MEM_FAIL;
	}
	*element_ptr = element;
	
	status = SListPushFront(list, (void *) element_ptr);
	if (SUCCESS != status)
	{
		fprintf(stderr, "\nPushfront() function doesn't work properly.\n");
		free(element_ptr);
		element_ptr = NULL;
		
		return status;
	}
	
	printf("\nThe size of the list after the push front is: %lu", 
	                                                         SListLength(list));
	
	printf("\nThe element of the list after push front 1 are:\n");
	PrintSList(list, param); 

	return SUCCESS;	
}

static status_ty SListPushBackTest(slist_ty *list)
{
	status_ty status = 0;
	int param = 0;
	int element = 2;
	int *element_ptr = NULL;
	
	assert(NULL != list);
	
	element_ptr = (int *) malloc(sizeof(int));
	if (NULL == element_ptr)
	{
		fprintf(stderr, "\nThe memory allocation for the element failed.\n");
		
		return MEM_FAIL;
	}
	*element_ptr = element;
	
	status = SListPushBack(list, (void *) element_ptr);
	if (SUCCESS != status)
	{
		fprintf(stderr, "\nPushBack() function doesn't work properly.\n");
		free(element_ptr);
		element_ptr = NULL;
				
		return status;
	}
	
	printf("\nThe size of the list after the push back is: %lu", 
	                                                         SListLength(list));
	
	printf("\nThe element of the list after push back 2 are:\n");
	PrintSList(list, param);
	
	return SUCCESS;	
}

static status_ty SListPopBackTest(slist_ty *list)
{
	status_ty status = 0;
	int param = 0;
	
	assert(NULL != list);
	
	status = SListPopBack(list);
	if (SUCCESS != status)
	{
		fprintf(stderr, "\nPopBack() function doesn't work properly.\n");
		
		return status;
	}
	
	printf("\nThe size of the list after the pop back is: %lu", 
	                                                         SListLength(list));
	
	printf("\nThe element of the list after pop back are:\n");
	PrintSList(list, param);  
	
	return SUCCESS;	
}

static status_ty SListLengthTest(slist_ty *list)
{
	assert(NULL != list);
	
	printf("\nThe length of the list is: %lu\n", SListLength(list));

	return SUCCESS;
}

static status_ty SListIsEmptyTest(slist_ty *list)
{
	assert(NULL != list);
	
	if (1 == SListIsEmpty(list))
	{
		printf("\nThe list is empty.\n");
	}
	
	else
	{
		printf("\nThe list isn't empty.\n");	
	}
	
	return SUCCESS;
}

status_ty IsFindEqual(const void *left, const void *right)
{
	return (*(int *)left == *(int *)right);                                     
}

static status_ty SListFindTest(slist_ty *list)
{
	slist_node_ty *find_func_res = NULL;
	int element = 1;	                            

	assert(NULL != list);
	
	find_func_res = SListFind(list, IsFindEqual, (void *) &element);
	if (NULL == find_func_res)
	{
		printf("\nThe element with data 1 does not exist.\n");
		
		return SUCCESS;
	}
	
	printf("\nThe data in the first element with the data 1 is: %d", 
	                    				 *((int *)SListGetData(find_func_res)));
	printf("\nThe data in the next element is: %d\n", 
	                          *((int *)SListGetData(SListNext(find_func_res))));
	                                    
	return SUCCESS;
}

static status_ty SListBeginTest(slist_ty *list)
{
	assert(NULL != list);
	
	printf("\nThe data in the head of the list is: %d\n", 
	                                  *((int *)SListGetData(SListBegin(list))));
	
	return SUCCESS;
}

static status_ty SListSetDataTest(slist_ty *list)
{
	int param = 0;
	int data = 0;
	int *data_ptr = NULL;
	
	assert(NULL != list);
	
	data_ptr = (int *) malloc(sizeof(int));
	if (NULL == data_ptr)
	{
		fprintf(stderr, "\nThe memory allocation for the data failed.\n");
		
		return MEM_FAIL;
	}
	*data_ptr = data;
	
	
	SListSetData(SListBegin(list), (void *) data_ptr);
	printf("\nAfter set the data of the head to 0,\
 the elements in the list are:\n");
	PrintSList(list, param); 
	                                                    
	return SUCCESS;                                                          	
}

static status_ty SListInsertAfterTest(slist_ty *list)
{
	int param = 0;
	int data = 3;
	int *data_ptr = NULL;
	
	assert(NULL != list);
	
	data_ptr = (int *) malloc(sizeof(int));
	if (NULL == data_ptr)
	{
		fprintf(stderr, "\nThe memory allocation for the data failed.\n");
		
		return MEM_FAIL;
	}
	*data_ptr = data;
	
	SListInsertAfter(list, SListBegin(list), (void *) data_ptr);
	printf("\nAfter insert after the head element with data 3,\
 the elements are:\n");
	PrintSList(list, param); 
		                                                           
	return SUCCESS;	
}

static status_ty SListInsertTest(slist_ty *list)
{
	int param = 0;
	int data = 4;
	int *data_ptr = NULL;
	
	assert(NULL != list);
	
	data_ptr = (int *) malloc(sizeof(int));
	if (NULL == data_ptr)
	{
		fprintf(stderr, "\nThe memory allocation for the data failed.\n");
		
		return MEM_FAIL;
	}
	*data_ptr = data;
	
	SListInsert(list, SListNext(SListBegin(list)), (void *) data_ptr);
	printf("\nAfter insert before the next element from the head element with\
 data 4, the elements are:\n");
	PrintSList(list, param); 
		                                                           
	return SUCCESS;	
}

static status_ty SListRemoveTest(slist_ty *list)
{
	int param = 0;
	
	assert(NULL != list);
	
	SListRemove(list, SListNext(SListBegin(list)));
	printf("\nAfter remove the element next to the head,\
 the elements in the list are:\n");
	PrintSList(list, param); 
	                                                           
	return SUCCESS;                                                           
}

static status_ty SListRemoveAfterTest(slist_ty *list)
{
	int param = 0;

	assert(NULL != list);
	
	SListRemoveAfter(list, SListNext(SListBegin(list)));
	printf("\nAfter remove the element after the element next to the head,\
 the elements in the list are:\n");
	PrintSList(list, param); 
	                                                           
	return SUCCESS;                                                           
}

static status_ty AutomatTests()
{
	slist_ty *list = NULL;
	status_ty status = 0;
	int i = 0;

	test_function_ty tests[] = {
		SListIsEmptyTest,
		SListLengthTest,
		SListPushFrontTest,
		SListPushFrontTest,
		SListPushBackTest,
		SListPushBackTest, 
		SListIsEmptyTest,
		SListLengthTest,
		SListPopBackTest,
		SListSetDataTest,
		SListBeginTest,
		SListFindTest,
		SListInsertTest,
		SListInsertAfterTest,
		SListRemoveTest,
		SListRemoveAfterTest,
		SListFindTest,
		SListPopFrontTest,
		SListBeginTest,
		SListPopBackTest,
		SListIsEmptyTest,
		SListPopFrontTest,
		SListIsEmptyTest,
		SListPushBackTest,
		SListPushBackTest  };
	
	list = SListCreate();
	
	if (NULL == list)
	{
		printf("\n*** The creation of the list failed ***\n");
			
		return FAIL;
	}
	
	else
	{
		printf("\n*** The creation of the list success ***\n");
	}
	
	for (i = 0; i < TESTS_NUM; ++i)
    {
    	printf("\nTest N.%d:", i);
        status = tests[i](list);
        if (FAIL == status)
        {
        	return FAIL;
        }
    }
    
 	SListDestroy(list);
	list = NULL;   

    return SUCCESS;
}
