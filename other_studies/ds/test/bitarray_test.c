#include <stdio.h> /* printf, scanf, sprintf */
#include <stddef.h> /* size_t */
#include <string.h> /* strcmp */
#include <assert.h> /* assert */

#include "../include/bitarray.h"

static void PrintBitArray(bitarr_ty bit_array);
static int SetAllTest();
static int ResetAllTest();
static void SetOnTest();
static void SetOffTest();
static void SetBitTest();
static void GetValTest();
static void FlipBitTest();
static void MirrorTest();
static void RotRTest();
static void RotLTest();
static void CountOnTest();
static void CountOffTest();
static char* DeleteStrZero(char *str);
static int ToStringTest();
static void BitIsOffTest();
static void BitsArrLutCountOnTest();
static void BitsArrLutMirrorTest();
static void TestsMenu();

int main()
{
	TestsMenu();
	
	return 0;
}

static void PrintBitArray(bitarr_ty arr)
{
	size_t bit_to_print = 0;
	size_t mask = 1U;
	int i = 0;
	
	printf("\nThe current bit array is:\n");

	for (i = (sizeof(size_t) * BYTE) - 1; 0 <= i; --i)
	{		
		bit_to_print = (arr >> i) & mask;
		
		printf("%lu", (size_t) bit_to_print);
				
		if (0 == i % 8)
		{
			printf(" ");
		}
		
		if (32 == i)
		{
		
			printf("\n");
		}
	}
	
	printf("\n");
}

static int SetAllTest()
{
	bitarr_ty arr = 0;
	size_t mask = 1U;
	
	printf("\n--- Test for SetAll() ---\n"); 
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
	
	printf("\nAfter set all to ON:\n");
	arr = SetAll();
	PrintBitArray(arr);
	
	while (0 != (arr & mask))
	{
		arr >>= 1;
	}
	
	if (0 == arr)
	{
		printf("\n* The function is working properly *\n");
		
		return SUCCESS;
	}
	
	else
	{
		printf("\n* The function is not working properly *\n");
			
		return FAIL;
	}		
}

static int ResetAllTest()
{
	bitarr_ty arr = 0;
	
	printf("\n--- Test for ResetAll() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
	
	printf("\nAfter set all to ON:\n");
	arr = ResetAll();
	PrintBitArray(arr);
	
	if (0 == arr)
	{
		printf("\n* The function is working properly *\n");
		
		return SUCCESS;
	}
	
	else
	{
		printf("\n* The function is not working properly *\n");
			
		return FAIL;
	}		
}

static void SetOnTest()
{
	bitarr_ty arr = 0;
	size_t index = 0;
	
	printf("\n--- Test for SetOn() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	
	PrintBitArray(arr);
	
	printf("Enter the bit index to set ON: ");
	scanf("%lu", &index);
	
	printf("\nAfter set the bit ON:\n");
	
	arr = SetOn(arr, index);
	PrintBitArray(arr);
}

static void SetOffTest()
{
	bitarr_ty arr = 0;
	size_t index = 0;
	
	printf("\n--- Test for SetOff() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
	
	printf("Enter the bit index to set OFF: ");
	scanf("%lu", &index);
	
	printf("\nAfter set the bit OFF:\n");
	
	arr = SetOff(arr, index);
	PrintBitArray(arr);
}

static void SetBitTest()
{
	bitarr_ty arr = 0;
	size_t index = 0;
	int boolean_value = 0;
	
	printf("\n--- Test for SetBit() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
	
	printf("Enter the bit index to set: ");
	scanf("%lu", &index);
	printf("Enter the boolean value you want to set: ");
	scanf("%d", &boolean_value);
	
	if (0 == boolean_value)
	{
		arr = SetAll();
	}
	
	if (1 == boolean_value)
	{
		arr = ResetAll();
	}
	
	printf("\nAfter set the bit:\n");
	
	arr = SetBit(arr, index, boolean_value);
	PrintBitArray(arr);
}

static void GetValTest()
{
	bitarr_ty arr = 0;
	size_t index = 0;
	int bit_value = 0;
	
	printf("\n--- Test for GetVal() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
		
	printf("\nEnter the bit position to get: ");
	scanf("%lu", &index);
	
	bit_value = GetVal(arr, index);
	printf("\nThe bit value is: %d\n", bit_value);
}

static void FlipBitTest()
{
	bitarr_ty arr = 0;
	size_t index = 0;
	
	printf("\n--- Test for FlipBit() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
		
	printf("\nEnter the bit position to flip: ");
	scanf("%lu", &index);
	
	arr = FlipBit(arr, index);
	PrintBitArray(arr);
}

static void MirrorTest()
{
	bitarr_ty arr = 0;
	
	printf("\n--- Test for Mirror() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
	
	printf("\nAfter mirror:\n");
	arr = Mirror(arr);
	PrintBitArray(arr);
}

static void RotRTest()
{
	bitarr_ty arr = 0;
	size_t shift = 0;
	
	printf("\n--- Test for RotR() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
	
	printf("\nEnter number of bits to shift right: ");
	scanf("%lu", &shift);
	arr = RotR(arr, shift);
	printf("\nAfter shift right:\n");
	PrintBitArray(arr);
}

static void RotLTest()
{
	bitarr_ty arr = 0;
	size_t shift = 0;
	
	printf("\n--- Test for RotL() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
	
	printf("\nEnter number of bits to shift left: ");
	scanf("%lu", &shift);
	arr = RotL(arr, shift);
	printf("\nAfter shift left:\n");
	PrintBitArray(arr);
}

static void CountOnTest()
{
	bitarr_ty arr = 0;
	size_t count_set_on = 0;
	
	printf("\n--- Test for CountOn() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
	
	printf("\nThe number of ON bits in this number is: ");
	count_set_on = CountOn(arr);
	printf("%lu\n", count_set_on);
}

static void CountOffTest()
{
	bitarr_ty arr = 0;
	size_t count_set_off = 0;
	
	printf("\n--- Test for CountOff() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
	
	printf("\nThe number of OFF bits in this number is: ");
	count_set_off = CountOff(arr);
	printf("%lu\n", count_set_off);
}

static char* DeleteStrZero(char *str)
{
	assert(NULL != str);
	
	while ('0' == *str)
	{
		++str;
	}
	
	return str;
}

static int ToStringTest()
{
	bitarr_ty arr = 0;
	char func_res[MAX_STR_SIZE] = {0};
	char origin_func_res[MAX_STR_SIZE] = {0};
	
	printf("\n--- Test for ToString() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
	
	printf("\nFunc res: ");
	ToString(arr, func_res); 
	printf("%s\n", func_res);
	
	printf("\nOrigin func res: ");
	sprintf(origin_func_res, "%b", arr); 
	printf("%s\n", origin_func_res);
	
	if (EQUAL != strcmp(DeleteStrZero(func_res), origin_func_res))
	{
		printf("\n* The function is not working properly *\n");
		
		return FAIL;
	}
	
	printf("\n* The function is working properly *\n");
	
	return SUCCESS;
}

static void BitIsOffTest()
{
	bitarr_ty arr = 0;
	size_t index = 0;
	int bit_value = 0;
	
	printf("\n--- Test for BitIsOff() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
		
	printf("\nEnter the bit position to check: ");
	scanf("%lu", &index);
	
	bit_value = BitIsOff(arr, index);
	printf("BitIsOff() func res (1 - TRUE, 0 - FALSE): %d\n", bit_value);
}

static void BitsArrLutCountOnTest()
{
	bitarr_ty arr = 0;
	size_t count_set_on = 0;
	
	printf("\n--- Test for BitsArrLutCountOn() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
	
	printf("\nThe number of ON bits in this number is: ");
	count_set_on = BitsArrLutCountOn(arr);
	printf("%lu\n", count_set_on);
}

static void BitsArrLutMirrorTest()
{
	bitarr_ty arr = 0;
	
	printf("\n--- Test for BitsArrLutMirror() ---\n");
	
	printf("Enter bit array value (decimal number): ");
	scanf("%lu", &arr);
	PrintBitArray(arr);
	
	printf("\nAfter LUT mirror:\n");
	arr = BitsArrLutMirror(arr);
	PrintBitArray(arr);
}

static void TestsMenu()
{
	int choice = 0;
	
	choice = 1;
	printf("\n --- Tests Menu: ---\n");
	printf("\n1. SetAllTest()\n");
	printf("2. ResetAllTest()\n");
	printf("3. SetOnTest()\n");
	printf("4. SetOffTest()\n");
	printf("5. SetBitTest()\n");
	printf("6. GetValTest()\n");
	printf("7. FlipBitTest()\n");
	printf("8. MirrorTest()\n");
	printf("9. RotRTest()\n");
	printf("10. RotLTest()\n");
	printf("11. CountOnTest()\n");
	printf("12. CountOffTest()\n");
	printf("13. ToStringTest()\n");
	printf("14. BitIsOffTest()\n");
	printf("15. BitsArrLutCountOnTest()\n");
	printf("16. BitsArrLutMirrorTest()\n");
	printf("0. Exit\n\n");
	
	while (choice != EXIT)
	
	{
		printf("Enter your choice: ");
		scanf("%d", &choice);
		switch (choice)
		
		{
		case 1:
			printf("\n");
			SetAllTest();
			printf("\n");
			break;		
		case 2:
			printf("\n");
			ResetAllTest();
			printf("\n");
			break;
		case 3:
			printf("\n");
			SetOnTest();
			printf("\n");
			break;
		case 4:
			printf("\n");
			SetOffTest();
			printf("\n");
			break;
		case 5:
			printf("\n");
			SetBitTest();
			printf("\n");
			break;
		case 6:
			printf("\n");
			GetValTest();
			printf("\n");
			break;
		case 7:
			printf("\n");
			FlipBitTest();
			printf("\n");
			break;
		case 8:
			printf("\n");
			MirrorTest();
			printf("\n");
			break;
		case 9:
			printf("\n");
			RotRTest();
			printf("\n");
			break;
		case 10:
			printf("\n");
			RotLTest();
			printf("\n");
			break;
		case 11:
			printf("\n");
			CountOnTest();
			printf("\n");
			break;
		case 12:
			printf("\n");
			CountOffTest();
			printf("\n");
			break;
		case 13:
			printf("\n");
			ToStringTest();
			printf("\n");
			break;
		case 14:
			printf("\n");
			BitIsOffTest();
			printf("\n");
			break;
		case 15:
			printf("\n");
			BitsArrLutCountOnTest();
			printf("\n");
			break;
		case 16:
			printf("\n");
			BitsArrLutMirrorTest();
			printf("\n");
			break;
		case 0:
			break;
		default:
			printf("Invalid choice, please try again...\n");
			break;
		}
	}
}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
