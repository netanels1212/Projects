#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdio.h> /* printf */

#include "avl.h"
#include "utils.h"

enum
{
	TESTS_NUM = 7
};

typedef int (*avl_test_func_ty)(void);

static int IsBefore(const void *data1_, const void *data2_, const void *params_);
static int CopyAvlToArrOP(void *node_data_, void *dest_);

static int AvlCreateDestroyTest(void);
static int AvlInsertTest(void);
static int AvlHeightTest(void);
static int AvlSizeTest(void);
static int AvlIsEmptyTest(void);
static int AvlFindTest(void);
static int AvlForEachTest(void);

static int AutomateTests(void);

int main()
{
	int avl_status = AVL_NO_ERROR;
	
	avl_status = AutomateTests();
	
	if (AVL_NO_ERROR != avl_status)
	{
		printf("\n *** There is an ERROR in the program ***\n");
		
		return AVL_GENERAL_ERROR;
	}
	
	printf("\n *** All the tests passed, the program work fine ***\n");
	
	return AVL_NO_ERROR;
}

static int IsBefore(const void *data1_, const void *data2_, const void *params_)
{
	assert(NULL != data1_);
	assert(NULL != data2_);

	return (*((int*)data1_) < *((int*)data2_));
}

static int AvlCreateDestroyTest(void)
{
	avl_ty *avl = AvlCreate(IsBefore, NULL);

	if (NULL == avl)
	{
		fprintf(stderr, "\nAvlCreate() function didn't work properly\n");
		
		return AVL_GENERAL_ERROR;
	}

	/* TODO: BAD_MEM(avl);  */
	AvlDestroy(avl);

	return AVL_NO_ERROR;
}

static int CopyAvlToArrOP(void *node_data_, void *dest_)
{
	int i = 0;

	assert(NULL != node_data_);
	assert(NULL != dest_);

	while (-1 != ((int *)dest_)[i])
	{
		++i;
	}

	((int *)dest_)[i] = *((int *)node_data_);

	return AVL_NO_ERROR;
}

static int AvlInsertTest(void)
{
	avl_ty *avl = AvlCreate(IsBefore, NULL);
	int data[] = {3, 6, 5};
	int avl_nodes[] = {-1, -1, -1};
	size_t data_amount = 3;
	size_t size_after_insert = 0;
	size_t i = 0;

	for (i = 0; i < data_amount; ++i)
	{
		AvlInsert(avl, (void *)&data[i]);
	}

	AvlForEach(avl, PRE_ORDER, CopyAvlToArrOP, avl_nodes);

	for (i = 0; i < data_amount; ++i)
	{
		if (avl_nodes[i] != data[i])
		{
			fprintf(stderr, "\nAvlInsert() didn't work properly\n");

			AvlDestroy(avl);

			return AVL_GENERAL_ERROR; 
		}
	}

	size_after_insert = AvlSize(avl);
	AvlDestroy(avl);

	if (size_after_insert != data_amount)
	{
		fprintf(stderr, "\nAvlInsert() didn't work properly\n");

		return AVL_GENERAL_ERROR; 
	}

	return AVL_NO_ERROR;
}

static int AvlHeightTest(void)
{
	avl_ty *avl = AvlCreate(IsBefore, NULL);
	int data[] = {3, 6, 5, 2};
	size_t data_amount = 4;
	size_t height = 0;
	size_t expected_height = 3;
	size_t i = 0;

	for (i = 0; i < data_amount; ++i)
	{
		AvlInsert(avl, (void *)&data[i]);
	}

	height = AvlHeight(avl);
	AvlDestroy(avl);

	if (height != expected_height)
	{
		fprintf(stderr, "\nAvlHeight() didn't work properly\n");

		return AVL_GENERAL_ERROR; 
	}

	return AVL_NO_ERROR;
}

static int AvlSizeTest(void)
{
	avl_ty *avl = AvlCreate(IsBefore, NULL);
	int data[] = {3, 6, 5, 2};
	size_t data_amount = 4;
	size_t size_after_insert = 0;
	size_t i = 0;

	for (i = 0; i < data_amount; ++i)
	{
		AvlInsert(avl, (void *)&data[i]);
	}

	size_after_insert = AvlSize(avl);
	AvlDestroy(avl);

	if (size_after_insert != data_amount)
	{
		fprintf(stderr, "\nAvlSize() didn't work properly\n");

		return AVL_GENERAL_ERROR; 
	}

	return AVL_NO_ERROR;
}

static int AvlIsEmptyTest(void)
{
	avl_ty *avl = AvlCreate(IsBefore, NULL);
	int data[] = {3, 6, 5, 2};
	size_t data_amount = 4;
	size_t size_after_insert = 0;
	size_t i = 0;

	if (1 != AvlIsEmpty(avl))
	{
		fprintf(stderr, "\nAvlIsEmpty() didn't work properly\n");

		AvlDestroy(avl);

		return AVL_GENERAL_ERROR; 
	}

	for (i = 0; i < data_amount; ++i)
	{
		AvlInsert(avl, (void *)&data[i]);
	}

	if (0 != AvlIsEmpty(avl))
	{
		fprintf(stderr, "\nAvlIsEmpty() didn't work properly\n");

		AvlDestroy(avl);

		return AVL_GENERAL_ERROR; 
	}

	AvlDestroy(avl);

	return AVL_NO_ERROR;
}

static int AvlFindTest(void)
{
	avl_ty *avl = AvlCreate(IsBefore, NULL);
	void *find_ret = NULL;
	int data[] = {3, 6, 5, 2, 9, 8};
	size_t data_amount = 6;
	size_t i = 0;

	for (i = 0; i < data_amount; ++i)
	{
		AvlInsert(avl, (void *)&data[i]);
	}

	find_ret = AvlFind(avl, &data[5]);

	if (NULL == find_ret)
	{
		fprintf(stderr, "\nAvlFind() didn't work properly\n");

	    AvlDestroy(avl);

		return AVL_GENERAL_ERROR; 
	}

	AvlDestroy(avl);

	return AVL_NO_ERROR;
}

static int AvlForEachTest(void)
{
	avl_ty *avl = AvlCreate(IsBefore, NULL);
	int data[] = {3, 6, 5, 2, 9, 8};
	int pre_order_expected[] = {3, 2, 6, 5, 9, 8};
	int in_order_expected[] = {2, 3, 5, 6, 8, 9};
	int post_order_expected[] = {2, 5, 8, 9, 6, 3};
	int pre_order[] = {-1, -1, -1, -1, -1, -1};
	int in_order[] = {-1, -1, -1, -1, -1, -1};
	int post_order[] = {-1, -1, -1, -1, -1, -1}; 
	size_t data_amount = 6;
	size_t i = 0;

	for (i = 0; i < data_amount; ++i)
	{
		AvlInsert(avl, (void *)&data[i]);
	}

	AvlForEach(avl, PRE_ORDER, CopyAvlToArrOP, pre_order);
	AvlForEach(avl, IN_ORDER, CopyAvlToArrOP, in_order);
	AvlForEach(avl, POST_ORDER, CopyAvlToArrOP, post_order);

	for (i = 0; i < data_amount; ++i)
	{
		if ((size_t)pre_order[i] != pre_order_expected[i])
		{
			fprintf(stderr, "\nAvlForEach() - pre_order - didn't work properly\n");

			AvlDestroy(avl);

			return AVL_GENERAL_ERROR; 
		}

		if ((size_t)in_order[i] != in_order_expected[i])
		{
			fprintf(stderr, "\nAvlForEach() - in_order - didn't work properly\n");

			AvlDestroy(avl);

			return AVL_GENERAL_ERROR; 
		}

		if ((size_t)post_order[i] != post_order_expected[i])
		{
			fprintf(stderr, "\nAvlForEach() - post_order - didn't work properly\n");

			AvlDestroy(avl);

			return AVL_GENERAL_ERROR; 
		}
	}

	AvlDestroy(avl);

	return AVL_NO_ERROR;
}

static int AutomateTests(void)
{
	int status = 0;
	int i = 0;

	avl_test_func_ty tests[] = 
	{
		AvlCreateDestroyTest,
		AvlInsertTest,
		AvlHeightTest,
		AvlSizeTest,
		AvlIsEmptyTest,
		AvlFindTest,
		AvlForEachTest
	};
	
	const char* test_func_names[] =
	{
		"AvlCreateDestroyTest",
		"AvlInsertTest",
		"AvlHeightTest",
		"AvlSizeTest",
		"AvlIsEmptyTest",
		"AvlFindTest",
		"AvlForEachTest"
    };

	for (i = 0; i < TESTS_NUM; ++i)
    {
    	printf("\nTest N.%d - %s:", i, test_func_names[i]);
        status = tests[i]();
        printf("\n-- Test function return value: %d, ", status);
        
        if (AVL_NO_ERROR != status)
        {
        	printf("not passed --\n\n");

        	return AVL_GENERAL_ERROR;
        }

        else
        {
            printf("passed! --\n\n");
        }
    } 

    return AVL_NO_ERROR;
}