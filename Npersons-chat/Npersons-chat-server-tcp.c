#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define MAXUSERS 50
int cnt = 0;
typedef struct u{
    int fd;
    int uid;
}*users[MAXUSERS];
int main(void)
{
    int msock, newsock;
    struct sockaddr_in serveraddr, useraddr;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(8888);
    serveraddr.sin_family = AF_INET;
    memset(serveraddr.sin_zero, '\0', sizeof(serveraddr.sin_zero));
    int fdmax;
    if((msock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("can't create socket\n");
        return (1);
    }
    if(bind(msock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1){
        perror("can't bind\n");
        return(2);
    }
    int i, j;
    socklen_t addr_size = sizeof(useraddr);
    listen(msock, 10);
    char buffer[1024];
    fd_set readfds, master;
    FD_ZERO(&master);
    FD_ZERO(&readfds);
    FD_SET(msock, &master);
    fdmax = msock;
    int nbytes;
    for(;;){
        readfds = master;
        if(select(fdmax + 1, &readfds,NULL, NULL, NULL) != -1){
            for(i = 0; i < fdmax; i++){
                if(FD_ISSET(i, &readfds)){
                    if(i == msock){
                        printf("we got one connection.....\n");
                        newsock = accept(msock, (struct sockaddr*)&useraddr, &addr_size);
                        FD_SET(newsock, &master);
                        if(newsock > fdmax){
                            fdmax = newsock;
                        }
                        if((nbytes = recv(i, buffer, 1024, 0)) >= 0){
                            if(nbytes == 0){
                                close(i);
                                FD_CLR(i, &master);
                            } else{
                                for(j = 0; j <= fdmax; j++){
                                    if(j != msock && j != i){
                                        send(j, buffer, nbytes, 0);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } 
                    
    return 0;
}               
    
            
            
                
