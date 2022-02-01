#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h> 
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h> 

int pid;
int sock;
void interrupt(int sig){
    close(sock);
    if(pid){
        kill(pid, sig);
        printf("Exiting...\n");
    }
    exit(0);
}

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Usage %s portnumber\n", argv[0]);
        return 1;
    }
    signal(SIGINT, &interrupt);
    signal(SIGTERM, &interrupt);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in addr = {
        AF_INET,
        htons(atoi(argv[1])),
        INADDR_ANY
    };

    int res = bind(sock, (struct sockaddr*)&addr, sizeof(addr));

    if(res < 0){
        perror("Bind");
        return -1;
    }

    pid = fork();

    if(pid){
        while(1){
            char buf[100];
            int chars = 0;
            char c;
            int portnumber;
            char filepath[255];

            printf("Enter a port number: ");
            scanf("%d", &portnumber);
            printf("Enter a file path: ");
            scanf("%s", filepath);

            struct sockaddr_in target = {
            AF_INET,
            htons(portnumber),
            inet_addr("127.0.0.1")
            };

            int fd = open(filepath, O_RDONLY);

            while((chars=read(fd, buf, 100)) > 0){
                    sendto(sock, buf, chars, 0, (struct sockaddr*)&target, sizeof(target));
                   chars = 0;
            }
            close(fd);
        }
        
    }
    else{
        struct sockaddr_in incoming;
        int lenincoming = sizeof(incoming);

        char buf[100];
        int chars = 0;

        int fd = creat("transferred", 0640);
        int start = 0;
        while( (chars = recvfrom(sock, buf, 100, 0, (struct sockaddr*)&incoming, &lenincoming)) >= 0 ){
            lseek(fd, start, SEEK_SET);
            write(fd, buf, chars);
            start +=chars;
        }
        close(fd);
    }
    
    return 0;
}