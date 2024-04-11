#include <assert.h> /* assert */
#include <stdio.h> /* printf, fprintf */

#include "scheduler.h"

#define TESTS_NUM 6

typedef sched_status_ty (*scheduler_test_function_ty)();

/*************** Operations Functions **************************/
static int PrintIntOpFunc(void *op_params);
static int PrintSchedSizeOpFunc(void *op_params);
static int PrintSchedIsEmptyOpFunc(void *op_params);
static int ClearOpFunc(void *op_params);

/*************** Automate Tests Functions **********************/
static sched_status_ty SchedCreateDestroyTest(void);
static sched_status_ty SchedAddTaskTest(void);
static sched_status_ty SchedRemoveTaskTest(void);
static sched_status_ty SchedSizeTest(void);
static sched_status_ty SchedIsEmptyTest(void);
static sched_status_ty SchedClearTest(void);

static sched_status_ty AutomateTests(void);

int main()
{
	sched_status_ty scheduler_status = 0;
	
	scheduler_status = AutomateTests();
	
	if (SCHED_NO_ERROR != scheduler_status)
	{
		fprintf(stderr, "\n *** There is an ERROR in the program ***\n");
		
		return SCHED_ERROR;
	}
	
	printf("\n *** All the tests passed, the program work fine ***\n");
	
	return SCHED_NO_ERROR;
}

static int PrintIntOpFunc(void *op_params)
{
	assert(NULL != op_params);

	printf("\nThe number is: %d\n", *(int *)op_params);

	return 1;
}

static int PrintSchedSizeOpFunc(void *op_params)
{
	assert(NULL != op_params);

	printf("\nThe size of the scheduler is: %lu",
	                                            SchedSize((sched_ty*)op_params));

	return 0;
}

static int PrintSchedIsEmptyOpFunc(void *op_params)
{
	assert(NULL != op_params);

	if (1 == SchedIsEmpty((sched_ty*)op_params)) /* TODO: magic? */
	{
		printf("\nThe scheduler is empty.\n");

		return 1;
	}

	printf("\nThe scheduler isn't empty.\n");

	return 1;
}

static int ClearOpFunc(void *op_params)
{
	assert(NULL != op_params);

	SchedClear((sched_ty*)op_params);

	return 0;
}

static sched_status_ty SchedCreateDestroyTest(void)
{
	sched_ty *scheduler = NULL;

	scheduler = SchedCreate();

	if (NULL == scheduler)
	{
		fprintf(stderr, "\n*** The creation of the scheduler failed ***\n");
		
		return SCHED_ERROR;
	}

	SchedDestroy(scheduler);
	scheduler = NULL;

	return SCHED_NO_ERROR;
}

static sched_status_ty SchedAddTaskTest(void)
{
	sched_ty *scheduler = NULL;
	sched_op_func_ty print_func = PrintIntOpFunc;
	sched_op_func_ty stop_func = (sched_op_func_ty) SchedStop;
	int first_print_op_param = 0;
	int second_print_op_param = 1;
	time_t first_print_func_interval = 1;
	time_t second_print_func_interval = 2;
	time_t stop_func_interval = 3;
	size_t scheduler_size_after_add = 0;


	scheduler = SchedCreate();

	SchedAddTask(scheduler, print_func,(void *) &first_print_op_param, 
		                                              first_print_func_interval);
	SchedAddTask(scheduler, print_func,(void *) &second_print_op_param, 
		                                             second_print_func_interval);
	SchedAddTask(scheduler, stop_func,(void *) scheduler, stop_func_interval);

	SchedRun(scheduler);

	SchedAddTask(scheduler, stop_func, scheduler, stop_func_interval);
	SchedRun(scheduler);

	scheduler_size_after_add = SchedSize(scheduler);

	SchedDestroy(scheduler);
	scheduler = NULL;

	if (2 != scheduler_size_after_add) /* TODO: magic */
	{
		fprintf(stderr, "\nThe function SchedAddTask() didn't work propely.\n");

		return SCHED_ERROR;
	}

	return SCHED_NO_ERROR;
}

static sched_status_ty SchedRemoveTaskTest(void)
{
	sched_ty *scheduler = NULL;
	sched_op_func_ty print_func = PrintIntOpFunc;
	sched_op_func_ty stop_func = (sched_op_func_ty) SchedStop;
	uid_ty first_print_func_uid;
	int first_print_op_param = 0;
	int second_print_op_param = 1;
	time_t first_print_func_interval = 1;
	time_t second_print_func_interval = 2;
	time_t stop_func_interval = 3;
	size_t scheduler_size_after_add = 0;


	scheduler = SchedCreate();

	first_print_func_uid = SchedAddTask(scheduler,
	       print_func,(void *) &first_print_op_param, first_print_func_interval);
	SchedAddTask(scheduler, print_func,(void *) &second_print_op_param, 
		                                             second_print_func_interval);
	SchedAddTask(scheduler, stop_func,(void *) scheduler, stop_func_interval);

	SchedRun(scheduler);

	SchedRemoveTask(scheduler, first_print_func_uid);

	SchedAddTask(scheduler, stop_func,(void *) scheduler, stop_func_interval);
	SchedRun(scheduler);

	scheduler_size_after_add = SchedSize(scheduler);

	SchedDestroy(scheduler);
	scheduler = NULL;

	if (1 != scheduler_size_after_add) /* TODO: magic */
	{
		fprintf(stderr, "\nThe function SchedRemoveTask() didn't work propely.\n");

		return SCHED_ERROR;
	}

	return SCHED_NO_ERROR;
}

static sched_status_ty SchedSizeTest(void)
{
	sched_ty *scheduler = NULL;
	sched_op_func_ty print_size_func = PrintSchedSizeOpFunc;
	sched_op_func_ty stop_func = (sched_op_func_ty) SchedStop;
	time_t print_func_interval = 1;
	time_t stop_func_interval = 2;
	size_t scheduler_size = 0;


	scheduler = SchedCreate();

	SchedAddTask(scheduler, print_size_func,(void *) scheduler,
	                                                        print_func_interval);
	SchedAddTask(scheduler, stop_func,(void *) scheduler, stop_func_interval);

	SchedRun(scheduler);

	scheduler_size = SchedSize(scheduler);

	SchedDestroy(scheduler);
	scheduler = NULL;

	if (0 != scheduler_size) /* TODO: magic */
	{
		fprintf(stderr, "\nThe function SchedSize() didn't work propely.\n");

		return SCHED_ERROR;
	}

	return SCHED_NO_ERROR;
}

static sched_status_ty SchedIsEmptyTest(void)
{
	sched_ty *scheduler = NULL;
	sched_op_func_ty print_is_empty_func = PrintSchedIsEmptyOpFunc;
	sched_op_func_ty stop_func = (sched_op_func_ty) SchedStop;
	time_t print_func_interval = 1;
	time_t stop_func_interval = 2;
	int scheduler_is_empty = 0;


	scheduler = SchedCreate();

	scheduler_is_empty = SchedIsEmpty(scheduler);

	if (0 == scheduler_is_empty) /* TODO: magic */
	{
		fprintf(stderr, "\nThe function SchedIsEmpty() didn't work propely.\n");


		SchedDestroy(scheduler);
		scheduler = NULL;

		return SCHED_ERROR;
	}


	SchedAddTask(scheduler, print_is_empty_func,(void *) scheduler,
	                                                        print_func_interval);
	SchedAddTask(scheduler, stop_func,(void *) scheduler, stop_func_interval);

	SchedRun(scheduler);

	scheduler_is_empty = SchedIsEmpty(scheduler);

	SchedDestroy(scheduler);
	scheduler = NULL;

	if (1 == scheduler_is_empty) /* TODO: magic */
	{
		fprintf(stderr, "\nThe function SchedIsEmpty() didn't work propely.\n");

		return SCHED_ERROR;
	}

	return SCHED_NO_ERROR;
}

static sched_status_ty SchedClearTest(void)
{
	sched_ty *scheduler = NULL;
	sched_op_func_ty print_func = PrintIntOpFunc;
	sched_op_func_ty clear_func = ClearOpFunc;
	sched_op_func_ty stop_func = (sched_op_func_ty) SchedStop;
	time_t print_func_interval = 1;
	time_t clear_func_interval = 2;
	time_t stop_func_interval = 2;
	int print_op_param = 1;
	int scheduler_is_empty = 0;


	scheduler = SchedCreate();

	SchedAddTask(scheduler, print_func, (void *) &print_op_param,
	                                                        print_func_interval);
	SchedAddTask(scheduler, clear_func,(void *) scheduler, clear_func_interval);
	SchedAddTask(scheduler, stop_func,(void *) scheduler, stop_func_interval);

	SchedRun(scheduler);

	scheduler_is_empty = SchedIsEmpty(scheduler);

	SchedDestroy(scheduler);
	scheduler = NULL;

	if (0 == scheduler_is_empty) /* TODO: magic */
	{
		fprintf(stderr, "\nThe function SchedClear() didn't work propely.\n");

		return SCHED_ERROR;
	}

	return SCHED_NO_ERROR;
}

static sched_status_ty AutomateTests(void)
{
	sched_status_ty scheduler_status = SCHED_NO_ERROR;
	int i = 0;

	scheduler_test_function_ty tests[] = 
	{
		SchedCreateDestroyTest,
		SchedAddTaskTest,
		SchedRemoveTaskTest,
		SchedSizeTest,
		SchedIsEmptyTest,
		SchedClearTest
	};
	
	const char* test_func_names[] =
	{
        "SchedCreateDestroyTest",
        "SchedAddTaskTest",
        "SchedRemoveTaskTest",
        "SchedSizeTest",
        "SchedIsEmptyTest",
        "SchedClearTest"
    };

	for (i = 0; i < TESTS_NUM; ++i)
    {
    	printf("\nTest N.%d - %s:", i, test_func_names[i]);
        scheduler_status = tests[i]();
        printf("\n-- Test function return value: %d --\n\n",
                                                         scheduler_status);
        
        if (SCHED_NO_ERROR != scheduler_status)
        {
        	return SCHED_ERROR;
        }
    } 

    return SCHED_NO_ERROR;
}
