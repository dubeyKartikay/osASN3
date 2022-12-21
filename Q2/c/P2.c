#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>
#define SHMNAME "shm"
#define SEMOPHORE_NAME "fourteen"
struct data
{
    int id;
    char data[6];
};
struct dataPacket
{
    int flag;
    int max_int;
    struct data d[5];
};

int main()
{
    // Get the shared memory segment created by P1
    // sleep(0.5);
    key_t key = ftok(SHMNAME, 123);
    int shmid = shmget(key, 1024, 0666);
    if (shmid < 0)
    {
        perror("Error getting shared memory segment");
        return 1;
    }

    char *data = shmat(shmid, NULL, 0);
    if (data == (char *)-1)
    {
        perror("Error attaching shared memory segment");
        return 1;
    }
    sem_t *sem = sem_open(SEMOPHORE_NAME, 0);
    int max_int = 0;
    struct timespec start, finish,diff;
    clock_gettime( CLOCK_REALTIME, &start );
    while (1)
    {
        // sleep(1);
        struct dataPacket dp;
        dp.flag = 1;
        sem_wait(sem);
        // printf("dcd %d\n",*data);

        memcpy(&dp, data, sizeof(struct dataPacket));
        
        if (dp.flag == -1)
        {
        sem_post(sem);
        break;
            
        }
        if (&dp != NULL && dp.flag != 1)
        {
            for (int i = 0; i < 5; i++)
            {
                printf("id : (%d)  (%s) \n", dp.d[i].id, dp.d[i].data);
                fflush(stdout);

                if (dp.d[i].id > max_int)
                {
                    max_int = dp.d[i].id;
                }
            }
            dp.max_int = max_int;
            dp.flag = 1;
            printf("P2 : max_int %d\n",max_int);
            memcpy(data, &dp, sizeof(struct dataPacket));
        }

        

        sem_post(sem);
    }

    sem_close(sem);

    clock_gettime( CLOCK_REALTIME, &finish );
    diff.tv_sec = finish.tv_sec - start.tv_sec;
    diff.tv_nsec = finish.tv_nsec - start.tv_nsec;
    if (diff.tv_nsec < 0)
    {
        diff.tv_nsec+=1000000000;
        diff.tv_sec--;
    }
    
    printf("Time taken %d,%ld",diff.tv_sec,diff.tv_nsec);
    if (shmdt(data) < 0)
    {
        perror("Error detaching shared memory segment");
        return 1;
    }

    return 0;
}
