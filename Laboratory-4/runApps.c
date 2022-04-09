#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem1, sem2, sem3;

void *openFirefox(void *arg)
{
    sem_wait(&sem1); // sem1 has been initialized to 1 so this thread can run first
    printf("First to open : Firefox \n");
    system("firefox");
    sem_post(&sem2); // we set sem2 to 1 as it the second thread using sem2 will run
}

void *openEmacs(void *arg)
{
    sem_wait(&sem2); // sem2 has been set to 1 in openSafari, so this thread is no more asleep
    printf("Second to open : Emacs \n");
    system("emacs");
    sem_post(&sem3); // we set sem3 to 1 as it the third thread using sem3 will run
}

void *openVi(void *arg)
{
    sem_wait(&sem3); // sem2 has been set to 1 in openSpotify, so this thread is no more asleep
    printf("Third to open : VI");
    system("vi");
    // we do not need to use any other sem_post here
}

int main()
{
    sem_init(&sem1, 0, 1); // we set sem1 to 1 because we want this thread to be the first one to run
    sem_init(&sem2, 0, 0); // the others are set to 0 because we want them to run in second and third position
    sem_init(&sem3, 0, 0);

    pthread_t thread1, thread2, thread3;

    pthread_create(&thread1, NULL, &openFirefox, NULL);
    pthread_create(&thread2, NULL, &openEmacs, NULL);
    pthread_create(&thread3, NULL, &openVi, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
}