#include <stddef.h> /*size_t*/
#include <sys/types.h> /*ssize_t*/
#include <assert.h> /* assert */
#include <stdio.h> /* fprintf */
#include <stdlib.h> /* malloc, free */

#include "cbuffer.h"

enum i_o_ty {NOT_READ = -2, NOT_WRITE = -1};

struct c_buff
{
	size_t read_offset;
	size_t size;
	size_t capacity;
	char buffer[1];
};

c_buff_ty *CBuffCreate(size_t capacity)
{
	c_buff_ty *c_buff = NULL;
	
	assert(0 != capacity);
	
	c_buff = (c_buff_ty*) malloc(sizeof(c_buff_ty)+(capacity - 1)*sizeof(char));
	
	if (NULL == c_buff)
	{
		fprintf(stderr, "\nThe memory allocation for the buffer failed.\n");
		
		return NULL;
	}
	
	c_buff -> capacity = capacity;
	c_buff -> size = 0;
	c_buff -> read_offset = 0;
	
	return c_buff;
}

void CBuffDestory(c_buff_ty *c_buff)
{
	assert(NULL != c_buff);
	
	free(c_buff);
	c_buff = NULL;
}

ssize_t CBuffRead(c_buff_ty *c_buff, void *dest, size_t count)
{
	ssize_t bytes_was_read = NOT_READ;
	size_t i = 0;
	
	assert(NULL != c_buff);
	assert(NULL != dest);
	assert(count <= c_buff -> size);
	
	if(CBuffIsEmpty(c_buff))
	{
		return bytes_was_read;
	}
	
	for (i = 0; i < count; ++i)
	{
		*((char *)dest + i) = (c_buff -> buffer)[c_buff -> read_offset];
		c_buff -> read_offset = (c_buff -> read_offset +1) % c_buff -> capacity;
		--(c_buff -> size);
		++bytes_was_read;
	}
	*((char *)dest + i) = '\0';
	
	return 1 + bytes_was_read; /* maybe 2 +   ? */
}

ssize_t CBuffWrite(c_buff_ty *c_buff, const void *src, size_t count)
{
	ssize_t bytes_was_write = NOT_WRITE;
	size_t i = 0;
	
	assert(NULL != c_buff);
	assert(NULL != src);
	assert(count <= CBuffFreeSpace(c_buff));
	
	if(CBuffFreeSpace(c_buff))
	{
		return bytes_was_write;
	}
	
	for (i = 0; i < count; ++i)
	{
		c_buff -> buffer[(c_buff->read_offset + c_buff->size)%c_buff->capacity]
		                                                   = *((char *)src + i);
		++(c_buff -> size);                                                        
		++bytes_was_write;
	}
	
	return 1 + bytes_was_write;
}

int CBuffIsEmpty(const c_buff_ty *c_buff)
{
	assert(NULL != c_buff);
	
	return (0 == c_buff -> size);
}

size_t CBuffFreeSpace(const c_buff_ty *c_buff)
{
	assert(NULL != c_buff);
	
	return ((c_buff -> capacity) - (c_buff -> size));
}

size_t CBuffSize(const c_buff_ty *c_buff)
{
	assert(NULL != c_buff);
	
	return c_buff -> size;
}

size_t CBuffReadOffset(const c_buff_ty *c_buff)
{
	assert(NULL != c_buff);
	
	return c_buff -> read_offset;
}
