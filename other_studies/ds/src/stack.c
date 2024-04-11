#include <stdio.h> /* fprintf */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */

#include "stack.h"

struct stack
{
    size_t max_capacity;	       /* max size that the stack can be */
    size_t curr_num_of_elements;  /* number of current elements in the stack */
    size_t size_of_element;
    char *buffer;
};

stack_ty *StackCreate(size_t size, size_t size_of_element)
{
	stack_ty *stack;
	size_t total_size = 0;
	
	total_size = sizeof(stack_ty) + (size * size_of_element);
	stack = (stack_ty *) malloc(total_size);
	
	if (NULL == stack)
	{
		return NULL;
	}

	stack -> max_capacity = size;
	stack -> curr_num_of_elements = 0;
	stack -> size_of_element = size_of_element;
	stack -> buffer = ((char *) stack) + sizeof(stack_ty);
	
	return stack;
}

void StackDestroy(stack_ty *stack)
{
	assert(NULL != stack);
	
	free(stack);
	stack = NULL;
}

size_t StackSize(const stack_ty *stack) 
{
	assert(NULL != stack);
	
	return stack -> curr_num_of_elements;
}

size_t StackCapacity(const stack_ty *stack)
{
	assert(NULL != stack);
	
	return stack -> max_capacity;
}

void StackPush(stack_ty *stack, const void *element)
{
	
	assert(NULL != stack);
	assert(NULL != element);
	
	if (StackSize(stack) < stack -> max_capacity)
	{
		memcpy(stack -> buffer + (StackSize(stack) * stack -> size_of_element),
		       element, stack -> size_of_element);
		stack -> curr_num_of_elements += 1;	
	}
	
	else
	{
		fprintf(stderr, "The stack is already full,\
		                 it's not possible to push element.\n");
	}
}

int StackIsEmpty(const stack_ty *stack)
{
	assert(NULL != stack);
	
	return (0 == StackSize(stack));
}

void StackPop(stack_ty *stack)
{	
	assert(NULL != stack);
	
	if (0 == StackIsEmpty(stack))
	{
		stack -> curr_num_of_elements -= 1;
	}
	
	else
	{
		fprintf(stderr, "The stack is empty, you can't remove a element.\n");
	}
}

void *StackPeek(const stack_ty *stack)
{
	assert(NULL != stack);
	
	if (0 < StackSize(stack))
	{
		return &(stack -> buffer[StackSize(stack) * stack -> size_of_element
		        - stack -> size_of_element]);
	}
	
	else
	{
		return NULL;
	} 
}
