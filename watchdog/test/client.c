#define _POSIX_C_SOURCE 200809L

#include <errno.h> /* errno */
#include <stdlib.h> /* setenv, getenv, atoi */
#include <stdio.h>  /* printf, fprintf, sprintf */

#include "watchdog.h"

#define MAX_STR_LEN 30



int main(int argc, char *argv[])
{
	int i = 0;
/*	int num = 1000;
	int curr_env_var_value = 0;
	char *env_var_name = "MAX_REVIVES";
	char *env_var_value = "5";
	char new_env_var_value[MAX_STR_LEN] = {0};*/

/*	if (0 != setenv(env_var_name, env_var_value, 1))
	{
		fprintf(stderr, "\nThere was an error in set the revive env variable.\n");

		return errno;
	}*/

	MakeMeImmortal(argc, argv, 3, 10);
/*
	printf("Running!\n");

	curr_env_var_value = atoi(getenv(env_var_name));

	printf("MAX_REVIVES env variable = %d", curr_env_var_value);

	for (i = 0; i < num; ++i)
	{
		if ((rand() % 1000) == i)
		{
			--curr_env_var_value;
			sprintf(new_env_var_value, "%d", curr_env_var_value);
			if (0 != setenv(env_var_name, new_env_var_value, 1))
			{
				fprintf(stderr, "\nThere was an error in set the env variable.\n");

				return errno;
			}

			printf("crash on i=%d\n", i);

			if (0 == curr_env_var_value)
			{
				exit(1);
			}
		}
	}*/

	while (i < 100000)
	{
		++i;
	};

	StopWD();

/*	if (-1 == unsetenv("MAX_REVIVES"))
	{
		fprintf(stderr, "\nThere was an error in delete the env MAX_REVIVES variable.\n");

		return errno;
	}*/

	return 0;
}