#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h> 
#include <sys/un.h>

int main(){

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "unixsocket.tmp");

    if ( connect(sock, (struct sockaddr*)&addr, sizeof(addr) ) < 0){
        perror("Connect");
        return -1;
    }

    printf("Enter two numbers:\n");
    int num[2];
    scanf("%d", &num[0]);
    scanf("%d", &num[1]);

    send(sock, num, sizeof(num), 0);

    int result;
    
    recv(sock, &result, sizeof(result), 0);
       
    printf("The GCD of %d and %d is %d\n",num[0], num[1], result);
    
    close(sock);
    
    return 0;
}