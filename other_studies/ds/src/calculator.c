#include <stddef.h>/* size_t */
#include <assert.h> /* assert */
#include <string.h> /* strlen */
#include <stdlib.h> /* strtod */
#include <stdio.h> /* fprintf, printf */

#include "stack.h"
#include "utils.h"

enum
{
	IS_INT = 0,
	OPER_LUT_SIZE = 6,
	TESTS_NUM = 6,
	NUM_EVENTS = 7,
	NUM_STATES = 8,
	FIRST_OP_ASCII = 42,
	LOWEST_OP_ASCII = 46,
	ASCII_RANGE = 256
};

enum
{
	CALC_NO_ERROR = 0,
	CALC_ERROR = 1
};


enum events
{
	DIGIT = 0,
	OP = 1,
	SPACE = 2, 
	EQUAL = 3, 
	END_E = 4, 
	OTHER = 5,
	DOT = 6
};

typedef enum
{
	BEFORE_NUM = 0,
	MID_NUM = 1,
	AFTER_NUM = 2,
	FLOAT_START = 3,
	FLOAT_MID = 4,
	CALC = 5,
	END = 6,
	ERROR = 7
} state_name_ty;



typedef struct
{
	stack_ty *num_stack;
	stack_ty *op_stack;
	double number;
	size_t float_counter;
} calc_ty;

typedef	int (*state_func_ty)(calc_ty*, char);
typedef	double (*op_func_ty)(double, double);

typedef struct
{
	state_name_ty id;
	state_func_ty func;
} state_ty;

typedef struct
{
	int priority;
	op_func_ty func;
} op_ty;


static int OnFirstDigit(calc_ty *calc_, char curr_char_);
static int OnDigit(calc_ty *calc_, char curr_char_);
static int OnFloatDigit(calc_ty *calc_, char curr_char_);
static int OnOperator(calc_ty *calc_, char curr_char_);
static int OnSpace(calc_ty *calc_, char curr_char_);
static int OnEqual(calc_ty *calc_, char curr_char_);
static int OnEnd(calc_ty *calc_, char curr_char_);
static int OnError(calc_ty *calc_, char curr_char_);
static int OnDot(calc_ty *calc_, char curr_char_);

static double AddOp(double first_num_, double second_num_);
static double SubOp(double first_num_, double second_num_);
static double MultOp(double first_num_, double second_num_);
static double DivOp(double first_num_, double second_num_);


static int CharToEventIMP(char ch_);
static op_ty *CharToOpIMP(char ch_);
static int PriorityCmpIMP(const op_ty *first_op_, const op_ty *second_op_);
static void NumIMP(calc_ty *calc_, double curr_digit_, int is_int_);
static double CharToDoubleIMP(char curr_char_);
static double CalcPresdenceExprIMP(calc_ty *calc_, const op_ty *op_);


/*returns nan if bad expression*/
double Calculate(const char *expression_);

static int TestCalculate(void);


static const state_ty fsm_states[NUM_STATES][NUM_EVENTS] =
{
	                /*DIGIT*/                   /*OP*/                    /*SPACE*/              /*EQUAL*/         /*END*/           /*OTHER*/          /*DOT*/
	/*BEFORE_NUM*/ 	{{MID_NUM, OnFirstDigit},   {ERROR, OnError},         {BEFORE_NUM, OnSpace}, {ERROR, OnError}, {ERROR, OnError}, {ERROR, OnError}, {ERROR, OnError}},
	/*MID_NUM*/     {{MID_NUM, OnError},        {BEFORE_NUM, OnOperator}, {AFTER_NUM, OnSpace},  {CALC, OnEqual},  {ERROR, OnError}, {ERROR, OnError}, {FLOAT_START, OnDot}},
	/*AFTER_NUM*/   {{ERROR, OnError},          {BEFORE_NUM, OnOperator}, {AFTER_NUM, OnSpace},  {CALC, OnEqual},  {ERROR, OnError}, {ERROR, OnError}, {ERROR, OnError}},     
	/*FLOAT_START*/ {{FLOAT_MID, OnFloatDigit}, {ERROR, OnError},         {ERROR, OnError},      {ERROR, OnError}, {ERROR, OnError}, {ERROR, OnError}, {ERROR, OnError}},
	/*FLOAT_MID*/   {{FLOAT_MID, OnFloatDigit}, {BEFORE_NUM, OnOperator}, {AFTER_NUM, OnSpace},  {CALC, OnEqual},  {ERROR, OnError}, {ERROR, OnError}, {ERROR, OnError}},
	/*CALC*/        {{ERROR, OnError},          {ERROR, OnError},         {CALC, OnSpace},       {ERROR, OnError}, {END, OnEnd},     {ERROR, OnError}, {ERROR, OnError}},
	/*END*/         {{ERROR, OnError},          {ERROR, OnError},         {ERROR, OnError},      {ERROR, OnError}, {ERROR, OnError}, {ERROR, OnError}, {ERROR, OnError}},
	/*ERROR*/       {{ERROR, OnError},          {ERROR, OnError},         {ERROR, OnError},      {ERROR, OnError}, {ERROR, OnError}, {ERROR, OnError}, {ERROR, OnError}}
};

/* create events LUT */
static const int char_to_event_lut[ASCII_RANGE] =
{
	/* OTHER - is 5 */
	/*NULL-0*/ END_E,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	/*32*/ SPACE,
	5,5,5,5,5,5,5,5,5,
	/* '*' - 42*/ OP, /*'+' - 43*/ OP,5,/*'-' - 45*/ OP,/*'.' - 46*/ DOT, /*'/' - 47*/ OP,
	/*0 - 48*/ DIGIT,/*1 - 49*/ DIGIT,/*2 - 50*/ DIGIT,/*3 - 51*/ DIGIT,
	/*4 - 52*/ DIGIT,/*5 - 53*/ DIGIT,/*6 - 54*/ DIGIT,/*7 - 55*/ DIGIT,
	/*8 - 56*/ DIGIT,/*9 - 57*/ DIGIT,5,5,5,/*'=' - 61*/ EQUAL,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5
};

/* create operations LUT with her functions */
static op_ty oper_lut[OPER_LUT_SIZE] =
{
	{2, MultOp},
	{1, AddOp},
	{0, NULL},
	{1, SubOp},
	{0, NULL},
	{2, DivOp}
};

double nan = 0.0/0;

int IsNanIMP(double x)
{
    return (x != x);
}

double PowIMP(double base, int exp)
 {
    double res = 1.0;
    int i = 0;

    if (exp >= 0) 
    {
        for (i = 0; i < exp; ++i)
        {
            res *= base;
        }
    } 

    else 
    {
        for (i = 0; i > exp; --i)
        {
            res /= base;
        }
    }

    return res;
}

int main()
{
	int test_status = CALC_NO_ERROR;

	test_status = TestCalculate();

	return test_status;
}

static int CharToEventIMP(char ch_)
{
	/* return proper event from events LUT according to ch_ */
	return char_to_event_lut[(int)ch_];
}

static op_ty *CharToOpIMP(char ch_)
{
	/* assert */
	assert(41 < (int)ch_);
	assert(48 > (int)ch_);

	/* return proper op_ty from operations LUT according to ch_ */
	return &oper_lut[(int)ch_ - FIRST_OP_ASCII]; 
}

double Calculate(const char *expression_)
{
	calc_ty calc;
	state_ty curr_state;
	size_t stack_max_capacity = 0;
	int curr_event = 0;
	int status = CALC_NO_ERROR;
	void *calc_res = NULL;

	/* assert */
	assert(NULL != expression_);

	/* create calc */
	stack_max_capacity = strlen(expression_);
	calc.num_stack = StackCreate(stack_max_capacity, sizeof(double)); /* my TODO: add to utils void* and sizeof(double) check */

	if (NULL == calc.num_stack)
	{
		return nan;
	}

	calc.op_stack = StackCreate(stack_max_capacity, sizeof(op_ty));

	/* NULL fail handler (destroy first stack if second failed) */
	if (NULL == calc.op_stack) 
	{
		StackDestroy(calc.num_stack);

		return nan;
	}

	/* curr_state = {BEFORE_NUM, NULL} */
	curr_state.id = BEFORE_NUM;

	/* while curr_state.id != END && curr_state.id != ERROR */
	while (END != curr_state.id && ERROR != curr_state.id) /* my TODO: '\0' != *expression_ needed??? */ 
	{ 
		/* curr_event = CharToEventIMP(curr_char) */
		curr_event = CharToEventIMP(*expression_);

		/* curr_state = fsm_states[curr_state.id][curr_event]*/
		curr_state = fsm_states[curr_state.id][curr_event];

		/* status = curr_state.func(calc, curr_char) */
		status = curr_state.func(&calc, *expression_);

		/* if status != CALC_NO_ERROR */
		if (CALC_NO_ERROR != status)
		{
			/* destroy calc */
			StackDestroy(calc.num_stack);
			StackDestroy(calc.op_stack);

			/* return nan */
			return nan;
		}

		/* get next char */
		++expression_;
	}

	/* save calc_res */
	calc_res = StackPeek(calc.num_stack);

	/* destroy calc */
	StackDestroy(calc.num_stack);
	StackDestroy(calc.op_stack);

	/* return calc_res */
	return *(double *)calc_res;
}

static int OnFirstDigit(calc_ty *calc_, char curr_char_)
{
	/* assert */
	assert(NULL != calc_);
	assert(NULL != calc_->num_stack);
	assert(NULL != calc_->op_stack);

	/* initialize calculator flags */
	calc_->number = 0;
	calc_->float_counter = 0;

	/* call to OnDigit func */
	OnDigit(calc_, curr_char_);

	/* return CALC_NO_ERROR */
	return CALC_NO_ERROR;
}

static double CharToDoubleIMP(char curr_char_)
{
	char healp_str[2] = {0};
	double curr_digit = 0;	

	healp_str[0] = curr_char_;
	healp_str[1] = '\0';

	/* convert the char to double number */
	curr_digit = strtod(healp_str, NULL);

	return curr_digit;
}

static int OnDigit(calc_ty *calc_, char curr_char_)
{
	double curr_digit = 0;	

	/* assert */
	assert(NULL != calc_);
	assert(NULL != calc_->num_stack);
	assert(NULL != calc_->op_stack);

	/* CharToDoubleIMP(curr_char_) */
	curr_digit = CharToDoubleIMP(curr_char_);

	/* NumIMP(calc_, curr_digit, IS_INT) */
	NumIMP(calc_, curr_digit, IS_INT);

	return CALC_NO_ERROR;
}

static int OnFloatDigit(calc_ty *calc_, char curr_char_)
{
	double curr_digit = 0;	

	/* assert */
	assert(NULL != calc_);
	assert(NULL != calc_->num_stack);
	assert(NULL != calc_->op_stack);


	/* CharToDoubleIMP(curr_char_) */
	curr_digit = CharToDoubleIMP(curr_char_);

	/* ++calc_->float_counter */
	++calc_->float_counter;

	/* add to calc_->number the frequence value */
	NumIMP(calc_, curr_digit, !IS_INT);

	return CALC_NO_ERROR;
}

static int PriorityCmpIMP(const op_ty *first_op_, const op_ty *second_op_)
{
	/* return first_op_->priority - second_op_->priority */
	return (first_op_->priority - second_op_->priority);
}

static void NumIMP(calc_ty *calc_, double curr_digit_, int is_int_)
{
	/* if IS_INT == is_int_ */
	if (IS_INT == is_int_)
	{
		/* calc_->number = ((calc_->number * 10) + curr_number_) */
		calc_->number = ((calc_->number * 10) + curr_digit_);
	}

	/* else */
	{
		/* calc_->number = (calc_->number + (curr_number * PowIMP(10, calc_->float_counter))) */
		calc_->number = (calc_->number + (curr_digit_ / PowIMP(10, calc_->float_counter)));
	}
}

static double CalcPresdenceExprIMP(calc_ty *calc_, const op_ty *op_)
{
	const op_ty *pop_op = NULL;
	void *first_num = NULL;
	void *second_num = NULL;
	double calc_value = 0;

	/* assert */
	assert(NULL != calc_);
	assert(NULL != calc_->num_stack);
	assert(NULL != calc_->op_stack);
	assert(NULL != op_);

	/* calc_value = nan */
	calc_value = nan;

	/* while calc_->op_stack not empty && PriorityCmpIMP(peek_op, op_) >= 0 */
	while (!StackIsEmpty(calc_->op_stack) && 
		   PriorityCmpIMP((op_ty *)StackPeek(calc_->op_stack), op_) >= 0)
	{
		/* num1 = pop(calc_->num_stack) */
		first_num = StackPeek(calc_->num_stack);
		StackPop(calc_->num_stack);

		/* num2 = pop(clc_->num_stack) */
		second_num = StackPeek(calc_->num_stack);
		StackPop(calc_->num_stack);

		/* peek_op = pop(calc_->op_stack) */
		pop_op = (op_ty *)StackPeek(calc_->op_stack);
		StackPop(calc_->op_stack);

		/* calc_value = calculate */
		calc_value = pop_op->func(*(double *)first_num, *(double *)second_num);
	}

	/* return calc_value */
	return calc_value;
}

static int OnOperator(calc_ty *calc_, char curr_char_)
{
	op_ty *op = NULL;
	double calc_value = 0;

	/* assert */
	assert(NULL != calc_);
	assert(NULL != calc_->num_stack);
	assert(NULL != calc_->op_stack);

	/* push calc_->number to num stack */
	StackPush(calc_->num_stack, &calc_->number);                                    /* my TODO: can do ERRORS ?? */

	/* set calc_ number and float_counter to zero */
	calc_->number = 0;
	calc_->float_counter = 0;                              

	/* op = CharToOpIMP(curr_char_) */
	op = CharToOpIMP(curr_char_);

	/* calc_value = CalcPresdenceExprIMP(calc_, op) */
	calc_value = CalcPresdenceExprIMP(calc_, op);

	/* if calc_value != nan */
	if (!IsNanIMP(calc_value))
	{
		/* push calc_value to calc_->num_stack */
		StackPush(calc_->num_stack, &calc_value);
	}

	/* push op to calc_->op_stack */
	StackPush(calc_->op_stack, (void *)op);

	/* return CALC_NO_ERROR */
	return CALC_NO_ERROR;
}

static int OnSpace(calc_ty *calc_, char curr_char_)
{
	/* assert */
	assert(NULL != calc_);
	assert(NULL != calc_->num_stack);
	assert(NULL != calc_->op_stack);
	assert(curr_char_);

	/* do nothing */

	/* return CALC_NO_ERROR */
	return CALC_NO_ERROR;
}

static int OnEqual(calc_ty *calc_, char curr_char_)
{
	op_ty *op = NULL;

	/* assert */
	assert(NULL != calc_);
	assert(NULL != calc_->num_stack);
	assert(NULL != calc_->op_stack);
	assert(curr_char_);

	/* op = CharToOpIMP(curr_char_) */
	op = CharToOpIMP((char)LOWEST_OP_ASCII);

	/* CalcPresdenceExprIMP(calc_, op) */
	CalcPresdenceExprIMP(calc_, op);

	/* return CALC_NO_ERROR */
	return CALC_NO_ERROR;
}


static int OnEnd(calc_ty *calc_, char curr_char_)
{
	/* assert */
	assert(NULL != calc_);
	assert(NULL != calc_->num_stack);
	assert(NULL != calc_->op_stack);
	assert(curr_char_);

	/* return CALC_NO_ERROR */
	return CALC_NO_ERROR;
}

static int OnError(calc_ty *calc_, char curr_char_)
{
	/* assert */
	assert(NULL != calc_);
	assert(NULL != calc_->num_stack);
	assert(NULL != calc_->op_stack);
	assert(curr_char_);

	/* fprintf ERROR message to stderr */
	DEBUG_ONLY(fprintf(stderr, "\nThe expression is not MID_NUM.\n");)

	/* return CALC_NO_ERROR */
	return CALC_NO_ERROR;
}

static int OnDot(calc_ty *calc_, char curr_char_)
{
	/* assert */
	assert(NULL != calc_);
	assert(NULL != calc_->num_stack);
	assert(NULL != calc_->op_stack);
	assert(curr_char_);

	/* do nothing */

	/* return CALC_NO_ERROR */
	return CALC_NO_ERROR;
}

static double AddOp(double first_num_, double second_num_)
{
	return (first_num_ + second_num_);
}

static double SubOp(double first_num_, double second_num_)
{
	return (first_num_ - second_num_);
}

static double MultOp(double first_num_, double second_num_)
{
	return (first_num_ * second_num_);
}

static double DivOp(double first_num_, double second_num_)
{
	assert(0 != second_num_);

	return (first_num_ / second_num_); 
}

static int TestCalculate(void)
{
	int i = 0;
	double calc_res = nan;

	char *test_expressions[] = 
	{
		"3   +5   -9  =",
		"4*8-      5  =",
		"6/ 3 +5 -  2/ 4 =",
		"6 --1=",
		"7?=",
		"1+1",
	};

	double expected[3] =
	{
		-1,
		27,
		6.5
	};

	for (i = 0; i < TESTS_NUM; ++i)
	{
    	printf("\nTest N.%d:", i);
        calc_res = Calculate(test_expressions[i]);

        if (i <= 2)
        {
	        if (calc_res != expected[i])
	        {
	         	printf("\n-- There is an ERROR in the program --\n");
	         	
	         	return CALC_ERROR;
	        }

	        printf("\nFunction result: %f\n", calc_res);
   		 }

   		 else
   		 {
 	        if (!IsNanIMP(calc_res))
	        {
	         	printf("\n-- There is an ERROR in the program --\n");
	         	
	         	return CALC_ERROR;
	        }

	        printf("\nFunction result: %f\n", calc_res);  		 	
   		 }
	}

	return CALC_NO_ERROR;
}