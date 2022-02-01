#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

int main(){

int source = open("tree.c", O_RDONLY);
if(source < 0){
    perror("Cannot open source file");
    return 1;
}
int target = creat("new.c", 0640);
if(target < 0){
    perror("Cannot open target file");
    return 2;
}
char buf[100];
int size;

while( (size = read(source, buf, 100)) > 0){

    int written = 0;
    while(size > written){
        int res = write(target, buf+written, size-written);

        if(res <= 0){
            perror("Cannot write to file");
            return -1;
        }
        written += res;
    }
}

printf("Copying is complete\n");

close(target);
close(source);

    return 0;
}