#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5
#define EAT 5000000

sem_t forks[N];
sem_t sauceBowls[2];

void *philosopher(void *idVoid)
{
    int id = *(int *)idVoid;

    printf("Philosopher %d is now thinking\n", id);

    if (id % 2 == 0)
    {
        sem_wait(&forks[(id + 1) % N]);
        sem_wait(&forks[id]);
    }
    else
    {
        sem_wait(&forks[id]);
        sem_wait(&forks[(id + 1) % N]);
    }
    sem_wait(&sauceBowls[(id % 2)]);


    printf("Philosopher %d has acquired forks and the saucebowl %d and is now eating\n", id,(id % 2));

    for (size_t i = 0; i < EAT; i++)
    {
        continue;
    }
    printf("Philosopher %d is done eating\n", id);

    sem_post(&sauceBowls[(id % 2)]);
    sem_post(&forks[id]);
    sem_post(&forks[(id + 1) % N]);
}

int main()
{
    for (size_t i = 0; i < N; i++)
    {
        sem_init(&forks[i], 0, 1);
    }
    for (size_t i = 0; i < 2; i++)
    {
        sem_init(&sauceBowls[i], 0, 1);
    }

    pthread_t threads[N];

    for (int i = 0; i < N; i++)
    {
        pthread_create(&threads[i], NULL, philosopher, (void *)&i);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}