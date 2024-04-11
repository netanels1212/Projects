#include <semaphore.h> /* semaphore functions */
#include <fcntl.h> /* O_CREAT */
#include <assert.h> /* assert */
#include <errno.h> /* errno */
#include <string.h> /* strcmp, strlen */
#include <stdio.h> /* fprintf, printf, scanf, sscanf */

#define MAX_COMMAND_LEN 30

int g_undo_counter = 0;

static char GetCmdIMP(char *whole_cmd_)
{
	char cmd_name_check[MAX_COMMAND_LEN] = {0};
	int is_valid_cmd = 0;

	assert(NULL != whole_cmd_);

    while (0 == is_valid_cmd)
    {
	    printf("\nEnter your command:\n");
	    scanf(" %[^\n]", whole_cmd_);

	    sscanf(whole_cmd_, "%1s", cmd_name_check);

	    if (1 != strlen(cmd_name_check))
	    {
	    	printf("\nThere is no such command.\n");
	    }

	    else
	    {
	    	is_valid_cmd = 1;
	    }
    }

    return whole_cmd_[0];
}

static int IsValidCmdValueIMP(int value_)
{
	if (0 > value_)
	{
		printf("\nThere is no option to enter negative command value. please try again...\n");

		return 0;
	}

	return 1;
}

static int SemDestroyIMP(sem_t *sem_, char *sem_name_)
{
	assert(NULL != sem_);

	if (-1 == sem_close(sem_))
	{
		fprintf(stderr, "\nsemaphore closing failed.\n");

		return errno;
	}

	if (-1 == sem_unlink(sem_name_)) 
	{
		fprintf(stderr, "\nsemaphore unlinking failed\n");

		return errno;
	}

	return 0;	
}

static void AdjustSemIMP(sem_t *sem_, int cmd_value_, char op_)
{
    while (0 < cmd_value_)
    {
        if ('D' == op_)
        {
            sem_wait(sem_);
        }

        else if ('I' == op_)
        {
            sem_post(sem_);
        }

        --cmd_value_;
    }
}

static int ExecuteSemOpIMP(sem_t *sem_, char *is_undo_, int cmd_value_, char op_)
{
	if (!IsValidCmdValueIMP(cmd_value_))
	{
	    return 1;
	}

	if(0 == strcmp(is_undo_, "undo"))
	{
		if ('D' == op_)
        {
	    	g_undo_counter -= cmd_value_;
        }

        else if ('I' == op_)
        {
	    	g_undo_counter += cmd_value_;
        }
	} 

	AdjustSemIMP(sem_, cmd_value_, op_);

	return 0;

}

static void UndoIMP(sem_t *sem_)
{
    int sem_value = 0;

    assert(NULL != sem_);

    sem_getvalue(sem_, &sem_value);

    while (0 < g_undo_counter)
    {
        sem_wait(sem_);
        --g_undo_counter;
    }

    while (0 > g_undo_counter)
    {
    	sem_post(sem_);
        ++g_undo_counter;
    }
}

int main(int argc, char* argv[])
{
	sem_t *sem = NULL;
	char whole_cmd[MAX_COMMAND_LEN] = {0};
	char is_undo[5] = {0};
	char cmd_op = 0;
    int cmd_value = 0;
    int sem_value = 0;

	assert(NULL != argv);
	assert(2 == argc);

	sem = sem_open(argv[1], O_CREAT | O_EXCL, 0666, 1);

	if (SEM_FAILED == sem)
	{
        fprintf(stderr, "\nsemaphore creation failed.\n");

        return errno;
    }

    while (1)
    {
    	cmd_op = GetCmdIMP(whole_cmd);

	    switch (cmd_op)
	    {
	    	case 'X':
	    		UndoIMP(sem);

				if (-1 == sem_getvalue(sem, &sem_value))
				{
				    printf("\nFailed to get semaphore value.\n");

					return errno;
				}

				printf("the semaphore value at Exit is: %d\n", sem_value);
	    	    		
	    		printf("\ngood bye!\n");

				return SemDestroyIMP(sem, argv[1]);

	        case 'V':
				if (-1 == sem_getvalue(sem, &sem_value))
				{
				    printf("\nFailed to get semaphore value.\n");

					return errno;
				}

				printf("the semaphore value is: %d\n", sem_value);

	        	break;

	        case 'D':
        		sscanf(whole_cmd, "%*s %d %s", &cmd_value, is_undo);
        		ExecuteSemOpIMP(sem, is_undo, cmd_value, 'D');       		

	        	break;

	        case 'I':
        		sscanf(whole_cmd, "%*s %d %s", &cmd_value, is_undo);
        		ExecuteSemOpIMP(sem, is_undo, cmd_value, 'I');    

	        	break;

	        default:
	        	printf("\nThere is no such operation. please try again...\n");

	        	break;
	    }
	}

	return 0;
}