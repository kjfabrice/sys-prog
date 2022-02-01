/**
 * This program copies any file(hidden files also)
 * from any directory on the system to any directory
 * source path and destination path must be provided 
 * to the program
*/

#include<stdlib.h> 
#include<stdio.h> 
#include<unistd.h> 
#include<fcntl.h>  
#include<string.h> 

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Usage: ./copy \"/source/path\""
        " \"/destination/path\"\n");
        return 2;
    }
    int fd[2];
    pipe(fd);
    if(!fork()){
        close(fd[0]);
        dup2(fd[1], 1);
        execlp("find", "find", argv[1], "-maxdepth", "1", "-type", "f", NULL);
        close(fd[1]);
       
        return 0;
    }
    else{
        close(fd[1]);
        int file = creat("temp.txt", 0640);
        char data;
        while(read(fd[0], &data, 1) > 0){
            write(file, &data, 1);
        }

        close(fd[0]);
        close(file);

        FILE *fp = fopen("temp.txt", "r");
        if(fp == NULL){
            perror("Cannot open the temp file");
            return 1;
        }
        char ch = getc(fp); char buf[1024]; 
        while(ch != EOF){
            int i = 0;
            while( ch != '\n' ){
                buf[i] = ch;
                i+=1;
                ch = getc(fp);
                if(ch == '\n'){
                    buf[i] = 0;
                    break;
                }
            }

            int source = open(buf, O_RDONLY);
            if(source < 0){
                perror("Cannot open the source file");
                return -1;
            }

            int len = strlen(argv[1]);
            char c[50], destination[150] ; i=0;
            strcpy(destination, argv[2]);
            while(buf[len] != '\0'){
                c[i] = buf[len];
                len+=1; i+=1;
            }
            c[i] = 0;
            strcat(destination, c);
        
            int target = creat(destination, 0640);
            if(target < 0){
                perror("Cannot open the target file");
                return -2;
            }

            char buffer[100];
            int size;
            while( (size = read (source, buffer, 100) ) > 0){
                int written = 0;
                while (size > written){
                    int res = write(target, buffer+written, size-written);
                
                    if(res <= 0){
                        perror("Cannot write to the target file");
                        return 3;
                    }
                
                    written += res;
                }
            }

            close(target);
            (source);

            ch = getc(fp);
        }
    
        fclose(fp);
        unlink("temp.txt");

    }
    printf("Copying complete\n");
    return 0;
}