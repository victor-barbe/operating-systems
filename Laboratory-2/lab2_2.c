#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  // P1 : a+b, save in shared space then exit
  // P2 : c+d, wait for P1, do the multiply operation, save result in shared space then exit
  // P3 : e+f, wait for P2, do ther final operation

  // wait version : P3 should be parent of P2, and P2 parent of P1

  int a = 9, b = 5, c = 8, d = 3, e = 1, f = 4, g = 3, h = 2;

  // create shared space
  int id = shmget(45678, 2 * sizeof(int), IPC_CREAT | 0600);
  int *tab = shmat(id, NULL, 0);

  // (a+b) – [(c*d) / (e-f)] + (g+h)

  if (fork() == 0)
  {

    if (fork() == 0)
    {
      tab[0] = a + b;
      exit(0);
      // wait NULL();
      if (fork() == 0)
      {
        tab[1] = c * d;
        exit(0);
      }
    }
    wait(NULL); // wait for P1 to end
    tab[2] = e - f;
    exit(0);
  }
  // this is P3
  tab[3] = g + h;
  wait(NULL); // wait for P2 to friend
  printf("result : %d \n", (tab[0] - (tab[1] / tab[2]) + tab[3]));
}

// tab[0] - (tab[1] / tab[2]) + tab[3]
// %d   %d    %d     %d\n", tab[0], tab[1], tab[2], tab[3]
// (a+b) – [(c*d) / (e-f)] + (g+h)
