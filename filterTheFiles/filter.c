#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>

int main(int argc, char *argv[]){

    int fd1[2];
    pipe(fd1);

    char letter;
    printf("Enter a character: ");
    scanf("%c", &letter);

    if(!fork()){
        //first child sending file names
        close (fd1[0]);
        dup2(fd1[1], 1);
        execlp("ls", "ls", argv[1], NULL);
        close(fd1[1]);
        return 0;
    }
    else if(!fork()){
        //second child filtering the names
        close(fd1[1]);
        dup2(fd1[0], 0);
        char data[128]; 
        while(!feof(stdin)){
            fscanf(stdin, "%s", data);
            if(data[0] == letter){
                printf("%s\n", data);
            }
            else{
                continue;
            }
        }
        close(fd1[0]);
        
        return 0;
    }

    close(fd1[1]);
    close(fd1[0]);
    wait(NULL);
    wait(NULL);
}