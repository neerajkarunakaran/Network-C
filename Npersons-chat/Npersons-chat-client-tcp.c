#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
void *readfunction(void *sock);
void * recvfunction(void *sock);
int main(void)
    
{
    int sock;
    pthread_t t1, t2;
    int status;
    struct sockaddr_in serveraddr;
    serveraddr.sin_port = htons(8888);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serveraddr.sin_zero, '\0', sizeof(serveraddr.sin_zero));
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1){
        perror("can 't connect\n");
        return(1);
    }
 
    pthread_create(&t1, 0, readfunction, &sock);
    pthread_create(&t2, 0, recvfunction, &sock);
    pthread_join(t1, (void*)&status);
    pthread_join(t2, (void*)&status);
    return 0;
}
void* recvfunction(void*sock)
{
    int socket, nbytes;
    socket = *(int*)sock;
    char buffer[1024];
    
    while((nbytes = recv(socket, buffer, 1024, 0)) > 0){
    printf("%s\n", buffer);
    }
}
void *readfunction(void*sock)
{
    int socket = *(int*)sock;
    char buffer[1024];
    while(1){
        fgets(buffer, 1024, stdin);
        send(socket, buffer, strlen(buffer), 0);
    }
}  
    
    
    
        
