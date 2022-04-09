#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
        pid_t c1_pid, c2_pid; 
        (c1_pid = fork()) && (c2_pid = fork()); 
        

        if (c1_pid == 0)
        {
            printf("Im child 1 \n");
            printf("PPID : %d \n", getppid());
            printf("My own PID : %d \n \n", getpid());
        }
        else if (c2_pid == 0)
        {
            printf("Im child 2 \n");
            printf("PPID : %d \n", getppid());
            printf("My own ID : %d \n \n", getpid());
        }
        else{
            printf("Im your father \n");
            printf("Childs, my PID is : %d \n \n", getpid()); 
        }
        
}