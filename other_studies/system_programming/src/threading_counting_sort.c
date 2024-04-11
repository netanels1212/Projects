#include <stddef.h> /* size_t */
#include <pthread.h> /* threads functions */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* printf, fprintf, fopen, fclose, fgetc */

#define ARR_SIZE 100000
#define HISTOGRAM_SIZE 256 /* ASCII range */
#define THREADS_NUM 1

size_t g_histogram[HISTOGRAM_SIZE] = {0};
int g_arr[ARR_SIZE] = {0};
pthread_mutex_t lock;

typedef struct
{
	int *arr;
	size_t size;
} thread_params;



void InitArrIMP(void)
{
	size_t i = 0;

	for (i = 0; i < ARR_SIZE; ++i)
	{
		g_arr[i] = rand() % 10; /* the %10 is the size of the histograms */
	}
}

int Exercise2InitIMP(void)
{
	FILE *file = NULL;
	size_t word_index = 0;

	file = fopen("/usr/share/dict/words", "rb");

	if (file == NULL) 
    {
        fprintf(stderr, "\nError to opening the file.\n");

        return 1;
    }

	while(word_index < ARR_SIZE)
    {
        int ch = fgetc(file);

        if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            g_arr[word_index] = ch;

            ++word_index;
        }
    }

    fclose(file);

    return 0;
}

void PrintArrInAsciiFormat()
{
	size_t i = 0;

	for (i = 0; i < ARR_SIZE; ++i)
	{
		printf("%c ", (char)g_arr[i]);
	}

	printf("\n");
}

void *CountOccurrence(void *params_)
{
	size_t i = 0;
	thread_params *params = NULL;
	size_t local_histogram[HISTOGRAM_SIZE];
	pthread_mutex_init(&lock, NULL);


	/* assert */
	assert(NULL != params_);

	params = params_;


	/* count the number of occurrence of each element in this range to his place in the histogram */
	for (i = 0; i < params->size; ++i)
	{
        local_histogram[params->arr[i]] += 1; 
	}

	pthread_mutex_lock(&lock); 
  
	for (i = 0; i < HISTOGRAM_SIZE; ++i)
	{
		g_histogram[i] += local_histogram[i];
	}
  
    pthread_mutex_unlock(&lock);


	/* return NULL */
	return NULL;
}

int CountSort(void)
{
	int *output_arr = NULL;
	size_t i = 0;

	/* allocate memory for the output array */
	output_arr = (int *)malloc(sizeof(int) * ARR_SIZE);

	/* memory allocation failure handler */
	if (NULL == output_arr)
	{
		fprintf(stderr, "\nMemory allocation for output arr failed.\n");

		return 1; /* failure */
	}

	/* for each element in the array from the end */
	for (i = ARR_SIZE - 1; (int)i >= 0; --i)
	{
		/* output_arr[histogram[curr element] - 1] = curr element */
		output_arr[g_histogram[g_arr[i]] - 1] = g_arr[i];

        --g_histogram[g_arr[i]];
	}

	/* copy the output array to the input array */ 
	for (i = 0; i < ARR_SIZE; ++i)
	{
		g_arr[i] = output_arr[i];
	}

	/* free output arr */
	free(output_arr);

	/* return success */
	return 0;
}

int IsSorted(void)
{
	size_t i = 0;

	/* for each element in the array */
	for (i = 0; i < (ARR_SIZE - 1); ++i)
	{
		/* if there is a element that is bigger than the next element */
		if (g_arr[i] > g_arr[i + 1])
		{
			/* return false */
			return 0;
		}

	}

	/* return true */
	return 1;
}


int main()
{
	/* declare array of threads in size of THREADS_NUM */
	pthread_t threads[THREADS_NUM] = {0};
	thread_params params[THREADS_NUM] = {0};
	size_t chunks_size = ARR_SIZE / THREADS_NUM;
	size_t i = 0;

	/* init global input array */
	/* InitArrIMP(); */
	Exercise2InitIMP();

	PrintArrInAsciiFormat();

	/* create the threads to fill the histogram */
	for (i = 0; i < THREADS_NUM; ++i)
	{
		if ((THREADS_NUM - 1) == i)
		{
			params[i].size = ARR_SIZE - ((THREADS_NUM - 1) * chunks_size);
		}

		else
		{
			params[i].size = chunks_size;
		}

		params[i].arr = g_arr + i * chunks_size;

		if (0 != pthread_create(&threads[i], NULL, CountOccurrence, &params[i])) 
        {
            fprintf(stderr, "There is an error in create thread.\n");

            return 1;
        }
	}

	/* wait untill all the threads end to execute */
	for (i = 0; i < THREADS_NUM; ++i)
	{
		if (0 != pthread_join(threads[i], NULL)) 
        {
            fprintf(stderr, "There is an error in wait for thread.\n");

            return 1;
        }
    }

	/* adjust g_histogram */
	for (i = 0; i < HISTOGRAM_SIZE - 1; ++i)
	{
		g_histogram[i + 1] += g_histogram[i];
	}

	/* do counting sort with the histogram */
	CountSort();

	/* check that the sort work fine */
	if (!IsSorted())
	{
		printf("\nProgram not work.. :(\n");

		return 1;
	}

	PrintArrInAsciiFormat();

	printf("\nProgram work!!! :)\n");


	return 0;
}
