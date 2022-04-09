#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main (){

printf("Process ID : "); //showing the actual process ID
printf("%d \n",getpid());

char *binaryPath = "/Users/pierre-louisgaucher/Desktop/LAB1OS/execOuvrir";
char *args[] = {binaryPath, NULL};
execv(binaryPath, args);

printf("%d", i); 

}