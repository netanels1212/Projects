#include <sys/types.h> /*ssize_t*/
#include <stdio.h> /* printf */
#include <assert.h> /* assert */

#include "cbuffer.h"

#define TESTS_NUM 4 
#define CAPACITY 5

enum cbuff_return_value {NOT_READ = -2, NOT_WRITE = -1, CB_SUCCESS, CB_FAIL};

typedef int status_ty;
typedef status_ty (*cbuff_test_function_ty)(c_buff_ty *);

/*********** Automate Tests Functions **************/
static status_ty CBuffReadWriteTest(c_buff_ty *c_buff);
static status_ty CBuffIsEmptyTest(c_buff_ty *c_buff);
static status_ty CBuffFreeSpaceTest(c_buff_ty *c_buff);
static status_ty CBuffSizeTest(c_buff_ty *c_buff);

static status_ty AutomateTests();

int main()
{
	status_ty cbuff_status = 0;
	
	cbuff_status = AutomateTests();
	
	if (CB_SUCCESS != cbuff_status)
	{
		printf("\n *** There is an ERROR in the program ***\n");
		
		return CB_FAIL;
	}
	
	printf("\n *** All the tests works, the program work fine ***\n");
	
	return CB_SUCCESS;
}

static status_ty CBuffReadWriteTest(c_buff_ty *c_buff) /* for capacity >= 4 */
{
	ssize_t bytes_was_read = NOT_READ;
	ssize_t bytes_was_write = NOT_WRITE;
	char *src = "hello!!!";
	char dest[2] = {0};
	
	assert(NULL != c_buff);
	
	bytes_was_write = CBuffWrite(c_buff, (void*) src, CAPACITY - 2);
	printf("\n%ld bytes was wrote", bytes_was_write);
	CBuffIsEmptyTest(c_buff);
	CBuffFreeSpaceTest(c_buff);
	CBuffSizeTest(c_buff);
	printf("\n");
	
	bytes_was_read = CBuffRead(c_buff, (void*) dest, 2);
	printf("\n%ld bytes was read\n", bytes_was_read);
	printf("The read offset after the read is: %lu\n", CBuffReadOffset(c_buff));
	printf("The elements was read: %s\n", dest);
	CBuffIsEmptyTest(c_buff);
	CBuffFreeSpaceTest(c_buff);
	CBuffSizeTest(c_buff);
	
	return CB_SUCCESS;
}

static status_ty CBuffIsEmptyTest(c_buff_ty *c_buff)
{
	assert(NULL != c_buff);
	
	if (CBuffIsEmpty(c_buff))
	{
		printf("\nThe circular buffer is empty.");
	}
	
	else
	{
		printf("\nThe circular buffer isn't empty.");	
	}
	
	return CB_SUCCESS;
}

static status_ty CBuffFreeSpaceTest(c_buff_ty *c_buff)
{
	assert(NULL != c_buff);
	
	printf("\nThe free space in the circular buffer is: %lu", 
	                                                    CBuffFreeSpace(c_buff));
	
	return CB_SUCCESS;
}

static status_ty CBuffSizeTest(c_buff_ty *c_buff)
{
	assert(NULL != c_buff);
	
	printf("\nThe size of the circular buffer is: %lu",CBuffSize(c_buff));
	
	return CB_SUCCESS;
}

static status_ty AutomateTests()
{
	c_buff_ty *c_buff = NULL;
	status_ty cbuff_status = 0;
	int i = 0;

	cbuff_test_function_ty tests[] = {
		CBuffIsEmptyTest,
		CBuffFreeSpaceTest,
		CBuffSizeTest,
		CBuffReadWriteTest };
	
	c_buff = CBuffCreate(CAPACITY);
	
	if (NULL == c_buff)
	{
		printf("\n*** The creation of the circular buffer failed ***\n");
			
		return CB_FAIL;
	}
	
	else
	{
		printf("\n*** The creation of the circular buffer success ***\n");
		printf("The capacity is: %d\n", CAPACITY);
	}
	
	for (i = 0; i < TESTS_NUM; ++i)
    {
    	printf("\nTest N.%d:", i);
        cbuff_status = tests[i](c_buff);
        printf("\n");
        if (CB_FAIL == cbuff_status)
        {
        	return CB_FAIL;
        }
    }
    
 	CBuffDestory(c_buff);
	c_buff = NULL;   

    return CB_SUCCESS;
}

