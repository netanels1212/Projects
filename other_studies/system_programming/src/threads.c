#include <unistd.h> /* sleep */
#include <time.h> /* time_t, time */
#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free, exit */
#include <omp.h> /* openmp */

#define G_ARR_SIZE 3       /* MAX LIMIT - 61880 */
#define NUM 1000000000

int g_arr[G_ARR_SIZE] = {0};


static void *ThreadRoutine(void *indx_)
{
	unsigned long sum_of_divisors = 0;
	unsigned long start_range = 0;
	unsigned long i = 0;

	assert(NULL != indx_);

	start_range = (3333333 * (*(int *)indx_)) + 1;

	for (i = start_range; (i <= NUM) && (i < start_range + 3333333); ++i)
	{
		if (0 == NUM % i)
		{
			sum_of_divisors += i;
		}
	}

	g_arr[*(int *)indx_] = sum_of_divisors;

	free(indx_);

	return NULL;
}

int main()
{
	time_t start = time(NULL);
	time_t end;
	unsigned long general_sum = 0;
	int i = 0;

	#pragma omp parallel for
	for (i = 0; i < G_ARR_SIZE; ++i)
	{
		int *index = (int *)malloc(sizeof(int));

		if (NULL == index)
		{
			exit(1);
		}

		*index = i;
		ThreadRoutine(index);
	}

	for (i = 0; i < G_ARR_SIZE; ++i)
	{
		general_sum += g_arr[i];
	}

	end = time(NULL);

	printf("Run time: %lu\n", end - start);

	printf("\nsum of divisors of %lu is: %lu\n", NUM, general_sum);

	return 0;
}