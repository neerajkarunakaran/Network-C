#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define MAXUSERS 50
int check_user(struct sockaddr_in *user);
int users[MAXUSERS];
int cnt = 0;
int main()
{
    int udpSocket, nBytes;
    char buffer[1024];
    struct sockaddr_in serverAddr, clientAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size, client_addr_size;
    int i;
    udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    int yes = 1;
    setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, &yes, sizeof (int));
    bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    addr_size = sizeof serverStorage;
    while (1) {
	nBytes =
	    recvfrom(udpSocket, buffer, 1024, 0, (struct sockaddr*)&serverStorage, &addr_size);
        printf("%s\n", buffer);
        struct sockaddr_in *p = (struct sockaddr_in*)&serverStorage;
        if(check_user(p)){
            int i;
            int sender = p->sin_port;
            for(i = 0; i < cnt; i++){
                
                if(users[i] != sender){
                    p->sin_port = htons(users[i]); 
                    sendto(udpSocket, buffer, nBytes, 0, (struct sockaddr*)&serverStorage, addr_size);
                }
            }
        } else{
  
            users[cnt++] = p->sin_port;
            int sender = p->sin_port;
            int i ;
            for(i = 0; i < cnt; i++) {
                if(users[i] != sender){
                    p->sin_port = users[i];
                    sendto(udpSocket, buffer, nBytes, 0, (struct sockaddr*)&serverStorage, addr_size);
                }
            }
        }
    }
                     
            
		     
    return 0;
}
int check_user(struct sockaddr_in *user){
    int i;
    for(i = 0; i < cnt; i++){
        if(users[i] == user->sin_port){
            return 1;
        }
    }
   
    return 0;
}      
