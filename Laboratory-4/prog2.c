#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

sem_t semaphore;
int i;

void *increment(void *arg)
{
    sem_wait(&semaphore); // semaphore takes the value 0 : the other thread is asleep
    printf("Increment beginning value i = %d \n", i);
    int reg; // we modify i value using reg method
    reg = i;
    reg++;
    i = reg;
    printf("Increment ending value i = %d \n", i);
    sem_post(&semaphore); // semaphore takes the value 1  : the other thread can be called
}

void *decrement(void *arg)
{
    sem_wait(&semaphore); // semaphore takes the value 0 : the other thread is asleep
    printf("Decrement beginning value i = %d \n", i);
    int reg; // we modify i value using reg method
    reg = i;
    reg--;
    i = reg;
    printf("Decrement ending value i = %d \n", i);
    sem_post(&semaphore); // semaphore takes the value 1  : the other thread can be called
}

int main()
{
    i = 65;
    sem_init(&semaphore, 0, 1); // we initialize the semaphore to 1

    pthread_t thread1, thread2;
    for (int j = 0; j < 20; j++)
    {
        pthread_create(&thread1, NULL, &increment, NULL); // we create 2 threads
        pthread_create(&thread2, NULL, &decrement, NULL);

        pthread_join(thread1, NULL); // we wait for our threads to finish their execution
        pthread_join(thread2, NULL);

        printf("Value of i : %d \n", i); // we print i value 
    }
    return 0;
}