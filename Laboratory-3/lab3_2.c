#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/resource.h>

clock_t times(struct tms *buf);

void *f1(void *arg)
{
    int *a = malloc(sizeof(int));
    *a = 5;
    int *b = malloc(sizeof(int));
    *b = 3;
    *a = *a + *b;
    pthread_exit(a);
}

void *f2(void *arg)
{
    int *c = malloc(sizeof(int));
    *c = 1;
    int *d = malloc(sizeof(int));
    *d = 4;
    *c = *c * *d;
    pthread_exit(c);
}

void *f3(void *arg)
{
    int *e = malloc(sizeof(int));
    *e = 6;
    int *f = malloc(sizeof(int));
    *f = 2;
    *e = *e - *f;
    pthread_exit(e);
}

void *f4(void *arg)
{
    int *g = malloc(sizeof(int));
    *g = 8;
    int *h = malloc(sizeof(int));
    *h = 7;
    *g = *g + *h;
    pthread_exit(g);
}

int main()
{
    struct tms start, end;
    struct rusage rstart, rend;

    times(&start);
    getrusage(RUSAGE_SELF, &rstart);

    pthread_t thread1, thread2, thread3, thread4;

    int *ptr1, *ptr2, *ptr3, *ptr4;
    for (int i = 0; i < 10000; i++)
    {
        pthread_create(&thread1, NULL, f1, NULL);
        pthread_join(thread1, (void **)&ptr1);

        pthread_create(&thread2, NULL, f2, NULL);
        pthread_join(thread2, (void **)&ptr2);

        pthread_create(&thread3, NULL, f3, NULL);
        pthread_join(thread3, (void **)&ptr3);
 
        pthread_create(&thread4, NULL, f4, NULL);
        pthread_join(thread4, (void **)&ptr4);
    }
    int finalResult = *ptr1 - (*ptr2 / *ptr3) + *ptr4;

    printf("ptr = %p val = %d\n", ptr1, *ptr1);
    printf("ptr = %p val = %d\n", ptr2, *ptr2);
    printf("ptr = %p val = %d\n", ptr3, *ptr3);
    printf("ptr = %p val = %d\n", ptr4, *ptr4);
    printf("final result = %d\n\n", finalResult);
    times(&end);
    getrusage(RUSAGE_SELF, &rend);

    printf("%lf usec\n", (end.tms_utime + end.tms_stime - start.tms_utime - start.tms_stime) * 1000000.0 / sysconf(_SC_CLK_TCK));

    printf("%ld Voluntary context switches\n", rend.ru_nvcsw);
    printf("%ld Involuntary context switches \n", rend.ru_nivcsw);
    printf("%ld Imputs\n", rend.ru_inblock);
    printf("%ld Outputs\n", rend.ru_oublock);
}