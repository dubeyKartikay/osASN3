#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define SHMNAME "shm"
#define SEMOPHORE_NAME "thhirteen"
#define STRINGLEN 5

char *genString()
{

    char *string = (char *)calloc(6, sizeof(char));
    for (size_t i = 0; i < STRINGLEN; i++)
    {
        string[i] = 97 + rand() % 26;
    }
    string[5] = '\0';
    return string;
}
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
    struct data dataArr[50];

    for (size_t i = 0; i < 50; i++)
    {
        dataArr[i].id = i;
        strcpy(dataArr[i].data, genString());
    }

    // Create a shared memory segment
    key_t key = ftok(SHMNAME, 123);
    sem_t *semo = sem_open(SEMOPHORE_NAME, O_CREAT, 0660, 1);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    if (shmid < 0)
    {
        perror("Error creating shared memory segment");
        return 1;
    }

    // Attach the shared memory segment to the process's address space
    char *data = shmat(shmid, NULL, 0);
    if (data == (char *)-1)
    {
        perror("Error attaching shared memory segment");
        return 1;
    }
    int max_int = 0;
    struct dataPacket dp;
    dp.flag = 1;
    dp.max_int = -1;
    memcpy(data, &dp, sizeof(struct dataPacket));
    while (1)
    {
        // sleep(0.5);
        // printf("here %d\n", max_int);
        fflush(stdout);

        sem_wait(semo);
        memcpy(&dp, data, sizeof(struct dataPacket));
        // // while (*data != max_int);
        if (dp.flag != 0)
        {
            max_int = dp.max_int;
            max_int++;
            if (max_int >= 49)
            {
                sem_post(semo);
                dp.flag = -1;
                memcpy(data, &dp, sizeof(struct dataPacket));
                break;
            }

            // printf("data : %d ", data);
            fflush(stdout);
            for (int i = max_int; i < max_int + 5; i++)
            {
                dp.d[i - max_int] = dataArr[i];
            }
            dp.flag = 0;
            memcpy(data, &dp, sizeof(struct dataPacket));
        }

        sem_post(semo);
    }

    // Detach the shared memory segment from the process's address space
    sem_close(semo);
    if (shmdt(data) < 0)
    {
        perror("Error detaching shared memory segment");
        return 1;
    }
    sem_unlink(SEMOPHORE_NAME);
    return 0;
}
