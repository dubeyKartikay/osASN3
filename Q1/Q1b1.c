#include <stdio.h>
#include <pthread.h>

#define N 5
#define EAT 5000000

int forks[N];
int sauceBowls[2];

int pilid[5] = {0, 1, 2, 3, 4};
void *philosopher(void *idVoid)
{

    while (1)
    {

        int id = *(int *)idVoid;

        printf("Philosopher %d is now thinking\n", id);

        if (id % 2 == 0)
        {
            while (forks[(id + 1) % N] == 1);
            forks[(id + 1) % N] == 1;
            while (forks[id] == 1);
            forks[id] = 1;
        }
        else
        {
            while (forks[id] == 1);
            forks[id] = 1;
            while (forks[(id + 1) % N] == 1);
            forks[(id + 1) % N] == 1;
        }
        while(sauceBowls[(id % 2)]==1);
        sauceBowls[(id % 2)] =1;
        printf("Philosopher %d has acquired forks and the saucebowl %d and is now eating\n", id, (id % 2));
        for (size_t i = 0; i < EAT; i++)
        {
            continue;
        }
        printf("Philosopher %d is done eating\n", id);

        sauceBowls[(id % 2)] = 0;
        forks[id] = 0;
        forks[(id + 1) % N] = 0;
    }
}

int main()
{
    for (size_t i = 0; i < N; i++)
    {
        forks[i] = 0;
    }
    for (size_t i = 0; i < 2; i++)
    {
        sauceBowls[i] = 0;
    }

    pthread_t threads[N];

    for (int i = 0; i < N; i++)
    {
        pthread_create(&threads[i], NULL, philosopher, pilid + i);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}