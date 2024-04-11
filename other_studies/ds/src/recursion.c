#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <string.h> /* strncmp */
#include <stdio.h> /* printf */

#include "stack.h"

enum
{
	NO_ERROR = 0,
	ERROR
};

unsigned long  FibonacciIterative(size_t index_)
{
	unsigned long first_element = 1;
	unsigned long second_element = 1;
	unsigned long third_element = 0;
	size_t i = 0;

	if (1 >= index_)
	{
		return 1;
	}

	for (i = 2; i <= index_; ++i)
	{
		third_element = first_element + second_element;
		first_element = second_element;
		second_element = third_element;	
	}

	return third_element;
}

unsigned long FibonacciRecursive(size_t index_)
{
	if (1 >= index_)
	{
		return 1;
	}

	return FibonacciRecursive(index_ - 2) + FibonacciRecursive(index_ - 1);
}

size_t StrLenIMP(size_t num_, const char *str_)
{
	assert(NULL != str_);

	if('\0' == *str_)
	{
		return 0 + num_;
	}

	return StrLenIMP(1 + num_, str_ + 1);
}

size_t StrLen(const char *str_)
{
	return StrLenIMP(0, str_);
}


int StrCmp(const char *str1_, const char *str2_)
{
	assert(NULL != str1_);
	assert(NULL != str2_);

	if (*str1_ != *str2_ || '\0' == *str1_)
	{
		return (*str1_ - *str2_);
	}

	return StrCmp(str1_ + 1, str2_ + 1);
}

char *StrCpy(char *dest_, const char *src_)
{
	assert(NULL != dest_);
	assert(NULL != src_);

	*dest_ = *src_;

	if ('\0' == *src_)
	{
		return dest_;
	}

	StrCpy(dest_ + 1, src_ + 1);

	return dest_;
}

char *StrCat(char *dest_, const char *src_)
{
	assert(NULL != dest_);
	assert(NULL != src_);

	if ('\0' == *dest_)
	{
		return StrCpy(dest_, src_);
	}

	StrCat(dest_ + 1, src_);

	return dest_;
}

char *StrStr(const char *heystack_, const char *needle_)
{
	size_t str_len = strlen(needle_);

	/*stop condition - if we over on all the heystack_ */
	if('\0' == *heystack_)
	{
		/*return NULL*/
		return NULL;
	}
	
	/* if the needle was found */
	if (0 == strncmp(heystack_, needle_, str_len))	
	{
		/* return the heystack_ from this position */
		return (char *)heystack_;
	}

	/* if needle not found - continue search in the heystack_ */
	return StrStr(heystack_ + 1, needle_);
}

/*Node* FlipList(Node* node)
{
    Node* new_head = NULL;

    assert(NULL != node );

    if (NULL == node->next)
    {
        return node;
    }

    new_head = FlipList(node->next);

    node->next->next = node;
    node->next = NULL;

    return new_head;
}*/

void InsertToSortedStack(stack_ty *stack_, void *data_)
{
	int top_data = 0;

	if(StackIsEmpty(stack_) || *(int *)data_ <= *(int *)StackPeek(stack_))
	{
		StackPush(stack_, data_);

		return;
	}

	top_data = *(int*)StackPeek(stack_);

	StackPop(stack_);
	
	InsertToSortedStack(stack_, data_);

	StackPush(stack_, &top_data);
}

void SortStack(stack_ty *stack_)
{
	void *data = NULL;

	if(StackIsEmpty(stack_))
	{
		return;
	}

	data = StackPeek(stack_);

	StackPop(stack_);

	SortStack(stack_);

	InsertToSortedStack(stack_, data);
}

static int CheckSortStack(stack_ty *stack_, int expected_[])
{
	int i = 0;

    while(!StackIsEmpty(stack_))
    {
        if (*(int*)StackPeek(stack_) != expected_[i])
        {
        	return ERROR;
        }

        StackPop(stack_);
        ++i;
    }

    return NO_ERROR;
}

static int TestFunc(void)
{
	stack_ty *stack = NULL;
	int stack_data[10] = {5, 6, 3, 4, 9, 1, 3, 7, 6, 0};
	int expected[10] =   {0, 1, 3, 3, 4, 5, 6, 6, 7, 9};
	char str1[] = "hello!!!!";
	char str2[] = "lo!";
	char str3[] = "blutiblut";
	char cat_res1[] = "hello!!!!lo!";
	char cat_res2[] = "lo!hello!!!!lo!";
	int i = 0;

	if (9 != StrLen(str1) || 3 != StrLen(str2))
	{
		printf("\nThe function StrLen() don't work properly.\n");

		return ERROR; 
	}

	if (0 == StrCmp(str1, str2) || 0 == StrCmp(str2, str1) || 0 != StrCmp(str1, str1))
	{
		printf("\nThe function StrCmp() don't work properly.\n");

		return ERROR; 
	}

	if (0 != StrCmp(StrCpy(str3, str2), str2))
	{
		printf("\nThe function StrCpy() don't work properly.\n");

		return ERROR; 
	}

	if (0 != StrCmp(StrCat(str1, str2), cat_res1) || 0 != StrCmp(StrCat(str2, str1), cat_res2))
	{
		printf("\nThe function StrCat() don't work properly.\n");

		return ERROR; 
	}

	if (NULL != StrStr(cat_res1, cat_res2) || 0 != StrCmp(StrStr(cat_res2, cat_res1), cat_res1))
	{
		printf("\nThe function StrStr() don't work properly.\n");

		return ERROR; 
	}

	if (55 != FibonacciIterative(9))
	{
		printf("\nThe function FibonacciIterative() don't work properly.\n");

		return ERROR; 
	}

	if (55 != FibonacciRecursive(9))
	{
		printf("\nThe function FibonacciRecursive() don't work properly.\n");

		return ERROR; 
	}


	stack = StackCreate(40, sizeof(int));
	for (i = 0; i < 10; ++i)
	{
		StackPush(stack, stack_data + i);
	}

	SortStack(stack);

	if (NO_ERROR != CheckSortStack(stack, expected))
	{
		printf("\nThe function SortStack() don't work properly.\n");

		return ERROR; 
	}

	return NO_ERROR;
}

int main()
{
	int test_status = TestFunc();

	if (NO_ERROR != test_status)
	{
		printf("\n *** There is an ERROR in one or more funtions ***\n");
		
		return ERROR;
	}

	printf("\n *** All the functions work fine ***\n");
	
	return NO_ERROR;
}