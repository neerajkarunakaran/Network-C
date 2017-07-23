#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
int main(void)
{
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    socklen_t addr_size = sizeof serverAddr;
    fd_set readfds, master;
    FD_ZERO(&readfds);
    FD_ZERO(&master);
    FD_SET(0, &master);
    FD_SET(clientSocket, &master);
    char buffer[1024];
    int nBytes;
    printf("Enter Your Name and wait for reply from  groop\n");
    nBytes = read(0, buffer, 1024);
    buffer[nBytes] = '\0';
    sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, addr_size);
    fflush(0);   
    while(1){
        readfds = master;
        select(clientSocket + 1, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(0, &readfds)){
            nBytes = read(0, buffer, 1024);
            buffer[nBytes] = '\0';
            sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, addr_size);
            
            FD_CLR(0, &readfds);
        }
        if(FD_ISSET(clientSocket, &readfds)){
            nBytes = recvfrom(clientSocket, buffer, 1024, 0, (struct sockaddr*)&serverAddr, &addr_size);
            buffer[nBytes] = '\0';
            printf("%s\n", buffer);
        
            FD_CLR(clientSocket, &readfds);
        }
    }
                  
                
           
                
               
    return 0;
}
        
 
    
           
