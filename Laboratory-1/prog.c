#include <stdio.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int a = fork();  // creates a new child process
  if(a == 0){
    printf("Im the child \n %d \n %d \n", getpid(), getppid());
  }
  else{
    sleep(3);
    printf("Im the parent \n %d \n", getpid());
  }
}

