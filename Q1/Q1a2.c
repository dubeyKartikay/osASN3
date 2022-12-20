#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define N 5
#define EAT 5000000

sem_t forks[N];
int pilId[N]=  {0,1,2,3,4};
void *philosopher(void *idVoid)
{
    int id = *(int *)idVoid;
    // id--;
    // printf("id: %d",id);
    // sleep(1);
    printf("%d id\n",id);
    while (1)
    {
        printf("Philosopher %d is now thinking\n", id);

        if (id == 4)
        {
            sem_wait(&forks[(id + 1) % N]);
            sem_wait(&forks[id]);
        }
        else
        {
            sem_wait(&forks[id]);
            sem_wait(&forks[(id + 1) % N]);
        }

        printf("Philosopher %d has acquired forks and is now eating\n", id);

        for (size_t i = 0; i < EAT; i++)
        {
            continue;
        }

        // sleep(1);

        printf("Philosopher %d is done eating\n", id);

        if (id == 4)
        {
            sem_post(&forks[id]);
            sem_post(&forks[(id + 1) % N]);
        }
        else
        {
            sem_post(&forks[(id + 1) % N]);
            sem_post(&forks[id]);
        }
    }
}

int main()
{
    for (size_t i = 0; i < N; i++)
    {
        sem_init(&forks[i], 0, 1);
    }

    pthread_t threads[N];

    for (int i = 0; i < N; i++)
    {
        // printf("i : %d\n",i);
        // int arg  = i;
        int res =  pthread_create(&threads[i], NULL, philosopher, pilId+i);
        printf("%d\n",res);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}