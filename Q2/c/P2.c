#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

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
    const char *name = "shared_memory";
    const int SIZE = 4096;

    int shm_fd;
    void *ptr;

    struct data dataArr[50];

    for (size_t i = 0; i < 50; i++)
    {
        dataArr[i].id = i;
        strcpy(dataArr[i].data, genString());
    }

    // open the shared memory object
    shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1)
    {
        printf("Error in opening shm : %s", strerror(errno));
        return 1;
    }

    // memory map the shared memory object
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("Error in nmap : %s", strerror(errno));
        return 2;
    }

    struct dataPacket buffer;
    int max_int = 0;
    while (1)
    {

        for (size_t i = max_int; i < max_int + 5; i++)
        {
            if (max_int > 49)
            {
                buffer.d[i - max_int].id = -1;
                strcpy(buffer.d[i - max_int].data, "\0");
                continue;
            }

            buffer.d[i - max_int] = dataArr[i];
        }

    
        // read from the shared memory region
    }
    printf("Message from P1: %s\n", (char *)ptr);

    return 0;
}
