#include <stddef.h> /* size_t */
#include <errno.h> /* errno */
#include <assert.h> /* assert */
#include <pthread.h> /* thread functions */
#include <semaphore.h> /* semaphore functions */
#include <stdlib.h> /* malloc, free */
#include <unistd.h> /* sleep */
#include <stdio.h> /* printf, fprintf */

#define PRODUCERS_NUM 5
#define CONSUMERS_NUM 5
#define BUFFER_CAPACITY 5

typedef struct
{
    int produced_data[BUFFER_CAPACITY];
    pthread_mutex_t write_mutex;
    pthread_mutex_t read_mutex;
    sem_t producer_sem;
    sem_t consumer_sem; 
    size_t read_idx;
    size_t write_idx;
} fsq_ty;


void FsqInit(fsq_ty *fsq_)
{
	/* assert */
	assert(NULL != fsq_);

	/* init all the fsq fields */
	fsq_->read_idx = 0;
    fsq_->write_idx = 0;
    pthread_mutex_init(&fsq_->write_mutex, NULL);
    pthread_mutex_init(&fsq_->read_mutex, NULL);
    sem_init(&fsq_->producer_sem, 0, 0);
    sem_init(&fsq_->consumer_sem, 0, PRODUCERS_NUM);
}

void FsqEnqueue(fsq_ty *fsq_, int data_)
{
	/* assert */
	assert(NULL != fsq_);

	/* decrease consumer semaphore */
	sem_wait(&fsq_->consumer_sem);

	/* lock the write mutex */
    pthread_mutex_lock(&fsq_->write_mutex);

	/* wrote to the fsq with atomic operation */
    __sync_lock_test_and_set(&fsq_->produced_data[fsq_->write_idx], data_);

	/* adjust write_idx */
    fsq_->write_idx = (fsq_->write_idx + 1) % BUFFER_CAPACITY;

	/* unlock the write mutex */
    pthread_mutex_unlock(&fsq_->write_mutex);

	/* increase producer semaphore */
	sem_post(&fsq_->producer_sem);
}

int FsqDequeue(fsq_ty *fsq_)
{
	int read_data = 0;

	/* assert */
	assert(NULL != fsq_);

	/* decrease producer semaphore */
	sem_wait(&fsq_->producer_sem);

	/* lock the read mutex */
    pthread_mutex_lock(&fsq_->read_mutex);

	/* read in atomic operation from the buffer */
    __sync_lock_test_and_set(&read_data, fsq_->produced_data[fsq_->read_idx]);

    /* adjust read index */
    fsq_->read_idx = (fsq_->read_idx + 1) % BUFFER_CAPACITY;

	/* unlock the read mutex */
    pthread_mutex_unlock(&fsq_->read_mutex);

	/* increase the consumer semaphore */
    sem_post(&fsq_->consumer_sem);


	/* return the data was read */
	return read_data;
}

void *ProducerRoutine(void *fsq_)
{
	int data = 0;

	/* assert */
	assert(NULL != fsq_);

	/* endless loop */
	while (1)
	{
		/* create random number */
        data = rand() % 100;
        printf("Produce: %d\n", data);

		/* enqueue the number to the fsq */
		FsqEnqueue((fsq_ty *)fsq_, data);

		/* sleep */
		sleep(2);
	}

	/* return NULL */
	return NULL;
}

void *ConsumerRoutine(void *fsq_)
{
	int read_data = 0;

	/* assert */
	assert(NULL != fsq_);

	/* endless loop */
	while (1)
	{
		/* dequeue from the fsq */
		read_data = FsqDequeue((fsq_ty *)fsq_);

		/* print the data to the user */
	    printf("Consumed: %d\n", read_data);
	
		/* sleep */
		sleep(2);
	}

	/* return NULL */
	return NULL;
}

int main()
{
	pthread_t threads[PRODUCERS_NUM + CONSUMERS_NUM] = {0};
	int i = 0;

	/* allocate memory for the fsq */
	fsq_ty *fsq = (fsq_ty *)malloc(sizeof(fsq_ty));

	/* allocation failure handler */
	if (NULL == fsq)
	{
		fprintf(stderr, "\nMemory allocation for the fsq failed.\n");

		return 1;
	}

	/* init the fsq */
	FsqInit(fsq);

	/* create producer and consumer threads */
	for (i = 0; i < PRODUCERS_NUM; ++i)
	{
		if (0 != pthread_create(&threads[i], NULL, ProducerRoutine, fsq))
		{
			fprintf(stderr, "\nThere was an error in creating the producer threads.\n");

			return 1;
		}
	}

	for (i = 0; i < CONSUMERS_NUM; ++i)
	{
		if (0 != pthread_create(&threads[PRODUCERS_NUM + i], NULL, ConsumerRoutine, fsq))
		{
			fprintf(stderr, "\nThere was an error in creating the consumer threads.\n");

			return 1;
		}
	}

	/* wait for threads to finish */
	for (i = 0; i < (PRODUCERS_NUM + CONSUMERS_NUM); ++i)
	{
		if (0 != pthread_join(threads[i], NULL))
		{
	        fprintf(stderr, "There was an error in joining thread.\n");

	        return 1;
		}
	}   

	/* close semaphores */
	if (-1 == sem_close(&fsq->producer_sem))
	{
		fprintf(stderr, "\nproducer semaphore closing failed.\n");

		return errno;
	}

	/* destroy consumer semaphore */
	if (-1 == sem_close(&fsq->consumer_sem))
	{
		fprintf(stderr, "\nconsumer semaphore closing failed.\n");

		return errno;
	}

	/* free memory allocation for the fsq */
	free(fsq);

	return 0;
}