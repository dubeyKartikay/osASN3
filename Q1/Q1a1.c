
#include<stdio.h>
#include<pthread.h>
#define N 5
#define EAT 5000000

int forksOccupied[5];
int philId[5] = {0,1,2,3,4};


void * philosopher(void * idVoid){
    int  id = *(int *)idVoid;

   while(1){
    printf("philosopher no %d is now thinking\n",id);

    if (id%2==0)
    {
        while (forksOccupied[id] );
        forksOccupied[id] = 1;
        while (forksOccupied[(id+1)%N] );
        forksOccupied[(id+1)%N] = 1;
    }else{
        while (forksOccupied[(id+1)%N]);
        forksOccupied[(id+1)%N] = 1;
        while (forksOccupied[id]);
        forksOccupied[id] = 1;
    }

    printf("Philosopher %d has acquired forks and is now EATING \n",id);

    for (size_t i = 0; i < EAT; i++)
    {
        continue;
    }
    printf("Philosopher %d  is done EATING \n",id);

    if (id%2==0)
    {
        forksOccupied[id] = 0;
        forksOccupied[(id+1)%N] = 0;
    }else{
        forksOccupied[(id+1)%N] = 0;
        forksOccupied[id] = 0;
    }


   }

}


int main(){
    for (size_t i = 0; i < N; i++)
    {
        forksOccupied[i] = 0;
    }
    
    pthread_t threads[N];

    for (int i = 0; i < N; i++)
    {    
        pthread_create(&threads[i],NULL,philosopher,philId+i);
    }
    
    for (int i = 0; i < N; i++)
    {    
        pthread_join(threads[i],NULL);
    }


    





    return 0;
}