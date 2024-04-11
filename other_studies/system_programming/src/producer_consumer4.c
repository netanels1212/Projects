#include <stddef.h> /* size_t */
#include <errno.h> /* errno */
#include <fcntl.h> /* O_CREAT */
#include <assert.h> /* assert */
#include <pthread.h> /* thread functions */
#include <semaphore.h> /* semaphore functions */
#include <stdlib.h> /* malloc, free */
#include <unistd.h> /* sleep */
#include <stdio.h> /* printf, fprintf */

#include "queue.h"

#define PRODUCERS_NUM 5
#define CONSUMERS_NUM 5


/* declare a global buffer */
queue_ty *g_produced_data = NULL;

/* declare a mutex */
pthread_mutex_t g_is_thread_in_action;

/* declare producer semaphore */
sem_t g_producer_sem;

/* declare consumer semaphore */
sem_t g_consumer_sem;



void *ProducerRoutine(void *value_to_produce_)
{
	int enqueue_status = 0;

	/* assert */
	assert(NULL != value_to_produce_);

	/* endless loop */
	while (1)
	{
		/* while buffer is not full */
		sem_wait(&g_producer_sem);

		/* lock the mutex */
		pthread_mutex_lock(&g_is_thread_in_action);

		/* produce the specific produce_value_ to the buffer */
		printf("\nProducer %d start producing!\n", *(int *)value_to_produce_);

		enqueue_status = QEnqueue(g_produced_data, value_to_produce_);

		if (QSUCCESS != enqueue_status)
		{
			fprintf(stderr, "\nError in producing the data.\n");

			free(value_to_produce_);

			return NULL;
		}

		/* unlock the mutex */
		pthread_mutex_unlock(&g_is_thread_in_action);

		/* increase consumer semaphore */
		sem_post(&g_consumer_sem);

		/* sleep */
		sleep(2);
	}

	/* return NULL */
	return NULL;
}

void *ConsumerRoutine(void *consumer_id_)
{
	/* endless loop */
	while (1)
	{
		/* while the buffer is not empty */
		sem_wait(&g_consumer_sem);
		
		/* lock the mutex */
		pthread_mutex_lock(&g_is_thread_in_action);

		printf("\nConsumer %d start consuming!\n", *(int *)consumer_id_);

		/* print the value from the queue to the user */
		printf("%d ", *(int *)QPeek(g_produced_data));

		QDequeue(g_produced_data);

		/* unlock the mutex */
		pthread_mutex_unlock(&g_is_thread_in_action);

		/* increase producer semaphore */
		sem_post(&g_producer_sem);

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

	/* create the global buffer */
	g_produced_data = QCreate();

	/* creation failure handler */
	if (NULL == g_produced_data)
	{
		return 1;
	}

    /* create and initialize producer semaphore */
	if (0 != sem_init(&g_producer_sem, 0, 5))
	{
        fprintf(stderr, "\nproducer semaphore creation failed.\n");

        return errno;
    }

	/* create and initialize consumer semaphore */
	if (0 != sem_init(&g_consumer_sem, 0, 0))
	{
        fprintf(stderr, "\nconsumer semaphore creation failed.\n");

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

	/* free buffer allocation */
	QDestroy(g_produced_data);

	/* destroy producer semaphore */
	if (-1 == sem_close(&g_producer_sem))
	{
		fprintf(stderr, "\nproducer semaphore closing failed.\n");

		return errno;
	}

	/* destroy consumer semaphore */
	if (-1 == sem_close(&g_consumer_sem))
	{
		fprintf(stderr, "\nconsumer semaphore closing failed.\n");

		return errno;
	}

	return 0;
}