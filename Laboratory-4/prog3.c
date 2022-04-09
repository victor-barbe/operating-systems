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
    sem_wait(&semaphore);
    int reg;
    reg = i;
    reg++;
    i = reg;
    printf("Increment ending value i = %d \n", i);
    sem_post(&semaphore);
}

void *decrement(void *arg)
{
    sem_wait(&semaphore);
    int reg;
    reg = i;
    reg--;
    i = reg;
    printf("Decrement ending value i = %d \n", i);
    sem_post(&semaphore);
}

void *add5(void *arg)
{
    sem_wait(&semaphore);
    int reg;
    reg = i;
    reg += 5;
    i = reg;
    printf("Add5 ending value i = %d \n", i);
    sem_post(&semaphore);
}

int main()
{
    i = 65;
    sem_init(&semaphore, 0, 1);

    pthread_t thread1, thread2, thread3;
    
        pthread_create(&thread1, NULL, increment, NULL);
        pthread_create(&thread2, NULL, decrement, NULL);
        pthread_create(&thread3, NULL, add5, NULL);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);

        printf("Value of i : %d \n\n", i);
    
    return 0;
}