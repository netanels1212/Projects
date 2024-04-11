#include <stddef.h> /* size_t */
#include <stdio.h> /* printf, fprintf */

#include "trie.h"

enum
{
	TESTS_NUM = 5
};

typedef int (*trie_test_func_ty)(void);


static int TrieCreateDestroyTest(void);
static int TrieGetTest(void);
static int TrieFreeTest(void);
static int TrieCountAvailableTest(void);
static int TrieIsAllTakenTest(void);

static int AutomateTests(void);

int main()
{
	int trie_status = TRIE_NO_ERROR;
	
	trie_status = AutomateTests();
	
	if (TRIE_NO_ERROR != trie_status)
	{
		printf("\n *** There is an ERROR in the program ***\n");
		
		return TRIE_ERROR;
	}
	
	printf("\n *** All the tests passed, the program work fine ***\n");
	
	return TRIE_NO_ERROR;
}

static int TrieCreateDestroyTest(void)
{
	trie_ty *trie = TrieCreate(5);

	if (NULL == trie)
	{
		fprintf(stderr, "\nTrieCreate() function didn't work properly\n");
		
		return TRIE_ERROR;
	}

	TrieDestroy(trie);

	return TRIE_NO_ERROR;
}

static int TrieGetTest(void)
{
	trie_ty *trie = TrieCreate(4);
	unsigned long get_status = 0;

	get_status = TrieGet(trie, 10);

	if (10 != get_status)
	{
		fprintf(stderr, "\nTrieGet() function didn't work properly\n");

		TrieDestroy(trie);
		
		return TRIE_ERROR;
	}

	get_status = TrieGet(trie, 10);

	if (11 != get_status)
	{
		fprintf(stderr, "\nTrieGet() function didn't work properly\n");

		TrieDestroy(trie);
		
		return TRIE_ERROR;
	}

	TrieDestroy(trie);

	return TRIE_NO_ERROR;
}

static int TrieFreeTest(void)
{
	trie_ty *trie = TrieCreate(4);
	unsigned long get_status = 0;

	TrieFree(trie, 10);
	TrieGet(trie, 10);
	TrieFree(trie, 10);
	get_status = TrieGet(trie, 10);

	if (10 != get_status)
	{
		fprintf(stderr, "\nTrieFree() function didn't work properly\n");

		TrieDestroy(trie);
		
		return TRIE_ERROR;
	}

	TrieDestroy(trie);

	return TRIE_NO_ERROR;
}

static int TrieCountAvailableTest(void)
{
	trie_ty *trie = TrieCreate(4);

	if (16 != TrieCountAvailable(trie))
	{
		fprintf(stderr, "\nTrieCountAvailable() function didn't work properly\n");

		TrieDestroy(trie);
		
		return TRIE_ERROR;
	}

	TrieGet(trie, 10);

	if (15 != TrieCountAvailable(trie))
	{
		fprintf(stderr, "\nTrieCountAvailable() function didn't work properly\n");

		TrieDestroy(trie);
		
		return TRIE_ERROR;
	}

	TrieDestroy(trie);

	return TRIE_NO_ERROR;
}

static int TrieIsAllTakenTest(void)
{
	trie_ty *trie = TrieCreate(2);

	if (0 != TrieIsAllTaken(trie))
	{
		fprintf(stderr, "\nTrieIsAllTaken() function didn't work properly\n");

		TrieDestroy(trie);
		
		return TRIE_ERROR;
	}

	TrieGet(trie, 0);
	TrieGet(trie, 1);
	TrieGet(trie, 2);
	TrieGet(trie, 3);

	if (1 != TrieIsAllTaken(trie))
	{
		fprintf(stderr, "\nTrieIsAllTaken() function didn't work properly\n");

		TrieDestroy(trie);
		
		return TRIE_ERROR;
	}

	TrieDestroy(trie);

	return TRIE_NO_ERROR;
}

static int AutomateTests(void)
{
	int status = 0;
	int i = 0;

	trie_test_func_ty tests[] = 
	{
		TrieCreateDestroyTest,
		TrieGetTest,
		TrieFreeTest,
		TrieCountAvailableTest,
		TrieIsAllTakenTest
	};
	
	const char* test_func_names[] =
	{
		"TrieCreateDestroyTest",
		"TrieGetTest",
		"TrieFreeTest",
		"TrieCountAvailableTest",
		"TrieIsAllTakenTest"
    };

	for (i = 0; i < TESTS_NUM; ++i)
    {
    	printf("\nTest N.%d - %s:", i, test_func_names[i]);
        status = tests[i]();
        printf("\n-- Test function return value: %d, ", status);
        
        if (TRIE_NO_ERROR != status)
        {
        	printf("not passed --\n\n");

        	return TRIE_ERROR;
        }

        else
        {
            printf("passed! --\n\n");
        }
    } 

    return TRIE_NO_ERROR;
}