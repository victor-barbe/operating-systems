#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/resource.h>

clock_t times(struct tms *buf);

int main()
{
    struct tms start, end;
    struct rusage rstart, rend;

    times(&start);
    getrusage(RUSAGE_SELF, &rstart);

    int a = 9, b = 5, c = 8, d = 3, e = 1, f = 4, g = 3, h = 2;

    // create shared space
    int id = shmget(45678, 2 * sizeof(int), IPC_CREAT | 0600);
    int *tab = shmat(id, NULL, 0);
    // (a+b) – [(c*d) / (e-f)] + (g+h)
    for (size_t i = 0; i < 10000; i++)
    {

        if (fork() == 0)
        {

            if (fork() == 0)
            {
                tab[0] = a + b;
                exit(0);
                if (fork() == 0)
                {
                    tab[1] = c * d;
                    exit(0);
                }
            }
            wait(NULL);
            tab[2] = e - f;
            exit(0);
        }
        tab[3] = g + h;
        wait(NULL);
    }
    printf("result : %d \n", (tab[0] - (tab[1] / tab[2]) + tab[3]));
    times(&end);
    getrusage(RUSAGE_SELF, &rend);

    printf("%lf usec\n\n", (end.tms_utime + end.tms_stime - start.tms_utime - start.tms_stime) * 1000000.0 / sysconf(_SC_CLK_TCK));
    printf("%ld Voluntary context switches\n", rend.ru_nvcsw);
    printf("%ld Involuntary context switches \n", rend.ru_nivcsw);
    printf("%ld Imputs\n", rend.ru_inblock);
    printf("%ld Outputs\n", rend.ru_oublock);
}
