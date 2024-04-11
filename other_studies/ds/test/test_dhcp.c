#include <stddef.h> /* size_t */
#include <stdio.h> /* printf, fprintf */

#include "dhcp.h"

enum
{
	DHCP_NO_ERROR = 0,
	DHCP_ERROR,
	TESTS_NUM = 5
};

typedef int (*dhcp_test_func_ty)(void);


static int DhcpCreateDestroyTest(void);
static int DhcpAllocateIPTest(void);
static int DhcpFreeIPTest(void);
static int DhcpCountFreeTest(void);
static int DhcpIsAllTakenTest(void);

static int AutomateTests(void);

int main()
{
	int dhcp_status = DHCP_NO_ERROR;
	
	dhcp_status = AutomateTests();
	
	if (DHCP_NO_ERROR != dhcp_status)
	{
		printf("\n *** There is an ERROR in the program ***\n");
		
		return DHCP_ERROR;
	}
	
	printf("\n *** All the tests passed, the program work fine ***\n");
	
	return DHCP_NO_ERROR;
}

static int DhcpCreateDestroyTest(void)
{
	dhcp_ty *dhcp = DhcpCreate(0xffffff00, 0xf5f5f500);

	if (NULL == dhcp)
	{
		fprintf(stderr, "\nDhcpCreate() function didn't work properly\n");
		
		return DHCP_ERROR;
	}

	DhcpDestroy(dhcp);

	return DHCP_NO_ERROR;
}

static int DhcpAllocateIPTest(void)
{
	dhcp_ty *dhcp = DhcpCreate(0xffffff00, 0xf5f5f500);
	ip_ty alloc_output = 0;

	alloc_output = DhcpAllocateIP(dhcp, 4126537068);

	if (4126537068 != alloc_output)
	{
		fprintf(stderr, "\nDhcpAllocateIP() function didn't work properly\n");

		DhcpDestroy(dhcp);
		
		return DHCP_ERROR;
	}

	alloc_output = DhcpAllocateIP(dhcp, 4126537068);

	if (4126537069 != alloc_output)
	{
		fprintf(stderr, "\nDhcpAllocateIP() function didn't work properly\n");

		DhcpDestroy(dhcp);
		
		return DHCP_ERROR;
	}

	DhcpDestroy(dhcp);

	return DHCP_NO_ERROR;
}

static int DhcpFreeIPTest(void)
{
	dhcp_ty *dhcp = DhcpCreate(0xffffff00, 0xf5f5f500);
	ip_ty alloc_output = 0;

	DhcpFreeIP(dhcp, 4126537068);
	DhcpAllocateIP(dhcp, 4126537068);
	DhcpFreeIP(dhcp, 4126537068);
	alloc_output = DhcpAllocateIP(dhcp, 4126537068);

	if (4126537068 != alloc_output)
	{
		fprintf(stderr, "\nDhcpFreeIP() function didn't work properly\n");

		DhcpDestroy(dhcp);
		
		return DHCP_ERROR;
	}

	DhcpDestroy(dhcp);

	return DHCP_NO_ERROR;
}

static int DhcpCountFreeTest(void)
{
	dhcp_ty *dhcp = DhcpCreate(0xffffff00, 0xf5f5f500);

	if (253 != DhcpCountFree(dhcp))
	{
		fprintf(stderr, "\nDhcpCountFree() function didn't work properly\n");

		DhcpDestroy(dhcp);
		
		return DHCP_ERROR;
	}

	DhcpAllocateIP(dhcp, 4126537068);

	if (252 != DhcpCountFree(dhcp))
	{
		fprintf(stderr, "\nDhcpCountFree() function didn't work properly\n");

		DhcpDestroy(dhcp);
		
		return DHCP_ERROR;
	}

	DhcpDestroy(dhcp);

	return DHCP_NO_ERROR;
}

static int DhcpIsAllTakenTest(void)
{
	dhcp_ty *dhcp = DhcpCreate(0xfffffff0, 0xf5f5f550);
	size_t i = 0;

	if (0 != DhcpIsAllTaken(dhcp))
	{
		fprintf(stderr, "\nDhcpIsAllTaken() function didn't work properly\n");

		DhcpDestroy(dhcp);
		
		return DHCP_ERROR;
	}

	for (i = 0; i < 16; ++i)
	{                        
		DhcpAllocateIP(dhcp, 0xF5F5F551);
	}

	if (1 != DhcpIsAllTaken(dhcp))
	{
		fprintf(stderr, "\nDhcpIsAllTaken() function didn't work properly\n");

		DhcpDestroy(dhcp);
		
		return DHCP_ERROR;
	}

	DhcpDestroy(dhcp);

	return DHCP_NO_ERROR;
}

static int AutomateTests(void)
{
	int status = 0;
	int i = 0;

	dhcp_test_func_ty tests[] = 
	{
		DhcpCreateDestroyTest,
		DhcpAllocateIPTest,
		DhcpFreeIPTest,
		DhcpCountFreeTest,
		DhcpIsAllTakenTest
	};
	
	const char* test_func_names[] =
	{
		"DhcpCreateDestroyTest",
		"DhcpAllocateIPTest",
		"DhcpFreeIPTest",
		"DhcpCountFreeTest",
		"DhcpIsAllTakenTest"
    };

	for (i = 0; i < TESTS_NUM; ++i)
    {
    	printf("\nTest N.%d - %s:", i, test_func_names[i]);
        status = tests[i]();
        printf("\n-- Test function return value: %d, ", status);
        
        if (DHCP_NO_ERROR != status)
        {
        	printf("not passed --\n\n");

        	return DHCP_ERROR;
        }

        else
        {
            printf("passed! --\n\n");
        }
    } 

    return DHCP_NO_ERROR;
}