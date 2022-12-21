#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>
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
    int socketfile = socket(AF_UNIX, SOCK_STREAM, 0);
    int clientFile;
    if (socketfile < 0)
    {
        printf("error in creating socket : %s", strerror(errno));
        return 1;
    }
    struct sockaddr_un *socketAddress = (struct sockaddr_un *)calloc(1, sizeof(struct sockaddr_un));
    socketAddress->sun_family = AF_UNIX;
    strcpy(socketAddress->sun_path, "./socket");
    int res = bind(socketfile, (struct sockaddr *)socketAddress, sizeof(struct sockaddr));
    if (res < 0)
    {
        printf("error in binding socket : %s", strerror(errno));
        return 2;
    }
    int max_id = 0;
    res = listen(socketfile, 100);
    if (res < 0)
    {
        printf("error in listening : %s", strerror(errno));
        return 3;
    }
    struct sockaddr_un *ClientsocketAddress = (struct sockaddr_un *)calloc(1, sizeof(struct sockaddr_un));
    socklen_t clientSocketLen = sizeof(*ClientsocketAddress);
    clientFile = accept(socketfile, (struct sockaddr *)ClientsocketAddress, &clientSocketLen);

    if (clientFile < 0)
    {
        printf("error in accepting request from client : %s", strerror(errno));
        return 4;
    }
    struct dataPacket *buffer = (struct dataPacket *)calloc(1, sizeof(struct dataPacket));
    struct timespec start, finish,diff;
    clock_gettime( CLOCK_REALTIME, &start );
    while (1)
    {
        res = recv(clientFile, buffer, sizeof(struct dataPacket), 0);
 
        if (res < 0)
        {
            printf("error in recieving request from client : %s", strerror(errno));
            return 5;
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

        // fflush(clientFile);
        res = send(clientFile, (void *)&max_id, sizeof(int), 0);
        
        if (res < 0)
        {
            printf("ERRROR\n");
            printf("%s", strerror(errno));
            fflush(stdout);
        }

        if (max_id==-1)
        {
            break;
        }
        
        // printf("Received message: %s\n", buffer);
    }
    clock_gettime( CLOCK_REALTIME, &finish );
    diff.tv_sec = finish.tv_sec - start.tv_sec;
    diff.tv_nsec = finish.tv_nsec - start.tv_nsec;
    if (diff.tv_nsec < 0)
    {
        diff.tv_nsec+=1000000000;
        diff.tv_sec--;
    }
    
    printf("Time taken %d,%ld",diff.tv_sec,diff.tv_nsec);
    close(clientFile);
    close(socketfile);
    return 0;
}
