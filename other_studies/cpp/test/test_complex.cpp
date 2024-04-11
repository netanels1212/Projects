#include <iostream>

#include "complex.hpp"

using namespace std;
using namespace HRD33;

const int G_TEST_NUM = 7;

enum
{
    NO_ERROR = 0,
    ERROR = 1
};

typedef int (*complex_test_func_ty)(void);

static int AddTest(void);
static int SubTest(void);
static int MultTest(void);
static int DivTest(void);
static int IsEqualTest(void);
static int IsNotEqualTest(void);
static int InputOpTest(void);

static int AutomateTests(void);

int main()
{
    int complex_status = NO_ERROR;

    complex_status = AutomateTests();

    if (NO_ERROR != complex_status)
    {
        cout << "\n *** There is an ERROR in the program ***\n";

        return ERROR;
    }

    cout << "\n *** All the tests passed, the program work fine ***\n";

    return NO_ERROR;
}

static int AddTest(void)
{
    Complex c1(5, 3);
    Complex c2(1, 2);

    cout << c1 << " + " << c2 << " -> " << (c1 + c2) << endl;

    c1 += c2;

    cout << c1 << " += " << c2 << " -> " << c1 << endl;

    if (6 != c1.GetReal() || 5 != c1.GetImaginary())
    {
        return ERROR;
    }

    return NO_ERROR;
}

static int SubTest(void)
{
    Complex c1(5, 2);
    Complex c2(1, 3);

    cout << c1 << " - " << c2 << " -> " << (c1 - c2) << endl;

    c1 -= c2;

    cout << c1 << " -= " << c2 << " -> " << c1 << endl;

    if (4 != c1.GetReal() || -1 != c1.GetImaginary())
    {
        return ERROR;
    }

    return NO_ERROR;
}

static int MultTest(void)
{
    Complex c1(5, 2);
    Complex c2(1, 3);

    cout << c1 << " * " << c2 << " -> " << (c1 * c2) << endl;

    c1 *= c2;

    cout << c1 << " *= " << c2 << " -> " << c1 << endl;

    if (5 != c1.GetReal() || 6 != c1.GetImaginary())
    {
        return ERROR;
    }

    return NO_ERROR;
}

static int DivTest(void)
{
    Complex c1(5, 4);
    Complex c2(1, 2);

    cout << c1 << " / " << c2 << " -> " << (c1 / c2) << endl;

    c1 /= c2;

    cout << c1 << " /= " << c2 << " -> " << c1 << endl;

    if (5 != c1.GetReal() || 2 != c1.GetImaginary())
    {
        return ERROR;
    }

    return NO_ERROR;
}

static int IsEqualTest(void)
{
    Complex c1(5, 2);
    Complex c2(1, 3);
    bool cmp_res = 0;

    cmp_res = (c1 == c2);

    cout << c1 << " == " << c2 << " -> " << cmp_res << endl;

    if (false != cmp_res)
    {
        return ERROR;
    }

    cmp_res = (c1 == c1);

    cout << c1 << " == " << c1 << " -> " << cmp_res << endl;

    if (true != cmp_res)
    {
        return ERROR;
    }

    return NO_ERROR;
}

static int IsNotEqualTest(void)
{
    Complex c1(5, 2);
    Complex c2(1, 3);
    bool cmp_res = 0;

    cmp_res = (c1 != c2);

    cout << c1 << " != " << c2 << " -> " << cmp_res << endl;

    if (true != cmp_res)
    {
        return ERROR;
    }

    cmp_res = (c1 != c1);

    cout << c1 << " != " << c1 << " -> " << cmp_res << endl;

    if (false != cmp_res)
    {
        return ERROR;
    }

    return NO_ERROR;
}

static int InputOpTest(void)
{
    Complex input_num;

    cout << "Enter your super complex number!\n";
    cout << "First enter 'real' part and then 'imag' part\n";

    cin >> input_num;

    cout << "You enter: " << input_num << endl;

    return NO_ERROR;
}

static int AutomateTests(void)
{
	int status = 0;
	int i = 0;

	complex_test_func_ty tests[] = 
	{
        AddTest,
        SubTest,
        MultTest,
        DivTest,
        IsEqualTest,
        IsNotEqualTest,
        InputOpTest
	};
	
	const char* test_func_names[] =
	{
        "AddTest",
        "SubTest",
        "MultTest",
        "DivTest",
        "IsEqualTest",
        "IsNotEqualTest",
        "InputOpTest"
    };

	for (i = 0; i < G_TEST_NUM; ++i)
    {
    	cout << "\nTest N." << i << " - " << test_func_names[i] << ":\n";
        status = tests[i]();
        cout << "\n-- Test function return value: " << status <<',';
        
        if (NO_ERROR != status)
        {
        	cout << " not passed --\n\n";

        	return ERROR;
        }

        else
        {
            cout << " passed! --\n\n";
        }
    } 

    return NO_ERROR;
}