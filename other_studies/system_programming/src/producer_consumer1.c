#include <stddef.h> /* size_t */
#include <unistd.h> /* sleep */
#include <pthread.h> /* thread functions */
#include <stdio.h> /* printf, fprintf */

#define BUFFER_SIZE 5

volatile int g_can_produce = 1;
volatile int g_can_consume = 0;

int g_buffer[BUFFER_SIZE] = {0};

void *ProducerRoutine()
{
	int curr_produce_num = 1;
	size_t i = 0;

	/* endless loop */
	while (1)
	{
		/* busy wait until the g_can_produce will be true */
		while (!g_can_produce);

		printf("\nStart producing!\n");

		i = 0;

		/* produce data to g_buffer */
		while (i < BUFFER_SIZE)
		{
			g_buffer[i] = curr_produce_num;

			++curr_produce_num;
			++i;
		}

		sleep(2);

		/* adjust the threads global flags */
		__sync_lock_release(&g_can_produce);
		__sync_lock_test_and_set(&g_can_consume, 1);
	}

	return NULL;
}

void *ConsumerRoutine()
{
	size_t i = 0;

	/* endless loop */
	while (1)
	{
		/* busy wait until the g_can_consume will be true */
		while (!g_can_consume);

		printf("\nStart consuming!\n");

		i = 0;

		/* print the value from the buffer to the user */
		while (i < BUFFER_SIZE)
		{
			printf("%d ", g_buffer[i]);

			++i;
		}

		printf("\n");

		sleep(2);

		/* adjust the threads global flags */
		__sync_lock_test_and_set(&g_can_produce, 1);
		__sync_lock_release(&g_can_consume);	
	}

	return NULL;
}

int main()
{
	pthread_t produce_thread;
    pthread_t consume_thread;

	/* create thread to execute Producer */
	if (0 != pthread_create(&produce_thread, NULL, ProducerRoutine, NULL))
	{
        fprintf(stderr, "\nThere was an error in creating the producer thread.\n");

        return 1;
    }

	/* create thread to execute Consumer */
	if (0 != pthread_create(&consume_thread, NULL, ConsumerRoutine, NULL))
	{
        fprintf(stderr, "\nThere was an error in creating the consumer thread.\n");

        return 1;
    }

	/* wait until the Consumer thread will end */
	if (0 != pthread_join(consume_thread, NULL))
	{
        fprintf(stderr, "There was an error in joining consumer thread.\n");

        return 1;
    }

    /* wait until the Producer thread will end */
	if (0 != pthread_join(produce_thread, NULL))
	{
        fprintf(stderr, "There was an error in joining producer thread.\n");

        return 1;
    }

	return 0;
}