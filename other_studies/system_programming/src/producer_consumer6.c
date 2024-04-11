#include <errno.h> /* errno */
#include <unistd.h> /* sleep */
#include <pthread.h> /* thread functions */
#include <semaphore.h> /* semaphore functions */
#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf, fprintf */

#define NUM_CONSUMERS 5


/* declare massege variable */
int massege = 0;

/* declare is_thread_in_action mutex */
pthread_mutex_t is_thread_in_action;

/* declare consumers semaphore */
sem_t consumers_sem;

/* declare condition variable */
int consumers_end_cond = NUM_CONSUMERS;

void *Producer()
{
	int i = 0;

	/* endless loop */
	while (1)
	{
		/* while the condition is true - all the consumers ends */
		if (0 == (consumers_end_cond % NUM_CONSUMERS))
		{
			/* lock the mutex */
			pthread_mutex_lock(&is_thread_in_action);

			/* create a massege */
			massege = rand() % 100;

			printf("Producer create the massege: %d\n", massege);

			/* unlock th mutex */
			pthread_mutex_unlock(&is_thread_in_action);

			/* increase the consumers semaphore to be in size of NUM_CONSUMERS */
			for (i = 0; i < NUM_CONSUMERS; ++i)
			{
				sem_post(&consumers_sem);
			}

			/* sleep */
			sleep(2);
		}
	}

	/* return NULL */
	return NULL;
}

void *Consumer(void *consumer_id_)
{
	/* endless loop */
	while (1)
	{
		/* decrese consumers semaphore */
		sem_wait(&consumers_sem);

		/* lock the mutex */
		pthread_mutex_lock(&is_thread_in_action);

		/* consume the massege */
		printf("Consumer %d consume the massege %d\n", *(int *)consumer_id_, massege);

		/* adjust the condition variable */
		++consumers_end_cond;

		/* unlock the mutex */
		pthread_mutex_unlock(&is_thread_in_action);

		/* sleep */
		sleep(2);
	}

	/* return NULL */
	return NULL;
}

int main()
{
	pthread_t producer;
	pthread_t consumers[NUM_CONSUMERS] = {0};
	int *consumer_id = NULL;
	int i = 0;

	/* initialize the consumers semaphore */
	if (0 != sem_init(&consumers_sem, 0, 0))
	{
		fprintf(stderr, "\nError in create consumers semaphore.\n");

		return 1;
	}

	/* create the producer thread */
	if (0 != pthread_create(&producer, NULL, Producer, NULL))
	{
		fprintf(stderr, "\nThere was an error in creating the producer threads.\n");

		return 1;
	}

	/* create the consumers threads */
	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		consumer_id = (int *)malloc(sizeof(int));

		if (NULL == consumer_id)
		{
			fprintf(stderr, "\nError in allocation memory for consumer id.\n");

			return 1;
		}

		*consumer_id = i;

		if (0 != pthread_create(&consumers[i], NULL, Consumer, consumer_id))
		{
			fprintf(stderr, "\nThere was an error in creating the consumer threads.\n");

			return 1;
		}
	}

	/* wait until all the threads will be over */
	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		if (0 != pthread_join(consumers[i], NULL))
		{
	        fprintf(stderr, "There was an error in joining consumers threads.\n");

	        return 1;
		}
	}

	if (0 != pthread_join(consumers[i], NULL))
	{
	    fprintf(stderr, "There was an error in joining producer thread.\n");

	    return 1;
	}

	/* destroy the semaphore */
	if (-1 == sem_close(&consumers_sem))
	{
		fprintf(stderr, "\nconsumer semaphore closing failed.\n");

		return errno;
	}

	return 0;
}