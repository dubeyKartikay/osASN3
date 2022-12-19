#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<errno.h>

struct data
{
    int id;
    char data[6];
};
struct dataPacket
{
    struct data d[5];
};

int main() {
    const char *name = "shared_memory";
    const int SIZE = 4096;

    int shm_fd;
    void *ptr;

    // create the shared memory object
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    if (shm_fd == -1) {
        printf("Error in opening shm : %s",strerror(errno));
        return 1;
    }

    // configure the size of the shared memory object
    ftruncate(shm_fd, SIZE);

    // memory map the shared memory object
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Error in nmap : %s",strerror(errno));
        return 2;
    }

    int * a;
    *a = -1;
    memcpy(ptr,a,sizeof(int));   
    while(1){
        while(ptr==NUL)
    }
    


    // write to the shared memory region
    sprintf(ptr, "Hello from P1!");
    ptr += strlen(ptr) + 1;

    return 0;
}