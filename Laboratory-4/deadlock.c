#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem1, sem2, sem3;

void *prog1(void *arg)
{
    sem_wait(&sem1); // we set sem1 to 0
    printf("Sem1 value = 0\n");
    sleep(2);                    // we create a context switch to another thread
    sem_wait(&sem2);             // we use sem_wait on sem2, BUT sem2 has been set to 0 in prog2, so prog1 is going to sleep
    printf("Sem2 value = -1\n"); // This part of the thread is never reached because of the deadlock
    sleep(2);
    sem_post(&sem2);
    sem_post(&sem1);
}

void *prog2(void *arg)
{
    sem_wait(&sem2); // we set sem2 to 0
    printf("Sem2 value = 0\n");
    sleep(2);                    // we create a context switch to another thread
    sem_wait(&sem3);             // we use sem_wait on sem3, BUT sem3 has been set to 0 in prog3, so prog2 is going to sleep
    printf("Sem3 value = -1\n"); // This part of the thread is never reached because of the deadlock
    sleep(2);
    sem_post(&sem3);
    sem_post(&sem2);
}

void *prog3(void *arg)
{
    sem_wait(&sem3); // we set sem3 to 0
    printf("Sem3 value = 0\n");
    sleep(2);                    // we create a context switch to another thread
    sem_wait(&sem1);             // we use sem_wait on sem1, BUT sem1 has been set to 0 in prog1, so prog3 is going to sleep
    printf("Sem1 value = -1\n"); // This part of the thread is never reached because of the deadlock
    sleep(2);
    sem_post(&sem1);
    sem_post(&sem3);
}

int main()
{
    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 1);
    sem_init(&sem3, 0, 1);

    pthread_t thread1, thread2, thread3;

    pthread_create(&thread1, NULL, &prog1, NULL);
    pthread_create(&thread2, NULL, &prog2, NULL);
    pthread_create(&thread3, NULL, &prog3, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
}