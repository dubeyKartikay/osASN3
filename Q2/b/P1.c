#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/stat.h>
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
    // Create the FIFOs
    if (mkfifo("./fifo1", 0666) < 0 || mkfifo("./fifo2", 0666) < 0)
    {
        printf("error in creating FIFOs : %s", strerror(errno));
        return 1;
    }

    // Open the FIFOs for reading and writing
    int fifo1 = open("./fifo1", O_RDONLY);
    int fifo2 = open("./fifo2", O_WRONLY);
    if (fifo1 < 0 || fifo2 < 0)
    {
        printf("error in opening FIFOs : %s", strerror(errno));
        return 2;
    }
    struct dataPacket *buffer = (struct dataPacket *)calloc(1, sizeof(struct dataPacket));
    // Continuously read from FIFO 1 and write to FIFO 2
    int max_id = 0;
    while (1)
    {
        int message;
        int res = read(fifo1, buffer, sizeof(struct dataPacket));
        buffer = (struct dataPacket *)buffer;
        if (res < 0)
        {
            printf("error in reading from FIFO 1 : %s", strerror(errno));
            return 3;
        }
        for (size_t i = 0; i < 5; i++)
        {
            if (buffer->d[i].id == -1)
            {
                max_id = -1;
                break;
            }

            printf("%d %s\n", buffer->d[i].id, buffer->d[i].data);
            if (buffer->d[i].id > max_id)
            {
                max_id = buffer->d[i].id;
            }
        }

        res = write(fifo2, (void *)&max_id, sizeof(int));
        if (res < 0)
        {
            printf("error in writing to FIFO 2 : %s", strerror(errno));
            return 4;
        }

        if (max_id == -1)
        {
            break;
        }
    }

    close(fifo1);
    close(fifo2);
    return 0;
}
