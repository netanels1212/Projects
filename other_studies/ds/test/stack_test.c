#include <stdio.h> /* printf, scanf */
#include <stdlib.h> /* free */
#include <ctype.h> /* size_t */
#include <assert.h> /* assert */

#include "stack.h"

enum return_values_ty {SUCCESS, FAIL};

static int StackCreateTest(stack_ty *stack);
static int StackSizeTest(stack_ty *stack);
static int StackCapacityTest(stack_ty *stack, size_t size);
static int StackPushAndPeekTest(stack_ty *stack);
static void StackIsEmptyTest(stack_ty *stack);
static int StackPopTest(stack_ty *stack);
static void TestMenu();

int main()
{
	TestMenu();

	return 0;
}

static int StackCreateTest(stack_ty *stack)
{
	assert(NULL != stack);
	
	printf("\n--- Test for StackCreate() ---\n"); 
	
	if (NULL == stack)
	{
		printf("\n* The function is not working properly *\n");
			
		return FAIL;
	}
	
	else
	{
		printf("\n* The function is working properly *\n");
		
		return SUCCESS;
	}	
}

static int StackSizeTest(stack_ty *stack)
{
	size_t last_size = 0;
	int element = 0;
	int is_push = 0;
	
	assert(NULL != stack);
	
	printf("\n--- Test for StackSize() ---\n");
	
	last_size = StackSize(stack);
	
	printf("Current size: %lu\n", last_size);
	
	if (!StackIsEmpty(stack))
	{
		StackPop(stack);
			
		printf("The size after pop is: %lu\n", StackSize(stack));
	}
	
	else
	{	
		printf("Enter INT element to push: ");
		scanf("%d", &element);
		StackPush(stack, &element);
		is_push = 1;
			
		printf("The size after push is: %lu\n", StackSize(stack));
	}
	
	if ((1 == is_push && last_size + 1 == StackSize(stack)) ||
	    (0 == is_push && last_size - 1 == StackSize(stack)))
	{
		printf("\n* The function is working properly *\n");
		
		return SUCCESS;
	}
	
	printf("\n* The function is not working properly *\n");
			
	return FAIL;
}

static int StackCapacityTest(stack_ty *stack, size_t size)
{
	assert(NULL != stack);
	
	printf("\n--- Test for StackCapacity() ---\n");
	
	printf("The stack capacity is: %lu\n", StackCapacity(stack));

	if (StackCapacity(stack) != size)
	{
		printf("\n* The function is not working properly *\n");
			
		return FAIL;
	}
	
	else
	{
		printf("\n* The function is working properly *\n");
		
		return SUCCESS;
	}	
}
	
static int StackPushAndPeekTest(stack_ty *stack)
{
	int element = 0;
	size_t size_before_push = 0;	
	
	assert(NULL != stack);
	
	printf("\n--- Test for StackPushAndPeekTest() ---\n");	
	
	printf("Enter INT element to push: ");
	scanf("%d", &element);
	
	size_before_push = StackSize(stack);
	StackPush(stack, &element);
	
	if (*(int *)StackPeek(stack) != element &&
	    StackCapacity(stack) != size_before_push)
	{
		printf("\n* The function is not working properly *\n");
			
		return FAIL;
	}
	
	else
	{
		printf("\n* The function is working properly *\n");
		
		return SUCCESS;
	}
}

static void StackIsEmptyTest(stack_ty *stack)
{
	assert(NULL != stack);
	
	printf("\n--- Test for StackIsEmpty() ---\n");				
	
	if (StackIsEmpty(stack))
	{
		printf("\nThe stack are empty\n");
	}
	
	else
	{
		printf("\nThe stack not empty\n");
	}
}

static int StackPopTest(stack_ty *stack)
{
	size_t size_before_pop = 0;
	
	assert(NULL != stack);
		
	printf("\n--- Test for StackPop() ---\n");
	
	size_before_pop = StackSize(stack);
	StackPop(stack);
	
	if (StackSize(stack) != size_before_pop - 1 && 0 != size_before_pop)
	{
		printf("\n* The function is not working properly *\n");
			
		return FAIL;
	}
	
	else
	{
		printf("\n* The function is working properly *\n");
		
		return SUCCESS;
	}		
}
		
static void TestMenu()
{
	stack_ty *stack = NULL;
	size_t size = 0;
	size_t size_of_element = 0;
	int choice = 0;

	printf("\n --- Tests Menu: ---\n");
	printf("\n1. StackCreateTest()\n");
	printf("2. StackSizeTest()\n");
	printf("3. StackCapacityTest()\n");
	printf("4. StackPushAndPeekTest()\n");
	printf("5. StackIsEmptyTest()\n");
	printf("6. StackPopTest()\n");
	printf("0. Exit\n\n");
	
	printf("Enter stack max size: ");
	scanf("%lu", &size);
	
	printf("Enter size of each element: ");
	scanf("%lu", &size_of_element);
	stack = StackCreate(size, size_of_element);
	
	choice = 1;

	
	while (choice != EXIT)
	
	{
		printf("Enter your choice: ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			printf("\n");
			StackCreateTest(stack);
			printf("\n");
			break;		
		case 2:
			printf("\n");
			StackSizeTest(stack);
			printf("\n");
			break;
		case 3:
			printf("\n");
			StackCapacityTest(stack, size);
			printf("\n");
			break;
		case 4:
			printf("\n");
			StackPushAndPeekTest(stack);
			printf("\n");
			break;
		case 5:
			printf("\n");
			StackIsEmptyTest(stack);
			printf("\n");
			break;
		case 6:
			printf("\n");
			StackPopTest(stack);
			printf("\n");
			break;
		case 0:
			break;
		default:
			printf("Invalid choice, please try again...\n");
			break;
		}
	}
	
	StackDestroy(stack);
	
	stack = NULL;
}
