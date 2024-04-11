/* Version: 1.0
				1.1 Add automat tests */
#include <stdio.h> /* printf, scanf */
#include <assert.h> /* assert */

#include "vector.h"

#define CAPACITY 5
#define ELEMENT_SIZE 4
#define TESTS_NUM 16

enum options_ty {EXIT, PRINT, GET_SIZE, GET_CAP, IS_EMPTY, RESERVE, FIT_SHRINK,
                 GET_VALUE, SET_VALUE, PUSH_BACK, POP_BACK};
                 
typedef status_ty (*test_function_ty) (vector_ty *);

/*********** User Input Tests ******************/
static status_ty VectorIsEmptyTest(vector_ty *vector);
static status_ty VectorGetSizeTest(vector_ty *vector); 
static status_ty VectorGetMaxCapacityTest(vector_ty *vector);
static status_ty VectorReserveTest(vector_ty *vector);
static status_ty VectorShrinkToFitTest(vector_ty *vector);
static status_ty VectorGetValueTest(vector_ty *vector);
static status_ty VectorSetValueTest(vector_ty *vector);
static status_ty VectorPushBackTest(vector_ty *vector);
static status_ty VectorPopBackTest(vector_ty *vector);
static status_ty UserTestsMenu();

/************* Automat Tests *********************/
static status_ty AutomatResereveTest(vector_ty *vector);
static status_ty AutomatGetValueTest(vector_ty *vector);
static status_ty AutomatSetValueTest(vector_ty *vector);
static status_ty AutomatPushBackTest(vector_ty *vector);
static status_ty AutomatTests();

int main()
{
	status_ty status = 0;
	
	/*status += UserTestsMenu();*/
	status += AutomatTests();
	
	return ((SUCCESS == status) ? SUCCESS : FAIL);
}

/*********** User Input Tests ******************/
static status_ty VectorIsEmptyTest(vector_ty *vector)
{
	assert(NULL != vector);
	
	if (1 == VectorIsEmpty(vector))
	{
		printf("\nThe vector is empty.\n");
	}
	
	else
	{
		printf("\nThe vector isn't empty.\n");	
	}
	
	return SUCCESS;
}

static status_ty VectorGetSizeTest(vector_ty *vector)
{
	assert(NULL != vector);
	
	printf("\nThe vector size is: %lu\n", VectorGetSize(vector));
	
	return SUCCESS;
}

static status_ty VectorGetMaxCapacityTest(vector_ty *vector)
{
	printf("\nThe vector capacity is: %lu\n", VectorGetMaxCapacity(vector));
	
	return SUCCESS;
}

static status_ty VectorReserveTest(vector_ty *vector)
{
	size_t new_capacity = 0;
	status_ty status = 0;
	
	assert(NULL != vector);
	
	printf("\nEnter the reserve capacity: ");
	scanf("%lu", &new_capacity);
	
	status = VectorReserve(vector, new_capacity);
	printf("After reserve the capacity is: %lu\n",
	                                              VectorGetMaxCapacity(vector));
	
	return ((SUCCESS == status) ? SUCCESS : FAIL);
}

static status_ty VectorShrinkToFitTest(vector_ty *vector)
{
	status_ty status = 0;
	
	assert(NULL != vector);
	
	printf("\nAfter shrink:\n");
	status = VectorShrinkToFit(vector);
	printf("The capacity after the shrink is: %lu\n", 
			                                      VectorGetMaxCapacity(vector));
	PrintIntVector(vector);	
	
	return ((SUCCESS == status) ? SUCCESS : FAIL);
}

static status_ty VectorGetValueTest(vector_ty *vector)
{
	size_t index = 0;
	
	assert(NULL != vector);
	
	printf("\nEnter the wanted index to get: ");
	scanf("%lu", &index);
	
	printf("The value in this index is: %d\n",
	                                      *(int*)VectorGetValue(vector, index));
	                                      
	return SUCCESS;
}

static status_ty VectorSetValueTest(vector_ty *vector)
{
	size_t index = 0;
	int val = 0;
	
	assert(NULL != vector);
	
	printf("\nEnter the wanted index to set: ");
	scanf("%lu", &index);
	printf("Enter the wanted value to set: ");
	scanf("%d", &val);
	
	VectorSetValue(vector, index, &val);
	printf("After set:\n");
	PrintIntVector(vector);
	
	return SUCCESS;	
}

static status_ty VectorPushBackTest(vector_ty *vector)
{
	int val = 0;
	status_ty status = 0;
	
	assert(NULL != vector);
	
	printf("\nEnter the wanted value to push: ");
	scanf("%d", &val);
	
	status = VectorPushBack(vector, &val);
	printf("The size after the push is: %lu\n", VectorGetSize(vector));
	printf("The capacity after the push is: %lu\n",
	                                              VectorGetMaxCapacity(vector));
	PrintIntVector(vector);
	
	return ((SUCCESS == status) ? SUCCESS : FAIL);
}

static status_ty VectorPopBackTest(vector_ty *vector)
{
	status_ty status = 0;
	
	assert(NULL != vector);
	
	status = VectorPopBack(vector);
	printf("\nThe size after the pop is: %lu\n", VectorGetSize(vector));
	printf("The capacity after the pop is: %lu\n",VectorGetMaxCapacity(vector));
	PrintIntVector(vector);
	
	return ((SUCCESS == status) ? SUCCESS : FAIL);
}

static status_ty UserTestsMenu()
{
	vector_ty *vector = NULL;
	int status = 0;
	size_t element_size = 0;
	size_t capacity = 0;
	int choice = 0;

	printf("\n --- Tests Menu: ---\n");
	printf("\n1. print\n");
	printf("2. get_size\n");
	printf("3. get_capacity\n");
	printf("4. is_empty\n");
	printf("5. reserve\n");
	printf("6. fit_shrink\n");
	printf("7. get_value\n");
	printf("8. set_value\n");
	printf("9. push_back\n");
	printf("10. pop_back\n");
	printf("0. Exit\n\n");
	
	printf("Enter vector max size (capacity): ");
	scanf("%lu", &capacity);
	
	printf("Enter size of each element: ");
	scanf("%lu", &element_size);
	
	vector = VectorCreate(capacity, element_size);
	
	if (NULL == vector)
	{
		printf("\n* The creation of the vector failed *\n");
			
		return FAIL;
	}
	
	else
	{
		printf("\n* The creation of the vector success *\n");
	}
	
	choice = 1;

	while (choice != EXIT)
	{
		printf("\nEnter your choice: ");
		scanf("%d", &choice);
		switch (choice)
		{
		case PRINT:
			PrintIntVector(vector);
			break;
					
		case GET_SIZE:
			status += VectorGetSizeTest(vector);
			break;
			
		case GET_CAP:
			status += VectorGetMaxCapacityTest(vector);
			break;
			
		case IS_EMPTY:
			status += VectorIsEmptyTest(vector);
			break;
			
		case RESERVE:
			status += VectorReserveTest(vector);
			break;
			
		case FIT_SHRINK:
			status += VectorShrinkToFitTest(vector);
			break;
			
		case GET_VALUE:
			status += VectorGetValueTest(vector);
			break;
			
		case SET_VALUE:
			status += VectorSetValueTest(vector);
			break;
			
		case PUSH_BACK:
			status += VectorPushBackTest(vector);
			break;
			
		case POP_BACK:
			status += VectorPopBackTest(vector);
			break;
			
		case EXIT:
			break;
			
		default:
			printf("Invalid choice, please try again...\n");
			break;
		}
	}
	
	VectorDestroy(vector);
	vector = NULL;
	
	return ((SUCCESS == status) ? SUCCESS : FAIL);
}

/************** Automat Tests *******************/
static status_ty AutomatResereveTest(vector_ty *vector)
{
	status_ty status = 0;
		
	assert(NULL != vector);
	
	status = VectorReserve(vector, 10);
	printf("\nAfter reserve to 10 the capacity is: %lu\n",
	                                              VectorGetMaxCapacity(vector));
	                                              
	return ((SUCCESS == status) ? SUCCESS : FAIL);
}

static status_ty AutomatGetValueTest(vector_ty *vector)
{
	assert(NULL != vector);
	
	printf("\nThe value in the index 1 is: %d\n",
	                                           *(int*)VectorGetValue(vector,1));
	
	return SUCCESS;
}

static status_ty AutomatSetValueTest(vector_ty *vector)
{
	int val = 5;
	
	assert(NULL != vector);
	
	VectorSetValue(vector, 1, &val);
	printf("\nAfter set index 1 to value 5:\n");
	PrintIntVector(vector);
	
	return SUCCESS;	
}

static status_ty AutomatPushBackTest(vector_ty *vector)
{
	status_ty status = 0;
	int val = 3;
	
	assert(NULL != vector);
	
	status = VectorPushBack(vector, &val);
	printf("\nThe size after the push is: %lu\n", VectorGetSize(vector));
	printf("The capacity after the push is: %lu\n",
	                                              VectorGetMaxCapacity(vector));
	PrintIntVector(vector);
	
	return ((SUCCESS == status) ? SUCCESS : FAIL);
}

static status_ty AutomatTests()
{
	vector_ty *vector = NULL;
	status_ty status = 0;
	int i = 0;
	
	test_function_ty tests[] = {
		VectorIsEmptyTest,
		VectorGetSizeTest,
		VectorGetMaxCapacityTest,
		AutomatPushBackTest,
		AutomatPushBackTest,
		AutomatPushBackTest,
		VectorShrinkToFitTest,
		VectorPopBackTest,
		AutomatResereveTest,
		AutomatSetValueTest,
		VectorPopBackTest,
		AutomatPushBackTest,
		AutomatGetValueTest,
		VectorIsEmptyTest,
		VectorGetSizeTest,
		VectorGetMaxCapacityTest};
	
	vector = VectorCreate(CAPACITY, ELEMENT_SIZE);
	
	if (NULL == vector)
	{
		printf("\n* The creation of the vector failed *\n");
			
		return FAIL;
	}
	
	else
	{
		printf("\n* The creation of the vector success *\n");
	}
	
	for (i = 0; i < TESTS_NUM; ++i)
    {
        status = tests[i](vector);
        if (FAIL == status)
        {
        	return FAIL;
        }
    }
    
 	VectorDestroy(vector);
	vector = NULL;   

    return SUCCESS;
}
