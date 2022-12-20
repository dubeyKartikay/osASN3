#include <semaphore.h> /*For semaphore functions */
#include <unistd.h>    /*For sleep */
#include <pthread.h>   /* For pthread related utilities */
#include <errno.h>     /* For error code */
#include <stdio.h>     /* For printf and perror */
#include <stdlib.h>    /* For exit() */
#include <math.h>
#define MAX_FORKS 5
#define MAX_PHILOSOPHERS 5

sem_t forks[MAX_FORKS];
int philosophers[MAX_PHILOSOPHERS];
int arr[MAX_PHILOSOPHERS];
void *philosopher(void *arg)
{
    int id = *(int *)arg;
    id--;
    int leftFork = id%5;
    int rightFork = (id + 1)%5;
    while (1)
    {
        if (id % 2 == 1)
        {
            sem_wait(&forks[rightFork]);
            sem_wait(&forks[leftFork]);
        }
        else
        {
            sem_wait(&forks[leftFork]);
            sem_wait(&forks[rightFork]);
        }
        philosophers[id] = 1;
        arr[id] = 1;
        printf("%d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);

        printf("Philosopher %d is eating with fork %d %d\n", id, leftFork, rightFork);
        for (int i = 0; i < pow(2, 20); i++)
        {
            continue;
        }
        sem_post(&forks[rightFork]);
        sem_post(&forks[leftFork]);
    }
}

int main()
{
    for (int i = 0; i < MAX_FORKS; i++)
    {
        sem_init(&forks[i], 0, 1);
    }
    pthread_t threads[MAX_PHILOSOPHERS];
    for (int i = 0; i < MAX_PHILOSOPHERS; i++)
    {
        printf("Creating thread %d\n", i);
        pthread_create(&threads[i], NULL, philosopher, (void *)&i);
    }

    for (int i = 0; i < MAX_PHILOSOPHERS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}