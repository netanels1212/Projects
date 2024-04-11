#include <stddef.h> /* size_t */
#include <errno.h> /* errno */
#include <fcntl.h> /* O_CREAT */
#include <assert.h> /* assert */
#include <pthread.h> /* thread functions */
#include <semaphore.h> /* semaphore functions */
#include <stdlib.h> /* malloc, free */
#include <unistd.h> /* sleep */
#include <stdio.h> /* printf, fprintf */

#include "singlylinkedlist.h"

#define PRODUCERS_NUM 5
#define CONSUMERS_NUM 5

/* declare a global list */
slist_ty *g_produced_data = NULL;

/* declare a mutex */
pthread_mutex_t g_is_thread_in_action;

/* declare list size semaphore */
sem_t g_slist_size_sem;

void *ProducerRoutine(void *value_to_produce_)
{
	/* assert */
	assert(NULL != value_to_produce_);

	/* endless loop */
	while (1)
	{
		/* lock the mutex */
		pthread_mutex_lock(&g_is_thread_in_action);

		/* produce the specific produce_value_ to the slist */
		printf("\nProducer %d start producing!\n", *(int *)value_to_produce_);

		SListPushBack(g_produced_data, value_to_produce_);

		/* unlock the mutex */
		pthread_mutex_unlock(&g_is_thread_in_action);

		/* increase slsit size semaphore */
		sem_post(&g_slist_size_sem);

		/* sleep */
		sleep(2);
	}

	/* return NULL */
	return NULL;
}

void *ConsumerRoutine(void *consumer_id_)
{
	int pop_status = 0;

	/* endless loop */
	while (1)
	{
		/* while the slist not empty */
		sem_wait(&g_slist_size_sem);

		/* lock the mutex */
		pthread_mutex_lock(&g_is_thread_in_action);

		/* while the slist not empty */
		printf("\nConsumer %d start consuming!\n", *(int *)consumer_id_);

		/* print the value from the slist to the user */
		printf("%d ", (*(int *)(SListGetData(SListBegin(g_produced_data)))));

		/* pop front the current element from the slist */
		pop_status = SListPopFront(g_produced_data);

		if (SUCCESS != pop_status)
		{
			exit(1);
		}

		/* unlock the mutex */
		pthread_mutex_unlock(&g_is_thread_in_action);

		/* sleep */
		sleep(2);
	}

	/* return NULL */
	return NULL;
}

int main()
{
	/* declare pthread array for the thread */
	pthread_t threads[PRODUCERS_NUM + CONSUMERS_NUM] = {0};
	int *value_to_produce = NULL;
	int *consumer_id = NULL;
	size_t i = 0;

	/* create the global slist */
	g_produced_data = SListCreate();

	/* creation failure handler */
	if (NULL == g_produced_data)
	{
		return 1;
	}

	/* create and initialize slist size semaphore */
	if (0 != sem_init(&g_slist_size_sem, 0, 0))
	{
        fprintf(stderr, "\nslist semaphore creation failed.\n");

        return errno;
    }

	/* create producers and consumers threads */
	for (i = 0; i < PRODUCERS_NUM; ++i)
	{
		value_to_produce = (int *)malloc(sizeof(int));

		if (NULL == value_to_produce)
		{
			return 1;
		}

		*value_to_produce = i;

		if (0 != pthread_create(&threads[i], NULL, ProducerRoutine, value_to_produce))
		{
			fprintf(stderr, "\nThere was an error in creating the producer threads.\n");

			return 1;
		}
	}

	for (i = 0; i < CONSUMERS_NUM; ++i)
	{
		consumer_id = (int *)malloc(sizeof(int));

		if (NULL == consumer_id)
		{
			return 1;
		}

		*consumer_id = i;
		
		if (0 != pthread_create(&threads[PRODUCERS_NUM + i], NULL, ConsumerRoutine, consumer_id))
		{
			fprintf(stderr, "\nThere was an error in creating the consumer threads.\n");

			return 1;
		}
	}

	/* wait until all the threads end their jobs */
	for (i = 0; i < (PRODUCERS_NUM + CONSUMERS_NUM); ++i)
	{
		if (0 != pthread_join(threads[i], NULL))
		{
	        fprintf(stderr, "There was an error in joining thread.\n");

	        return 1;
	    }
	}

	/* destroy slist */
	SListDestroy(g_produced_data);

	/* destroy producer semaphore */
	if (-1 == sem_close(&g_slist_size_sem))
	{
		fprintf(stderr, "\nslist semaphore closing failed.\n");

		return errno;
	}

	return 0;
}