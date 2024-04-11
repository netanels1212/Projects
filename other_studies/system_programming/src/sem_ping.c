#include <errno.h>           /* errno */
#include <stddef.h>          /* size_t */
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>       /* sem_open, sem_post, sem_close, sem_unlink */
#include <stdio.h>           /* printf */

int main()
{
	int i = 0;
    sem_t *sem_ping = NULL;
    sem_t *sem_pong = NULL;

    sem_ping = sem_open("ping", O_CREAT, 0666, 1);

    if (sem_ping == SEM_FAILED)
    {
        return 1;
    }

    sem_pong = sem_open("pong", O_CREAT, 0666, 0);

    if (sem_pong == SEM_FAILED)
    {
        return 1;
    }

    for(i = 0; i < 10; ++i)
    {
        sem_wait(sem_ping);

        printf("Ping: %d\n",i);
        /* sleep(2); */

        sem_post(sem_pong);
    }

    sem_close(sem_ping);
    sem_close(sem_pong);

    sem_unlink("ping");
    sem_unlink("pong");

    return 0;
}