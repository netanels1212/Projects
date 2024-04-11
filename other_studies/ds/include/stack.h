/*******************************************************************************
Project: Stack (WS12)
Name: Netanel Shmuel
Reviewer: Yan
Date: 02.10.2023
Version: 1.0
*******************************************************************************/

#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h>/*size_t*/

#define EXIT 0

typedef struct stack stack_ty;

/* Return value: ptr to the stack we create.
Undefined behaviour: size lower then size_of_elements */
stack_ty *StackCreate(size_t size, size_t size_of_element);

/* delete the stack from the memory.
Undefined behaviour: NULL ptr */
void StackDestroy(stack_ty *stack);

/* Return value: Number of current elements in the stack.
Undefined behaviour: NULL ptr */
size_t StackSize(const stack_ty *stack);

/* Return value: Max size that the stack can be.
Undefined behaviour: NULL ptr */
size_t StackCapacity(const stack_ty *stack);

/* insert a new member to the stack.
Undefined behaviour: NULL ptr */
void StackPush(stack_ty *stack, const void *element);

/* Return value: '1' if the stack are empty,
                 '0' if not.
Undefined behaviour: NULL ptr */
int StackIsEmpty(const stack_ty *stack);

/* remove the last element in the stack.
Undefined behaviour: NULL ptr */
void StackPop(stack_ty *stack);

/* Return value: Pointer to the last element in the stack.
Undefined behaviour: NULL ptr */
void *StackPeek(const stack_ty *stack);

#endif /*__STACK_H__*/
