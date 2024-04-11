#define _POSIX_C_SOURCE 200809L

#include <fcntl.h> /* O_CREAT, O_EXCL */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h> /* semaphore functions */
#include <stdio.h> /* fprintf */
#include <assert.h> /* assert */
#include <errno.h> /* errno */
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "keepwatching.h"

int CopyParams(char **params_)
{
	int params_argc = 0;
	int i = 0;

	assert(NULL != params_);

	params_argc = atoi(params_[3]);

	g_other_program_info = (char **)malloc((params_argc + 5) * sizeof(char *));

	if (NULL == g_other_program_info)
    {
        fprintf(stderr, "\nFailed to allocate memory for params array of strings.\n");
        
        return ERROR;
    }

	for (i = 0; i < params_argc + 4; ++i)
    {
    	g_other_program_info[i] = (char *)malloc((1 + strlen(params_[i])) * sizeof(char));

        if (NULL == g_other_program_info[i])
    	{
        	fprintf(stderr, "\nFailed to allocate memory for argument in the array of strings..\n");
        
        	return ERROR;
    	}

    	strcpy(g_other_program_info[i], params_[i]);
    }

    g_other_program_info[i] = NULL;

    return NO_ERROR;
}

int main(int argc, char *argv[])
{
	char new_env_var_value[MAX_STR_LEN] = {0};
	int i = 0;
	printf("**************IN WD***************\n");

	assert(NULL != argv);
			for(i=0;i<atoi(argv[3])+4;++i)
			{
				printf("WD args - %s\n",argv[i]);
			}

	CopyParams(argv);

	for (i = 0; i < 1 + 4; ++i)
	{
		printf("g_other_program_info[i] = %s\n", g_other_program_info[i]);
	}

	/* update WD_PID env variable */
	sprintf(new_env_var_value, "%d", getpid());

	if (0 != setenv("WD_PID", new_env_var_value, 1))
	{
		fprintf(stderr, "\nThere was an error in set the env variable.\n");

		return errno;
	}

	/* open the semaphore */
	if (NO_ERROR != CreateSem())
	{
		return errno;
	}

	/* post the semaphore */
	if (-1 == sem_post(g_is_wd_exist_sem))
	{
		fprintf(stderr, "\nThere was an error in post the semaphore.\n");

		return ERROR;
	}

	/* call KeepWatching */
	KeepWatchingWraper((void *)argv);

	return 0;
}
