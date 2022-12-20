#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define SHMNAME "shm"
#define SEMOPHORE_NAME "thhirteen"
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
            memcpy(data, &dp, sizeof(struct dataPacket));
        }

        

        sem_post(sem);
    }

    sem_close(sem);
    if (shmdt(data) < 0)
    {
        perror("Error detaching shared memory segment");
        return 1;
    }

    return 0;
}
