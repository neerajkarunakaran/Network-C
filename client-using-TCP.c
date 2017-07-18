#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT "3490"
#define MAXDATASIZE 100
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
	return &(((struct sockaddr_in *) sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

int main(void)
{
    int sockfd, numread;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    char *buf;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	return (1);
    }
    struct sockaddr *c = servinfo->ai_addr;
    struct sockaddr_in *a = (struct sockaddr_in *) c;
    inet_pton(AF_INET, "127.0.0.1", &(a->sin_addr));
    servinfo->ai_addrlen = sizeof(a->sin_addr);

    for (p = servinfo; p != NULL; p = p->ai_next) {
	if ((sockfd =
	     socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
	    fprintf(stderr, "secket\n");
	    continue;
	}
	if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
	    close(sockfd);
	    fprintf(stderr, "connect\n");
	    continue;
	}
	break;
    }

    if (p == NULL) {
	fprintf(stderr, "client: failed connect\n");
	return (2);
    }
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *) p->ai_addr), s,
	      sizeof(s));
    printf("client connecting to %s...\n", s);
    freeaddrinfo(servinfo);
    while (1) {
	buf = (char *) malloc(100 * sizeof(char));
	printf("me:");
	scanf("%s", buf);
	write(sockfd, buf, strlen(buf));
	//   send(sockfd, buf, strlen(buf), 0);
	free(buf);
	buf = (char *) malloc(100 * sizeof(char));
	int nread = read(sockfd, buf, 99);
	/*   if((numread = recv(sockfd, buf, 99, 0)) == -1){
	   fprintf(stderr, "recv\n");
	   exit(1);
	   } */
	buf[nread] = '\0';
	printf("server: '%s \n", buf);
	free(buf);
    }
    close(sockfd);
    return 0;
}
