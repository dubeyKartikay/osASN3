#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/un.h>

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
    struct data d[5];
};

int main(int argc, char *argv[])
{
    // printf("ss %d",sizeof(struct dataPacket));
    struct data dataArr[50];

    for (size_t i = 0; i < 50; i++)
    {
        dataArr[i].id = i;
        strcpy(dataArr[i].data, genString());
    }

    struct sockaddr_un *socketAddress = (struct sockaddr_un *)calloc(1, sizeof(struct sockaddr_un));

    int socketfile = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socketfile < 0)
    {
        printf("error in binding socket : %s", strerror(errno));
        return 1;
    }

    socketAddress->sun_family = AF_UNIX;
    strcpy(socketAddress->sun_path, "./socket");

    if (connect(socketfile, (struct sockaddr *)socketAddress, sizeof(*socketAddress)) < 0)
    {
        printf("error in connecting to socket : %s", strerror(errno));

        return 2;
    }

    struct dataPacket *buffer = (struct dataPacket *)calloc(1, sizeof(struct dataPacket));

    int max_int = 0;
    int iter = 0;
    while (1)
    {
        // printf("Iter %d",iter);
        printf("%d\n", max_int);
        if(max_int < 0){
            break;
        }
        for (size_t i = max_int; i < max_int + 5; i++)
        {
            if (max_int>49)
            {
                buffer->d[i-max_int].id = -1;
                strcpy(buffer->d[i-max_int].data,"\0");
                continue;
            }
            
            buffer->d[i-max_int] = dataArr[i];
        }

        // for (size_t i = 0; i < 5; i++)
        // {
        //     printf("%d %s\n", buffer->d[i].id, buffer->d[i].data);
        // }
        int res = send(socketfile, (void *)buffer, sizeof(struct dataPacket), 0);
        // printf("%d\n", res);
        if (res < 0)
        {
            printf("error in sending data to socket : %s", strerror(errno));
            return 3;
        }

        res = recv(socketfile, &max_int, sizeof(int), 0);
        iter++;
        max_int++;
    }
    close(socketfile);
    return 0;
}
