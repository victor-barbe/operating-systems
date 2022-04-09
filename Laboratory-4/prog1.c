#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int i = 65;

void *increment(void *arg)
{
    int reg = i;
    sleep(1);
    reg++;
    // sleep(1); // creates a context switch with other threads
    i = reg;
    return (NULL);
}

void *decrement(void *arg)
{
    int reg = i;
    sleep(1);
    reg--;
    // sleep(1); // creates a context switch with other threads
    i = reg;
    return (NULL);
}

int main()
{
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, increment, NULL);
    pthread_create(&thread2, NULL, decrement, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("i = %d\n", i);
}