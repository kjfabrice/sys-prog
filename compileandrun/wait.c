#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>

int main(){

    int pid = fork();
    int status;
    if(pid < 0){
        perror("Fork failed");
        return -1;
    }

    if(pid == 0){
        printf("I'm the child with pid %d and ppid %d\n", getpid(), getppid());
        return 80;
    }

    printf("I'm waiting for my child...\n");
    wait(&status);
    printf("The child return code is %d\n", WEXITSTATUS(status));
    printf("I'm the parent with pid %d and ppid %d\n", getpid(), getppid());
   
    return 0;
}
