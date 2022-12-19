#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include<string.h>
#include<sys/stat.h>


#define STRINGLEN 5

char *genString()
{

    char *string = (char *)calloc(6, sizeof(char));
    for (size_t i = 0; i < STRINGLEN; i++)
    {
        string[i] = 97 + rand() % 26;
    }
    string[STRINGLEN] = '\0';
    return string;
}
struct data
{
    int id;
    char data[6];
};
struct dataPacket
{
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
    // Open the FIFOs for reading and writing
    int fifo1 = open("./fifo1", O_WRONLY);
    int fifo2 = open("./fifo2", O_RDONLY);
    if (fifo1 < 0 || fifo2 < 0)
    {
        printf("error in opening FIFOs : %s", strerror(errno));
        return 1;
    }

    int message = 1;

    // Continuously write to FIFO 1 and read from FIFO 2
    struct dataPacket *buffer = (struct dataPacket *)calloc(1, sizeof(struct dataPacket));
    int max_int = 0;
    while (1)
    {
        printf("%d\n", max_int);

        if (max_int < 0)
        {
            break;
        }

        for (size_t i = max_int; i < max_int + 5; i++)
        {
            if (max_int > 49)
            {
                buffer->d[i - max_int].id = -1;
                strcpy(buffer->d[i - max_int].data, "\0");
                continue;
            }

            buffer->d[i - max_int] = dataArr[i];
        }

        int res = write(fifo1, (void *)buffer, sizeof(struct dataPacket));
        if (res < 0)
        {
            printf("error in writing to FIFO 1 : %s", strerror(errno));
            return 2;
        }

        res = read(fifo2, &max_int, sizeof(int));
        if (res < 0)
        {
            printf("error in reading from FIFO 2 : %s", strerror(errno));
            return 3;
        }

        // printf("Received message: %d\n", message);
        if (max_int < 0)
        {
            break;
        }

        max_int++;

        // message++;
    }

    close(fifo1);
    close(fifo2);
    return 0;
}
