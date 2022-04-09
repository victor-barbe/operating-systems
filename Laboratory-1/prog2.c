#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int i = 5; 
    if(fork() == 0){
        i = 12;
    }
    else{
        sleep(3); 
        printf("%d", i); 
    }
}

