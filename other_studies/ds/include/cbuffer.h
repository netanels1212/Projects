/*******************************************************************************
Project: Circular Buffer
Name: Netanel Shmuel
Reviewer: Yan
Date: 25.10.2023
Version: 1.0
*******************************************************************************/
#ifndef __CBUFFER_H__
#define __CBUFFER_H__

#include <stddef.h> /*size_t*/
#include <sys/types.h> /*ssize_t*/

typedef struct c_buff c_buff_ty;

/*
Description: Creates a circular buffer with the specified capacity.
Return Value: A pointer to the newly created circular buffer (c_buff_ty*).
Complexity: O(1)
Undefined Behavior: If the capacity is 0.
On Failure: Returns NULL
*/
c_buff_ty *CBuffCreate(size_t capacity);

/*
Description: This function frees the cbuffer and its resources.
Return Value: None (void)
Complexity: O(1)
Undefined Behavior: C_buff null
On Failure: Nothing
*/
void CBuffDestory(c_buff_ty *c_buff);

/*
Description: Reads count bytes from the circular buffer into the dest buffer. 
Return Value: The number of bytes actually read. 
Complexity: O(n)
Undefined Behavior: c_buff and dest null and count > size or when count > dest.
On Failure: Returns a negative value on failure.
*/
ssize_t CBuffRead(c_buff_ty *c_buff, void *dest, size_t count);

/*
Description: Writes count amount of bytes from the src into the circular buffer. 
Return Value: The number of bytes actually written
Complexity: O(n)
Undefined Behavior: c_buff and src null and count > free space or when 
                    count > src.
On Failure: Returns a negative value on failure.
*/
ssize_t CBuffWrite(c_buff_ty *c_buff, const void *src, size_t count);

/*
Description: Checks whether the circular buffer is empty.
Return Value: Returns 1 if the circular buffer is empty and 0 if it's not.
Complexity: O(1)
Undefined Behavior: c_buff null
On Failure: None
*/
int CBuffIsEmpty(const c_buff_ty *c_buff);

/*
Description: Returns the amount of free space available in the circular buffer.
Return Value: The amount of free space as a size_t.
Complexity: O(1)
Undefined Behavior: c_buff null
On Failure: None
*/
size_t CBuffFreeSpace(const c_buff_ty *c_buff);

/*
Description: Returns the current size (number of bytes used) of the circular buffer.
Return Value: The current size as a size_t.
Complexity: O(1)
Undefined Behavior: c_buff null
On Failure: None
*/
size_t CBuffSize(const c_buff_ty *c_buff);

/*
Description: Returns the read offset of the circular buffer.
Return Value: The current read offset as a size_t.
Complexity: O(1)
Undefined Behavior: c_buff null
On Failure: None
*/
size_t CBuffReadOffset(const c_buff_ty *c_buff);

#endif /*__CBUFFER_H__*/
