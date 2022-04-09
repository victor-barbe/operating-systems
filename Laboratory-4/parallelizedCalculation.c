#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// (a+b)*(c-d)*(e+f) T1 runs (a+b) and stores the result in a shared table (1st available spot)
// T2 runs (c+d) and stores the result in a shared table (1st available spot)
// T3 runs (e+f) and stores the result in a shared table (1st available spot)
// T4 waits for two tasks to end and does the corresponding calculation
// T4 waits for the remaining task to end and does the final calculation then displays the result

sem_t sem1, sem2, sem3, sem4, sem5;
int a = 2, b = 3, c = 4, d = 5, e = 6, f = 7;
int results[4];

void *T1(void *arg)
{
    sem_wait(&sem1); // sem1 is set to 0, this thread can run
    results[0] = a + b;
    sem_post(&sem2); // sem2 is set to 1, next thread can run
}

void *T2(void *arg)
{
    sem_wait(&sem2); // sem2 has been set to 1 in T1 (was initialized to 0 in main)
    results[1] = c - d;
    sem_post(&sem4); // we set sem4 to 1, as it T4 will wake up
}

void *T3(void *arg)
{
    sem_wait(&sem3); // sem2 has been set to 1 in T4
    results[2] = e + f;
    sem_post(&sem5); // we set sem5 to 1 as it the last part of T4 will be reachable
}

void *T4(void *arg)
{
    sem_wait(&sem4); // T4 is wake up by T2, we can start the operations
    int firstCalc = results[0] * results[1]; // first operation after 2 tasks ended
    printf("(a + b) * (c - d) = %d \n", firstCalc);
    sem_post(&sem3); // We set sem3 to 1 as it T3 will wake up (nothing will happen in this thread until T3 has run to completion)
    sem_wait(&sem5); // sem5 has been set to 1 in T3, we can continue the operations in this thread
    results[3] = firstCalc * results[2]; // we make the final operation
    printf("Final result of (a+b)*(c-d)*(e+f) = %d", results[3]); // we print the result
}

int main()
{
    sem_init(&sem1, 0, 1); // we set sem1 to 1 because we want this thread to be the first one to run
    sem_init(&sem2, 0, 0); // the others are set to 0 because we want them to run in second, third, fourth and fifth position
    sem_init(&sem3, 0, 0);
    sem_init(&sem4, 0, 0);
    sem_init(&sem5, 0, 0);

    pthread_t thread1, thread2, thread3, thread4;

    pthread_create(&thread1, NULL, &T1, NULL);
    pthread_create(&thread2, NULL, &T2, NULL);
    pthread_create(&thread3, NULL, &T3, NULL);
    pthread_create(&thread4, NULL, &T4, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
}