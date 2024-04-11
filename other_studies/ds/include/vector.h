/*******************************************************************************
Project: Vector (WS14)
Name: Netanel Shmuel
Reviewer: Najeeb 
Date: 05/10/2023
Version: 1.0
*******************************************************************************/
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <ctype.h> /* size_t */

enum return_values_ty {SUCCESS, FAIL};

typedef struct vector vector_ty;
typedef int status_ty;

/*
Function Description:create a vector with a capacity that was stated by the user

Return value: pointer to the vector

Notes: if memory allocation for the vector fails, returns NULL. 
	Undefined behaviour if capacity or element_size is set to 0.

Time Complexity: O(1)
*/
vector_ty *VectorCreate(size_t capacity , size_t element_size);

/*
Function Description: Destroy the vector and free its resources.

Return value: void

Notes: 
- If 'vector' is NULL, the function has no effect.
- After calling this function, the 'vector' pointer becomes invalid and 
  should not be used.

Time Complexity: O(1)
*/
void VectorDestroy(vector_ty *vector);

/*
Function Description: Get the current size (number of elements) of the vector.

Return value: 
- The current size of the vector.

Notes: 
- undefined when 'vector' is NULL.

Time Complexity: O(1)
*/
size_t VectorGetSize(vector_ty *vector);

/*
Function Description: Get the maximum capacity (number of elements) the vector
		 can hold without resizing.

Return value: 
- The maximum capacity of the vector.

Notes: 
- undefined when 'vector' is NULL.

Time Complexity: O(1)
*/
size_t VectorGetMaxCapacity(vector_ty *vector);

/*
Function Description: Check if the vector is empty (contains no elements).

Return value: 
- STATUS_SUCCESS (1) if the vector is empty.
- STATUS_FAILURE (0) if the vector is not empty.

Notes: 
- The function checks if the vector's size is zero to determine emptiness.
- undefined when 'vector' is NULL.

Time Complexity: O(1)
*/
status_ty VectorIsEmpty(vector_ty *vector);

/*
Function Description: Print the element of int type vector.

Return value: void.

Notes: 
- Prints the elements if the vector isn't empty,
  Otherwise print:"The vector is empty."
- undefined when 'vector' is NULL.

Time Complexity: O(n)
*/
void PrintIntVector(vector_ty *vector); 

/*
Function Description: Resize the vector to the reserved capacity

Return value: status of operation

Notes: if resizing the vector fails returns False. 
	   Undefined behaviour if pointer is NULL or new_capicty <= 0.
	   if new_capacity < vector -> size, yout data will be corrupt.
	
Time Complexity: O(1)
*/
status_ty VectorReserve(vector_ty *vector, size_t new_capacity);

/*
Function Description: Shrink vector to the current size

Return value: status of operation

Notes: if memory allocation for the vector fails, returns False. 
	Undefined behaviour if pointer is NULL.
	
Time Complexity: O(1)
*/
status_ty VectorShrinkToFit(vector_ty *vector);

/*
Function Description: Get the value at a specified index in the vector.

Return value: 
- Pointer to the value at the specified index.

Notes: 
- The returned pointer points to the value within the vector.
- Any modifications made to the returned value will affect the vector.
- undifined when the index greater then the vector size or 'vector' is NULL.

Time Complexity: O(1)
*/
void *VectorGetValue(vector_ty *vector, size_t index);

/*
Function Description: Set the value at a specified index in the vector.

Return value: void

Notes: 
- If 'vector' is NULL or 'index' is out of bounds, the function behavior is 
  undefined.
- 'val' should point to a valid value of the appropriate data type.

Time Complexity: O(1)
*/
void VectorSetValue(vector_ty *vector, size_t index, void *val);

/*
Function Description: Add another value to the end of the vector

Return value: status of operation

Notes: if adding to the vector fails, returns False. 
	Undefined behaviour if pointer is NULL or element is of wrong type.
	
Time Complexity: O(1)
*/
status_ty VectorPushBack(vector_ty *vector, void *element);

/*
Function Description: Remove value from the end of the vector

Return value: status of operation

Notes: if removing from vector fails, returns False. 
	Undefined behaviour if pointer is NULL.
	
Time Complexity: O(1)
*/
status_ty VectorPopBack(vector_ty *vector);

#endif /*__VECTOR_H__*/
