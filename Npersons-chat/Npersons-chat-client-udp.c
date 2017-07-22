#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
#include <pthread.h>
void* readfunction(void* clientSocket);
void* recvfunction(void* clientSocket);
int main(void)
{
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    pthread_t t1, t2;
                
                
           
                
               
    pthread_create(&t1, 0, readfunction, &clientSocket);
    pthread_create(&t2, 0, recvfunction, &clientSocket);
    int status;
    pthread_join(t1,(void*) &status);
    pthread_join(t2,(void*) &status);
    return 0;
}
void* readfunction(void* clientSocket)
{
    int clientsocket = *((int*)clientSocket);
    struct sockaddr_in serverAddr;
    char buffer[1024];
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    socklen_t addr_size = sizeof serverAddr;
    int nBytes;
    while(1){
        fgets(buffer, 1024, stdin);
        nBytes = strlen(buffer);
        sendto(clientsocket, buffer, nBytes, 0, (struct sockaddr*)&serverAddr, addr_size);
    }
}
        
 
void* recvfunction(void* clientSocket)
{
    int nBytes;
    int clientsocket = *((int*)clientSocket);
    struct sockaddr_storage serverAddr;
    socklen_t addr_size;
    char buffer[1024];
    while(nBytes = recvfrom(clientsocket, buffer, 1024, 0, (struct sockaddr*)&serverAddr, &addr_size)){
        struct sockaddr_in *p = (struct sockaddr_in*)&serverAddr;
        printf("port %d: %s\n", ntohs(p->sin_port), buffer);
    }
}    
           
