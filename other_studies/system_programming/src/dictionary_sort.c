#include <stddef.h> /* size_t */
#include <pthread.h> /* threads functions */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free, qsort */
#include <stdio.h> /* printf, fprintf, fopen, fclose, fseek, fscanf */
#include <string.h> /* strcmp, strcpy, strlen */

#define DICTIONARY_SIZE 104334
#define DUP_NUM 2
#define WORDS_NUM (DICTIONARY_SIZE * DUP_NUM)
#define WORD_SIZE 50
#define THREADS_NUM 3 

typedef struct
{
	size_t chunk_number;
	size_t size;
	char **sort_chunk;
} thread_params;

char g_dictionary[WORDS_NUM][WORD_SIZE] = {0};
pthread_mutex_t lock;


int InitDictionary(void)
{
	FILE *file = NULL;
	size_t dup_index = 0;
	size_t word_index = 0;

	file = fopen("/usr/share/dict/words", "rb");

	if (file == NULL) 
    {
        fprintf(stderr, "\nError to opening the file.\n");

        return 1;
    }

    for (dup_index = 0; dup_index < DUP_NUM; ++dup_index)
    {
    	fseek(file, 0, SEEK_SET); 

    	for (word_index = 0; word_index < DICTIONARY_SIZE; ++word_index)
    	{
    		fscanf(file, "%s", g_dictionary[(dup_index * DICTIONARY_SIZE) + word_index]);
    	}
    }

    return 0;
}

void PrintDictionaryIMP(void)
{
	size_t i = 0;

	while (WORDS_NUM > i)
	{
		printf("%s\n", g_dictionary[i]);

		++i;
	}
}

int ShuffleCmpFunc(const void *data1_, const void *data2_)
{
	assert(NULL != data1_);
	assert(NULL != data2_);

	return rand()%4 - 1;
}

int SortCmpFunc(const void *data1_, const void *data2_)
{
	return strcmp((char *)data1_, (char *)data2_);
}

void ShuffleIMP(void)
{
	qsort(&g_dictionary, WORDS_NUM, WORD_SIZE, ShuffleCmpFunc);
}

void Swap(char **data1_, char **data2_)
{
	char *temp = malloc(strlen(*data1_) + 1);

	assert(NULL != data1_);
	assert(NULL != data2_);	

    strcpy(temp, *data1_);
    strcpy(*data1_, *data2_);
    strcpy(*data2_, temp);

    free(temp);
}

void BubbleSort(char **array, size_t size) 
{
    size_t i, j;

    for (i = 0; i < size - 1; ++i)
    {
        for (j = 0; j < size - i - 1; ++j)
        {
            if (0 < SortCmpFunc(&array[j], &array[j + 1]))
            {
                char* temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }	
    }
}

void SelectionSort(char **arr_, size_t size_)
 {
    size_t min_index = 0;
    size_t i = 0;
 	size_t j = 0;

    for (i = 0; i < (size_ - 1); ++i) 
    {
        min_index = i;

        for (j = i + 1; j < size_; ++j) 
        {
            if (0 > strcmp(arr_[j], arr_[min_index]))
            {
                min_index = j;
            }
        }

        Swap(arr_ + i, arr_ + min_index);
    }
}

void *SortChunk(void *params_)
{
	thread_params *params = NULL;
	size_t i = 0;

	assert(NULL != params_);

	params = params_;


	for (i = 0; i < params->size; ++i)
	{
		strcpy(params->sort_chunk[i], g_dictionary[params->chunk_number + i]);
	}


/*	qsort(params->sort_chunk, params->size, sizeof(char *), SortCmpFunc);
*/
	BubbleSort(params->sort_chunk, params->size);

	pthread_mutex_lock(&lock); 

	for (i = 0; i < params->size; ++i)
	{
		strcpy(g_dictionary[params->chunk_number + i], params->sort_chunk[i]);
	}

	pthread_mutex_unlock(&lock);


	for (i = 0; i < params->size; ++i)
	{
		free(params->sort_chunk[i]);
	}

	free(params->sort_chunk);


	return NULL;
}

int main()
{
	pthread_t threads[THREADS_NUM] = {0};
	thread_params params[THREADS_NUM] = {0};
	size_t chunk_size = WORDS_NUM/THREADS_NUM;
	size_t i = 0;
	size_t j = 0;

	char **ptr_to_ptr = (char **)malloc(WORDS_NUM * sizeof(char *));
    
	InitDictionary();
	ShuffleIMP();

	/* create the threads to sort chunks of the dictionary */
	for (i = 0; i < THREADS_NUM; ++i)
	{
		if ((THREADS_NUM - 1) == i)
		{
			params[i].size = WORDS_NUM - (i * chunk_size);
		}

		else
		{
			params[i].size = chunk_size;
		}
		
		params[i].chunk_number = i * chunk_size;

		params[i].sort_chunk = (char **)malloc((sizeof(char *)) * params[i].size);

		if (NULL == params[i].sort_chunk)
		{
			fprintf(stderr, "\nAllocation for sort chunk array failed.\n");

			return 1;
		}

		for (j = 0; j < params[i].size; ++j)
		{
			params[i].sort_chunk[j] = (char *)malloc(strlen(g_dictionary[params[i].chunk_number + j]) + 1);

			if (NULL == params[i].sort_chunk[j])
			{
				fprintf(stderr, "\nAllocation for array in sort chunk failed.\n");

				return 1;
			}
		}

		if (0 != pthread_create(&threads[i], NULL, SortChunk, &params[i])) 
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

    for (i = 0; i < WORDS_NUM; ++i)
    {
        ptr_to_ptr[i] = g_dictionary[i];
    }
/*	qsort(&g_dictionary, WORDS_NUM, WORD_SIZE, SortCmpFunc);
*/    BubbleSort(ptr_to_ptr, WORDS_NUM);

	PrintDictionaryIMP();


	return 0;
}

