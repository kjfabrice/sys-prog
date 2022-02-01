#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h> 
#include <sys/un.h>

/**
 * 
 * This program finds the greatest
 * commom divisor of 2 integer numbers
 * received from the client
 * 
 */
int sock;
int gcd(int num1, int num2){
    if( num2==0)
        return num1;
    else
        return gcd(num2,num1%num2);
}

void interrupt(int signal){
    close(sock);
    unlink("unixsocket.tmp");
    printf("\nEXITING...\n");
    exit(0);
}

int main(){

    sock = socket(AF_UNIX, SOCK_STREAM, 0);

    signal(SIGINT, &interrupt);
    signal(SIGTERM, &interrupt);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "unixsocket.tmp");

    if ( bind(sock, (struct sockaddr*)&addr, sizeof(addr) ) < 0){
        perror("Bind");
        return -1;
    };

    listen(sock, 5);

    int newSocket = accept(sock, NULL, NULL);

    close(sock);

    int num[2];
    
    recv(newSocket, num, sizeof(num), 0);
    
    int result = gcd(num[0],num[1]);

    send(newSocket, &result, sizeof(result), 0);

    close(newSocket);

    unlink("unixsocket.tmp");
    
    return 0;
}