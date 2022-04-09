#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int i = 5; 
    if(fork() == 0){
        char *binaryPath = "/Users/pierre-louisgaucher/Desktop/LAB1OS/execOuvrir";
        char *args[] = {binaryPath, NULL};
        execv(binaryPath, args);

        i++;
        printf("%d \n", i);
    }
    else{
        printf("PID : %d", getpid());
    }
}