#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>
#define MAXUSERS 50
typedef struct u{
    char name[20];
    int port;
}user;
user users[MAXUSERS];
int check_user(struct sockaddr_in *user);
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
    fd_set readfds, master;
    FD_ZERO(&readfds);
    FD_ZERO(&master);
    FD_SET(udpSocket, &master);
    int uid;
    char buf[1024];
 
    while (1) {
        readfds = master;
        if(select(udpSocket + 1, &readfds, NULL, NULL, NULL)){
	        nBytes =
	            recvfrom(udpSocket, buffer, 1024, 0, (struct sockaddr*)&serverStorage, &addr_size);
            buffer[nBytes] = '\0';
            struct sockaddr_in *p = (struct sockaddr_in*)&serverStorage;
                if((uid = check_user(p)) > 0){
                    sprintf(buf, "%s->%s\n", users[uid].name, buffer);         
                 
                    int sender = ntohs(p->sin_port);
                    for(i = 0; i < cnt; i++){
                
                        if(users[i].port != sender){
                            p->sin_port = htons(users[i].port); 
                            sendto(udpSocket, buf, nBytes, 0, (struct sockaddr*)&serverStorage, addr_size);
                        }
                    }
                   
        
                } else{
                    sendto(udpSocket, "Your signed in Continue\n", 24, 0, (struct sockaddr*)&serverStorage, addr_size);
                    strcpy(users[cnt].name, buffer);
            
                    sprintf(buffer, "%s JOINED!!!!!!\n", users[cnt].name);
                    users[cnt++].port = ntohs(p->sin_port);
                    int sender = ntohs(p->sin_port);
                    
                    int i ;
                    for(i = 0; i < cnt; i++) {
                        if(users[i].port != sender){
                            p->sin_port = htons(users[i].port);
                            sendto(udpSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&serverStorage, addr_size);
                        }
                    
                    }
       
                }
            FD_CLR(udpSocket, &readfds);
        }
                     
    }        
		     
    return 0;
}
int check_user(struct sockaddr_in *user){
    int i;
    int sender = ntohs(user->sin_port);
    for(i = 0; i < cnt; i++){
        if(users[i].port == sender){
            return i;
        }
    }
   
    return 0;
}      
