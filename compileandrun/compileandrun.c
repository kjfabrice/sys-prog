#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<string.h>

int main(int argc, char *argv[]){

    if(argc <  2){
        printf("Usage: %s yourprogram", argv[0]);
        return 1;
    }

    int pid = fork();

    if(pid <  0){
        perror("Cannot fork");
        return -1;
    }

    if(pid == 0){
        //child process
        char cfile[255];
        strcpy(cfile, argv[1]);
        strcat(cfile, ".c");
        execlp("gcc", "gcc", "-o", argv[1], cfile, NULL);

        perror("Cannot compile your program");
        return -2;
    }

    int s;
    wait(&s);
    if(WIFEXITED(s)){
        if(WEXITSTATUS(s) !=0){
            printf("Compilation failed, check your program\n");
            return 2;
        }
    }
    else{
        printf("Child has crashed\n");
        return 3;
    }

    pid = fork();

    if(pid <  0){
        perror("Cannot fork");
        return -1;
    }

    //executing the program
    if(pid == 0){
        //child process
        execl(argv[1], argv[1], NULL);

        perror("Cannot execute your program");
        return -2;
    }

    wait(&s);
    if(WIFEXITED(s)){
        if(WEXITSTATUS(s) !=0){
            printf("Your program failed with return code %d\n", WEXITSTATUS(s));
            return 2;
        }
    }
    else{
        printf("Your program has crashed\n");
        return 3;
    }

    return 0;
}