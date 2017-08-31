// http server program
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <malloc.h>

// Extract method from client/user requist
char *getmethod(char *Requist)
{
    char *method;
    method = strtok(Requist, " ");
    return method;
}

//Extract uri from user/client requist
char *getRequistURI(char *Requist)
{
    char *RequistURI;
    RequistURI = strtok(NULL, " ");
    return RequistURI;
}

//get information of file by using stat sys call
struct stat *get_fileinfo(char *FilePath)
{
    struct stat *s = (struct stat *) malloc(sizeof(struct stat));
    if (stat(FilePath, s) == 0) {

	return s;
    }
}

//return client/user requisted file content size in bytes
int getfilesize(struct stat *s)
{
    return s->st_size;
}

//get current time
char *get_tdate(void)
{
    time_t t;
    struct tm *tm;
    char *tdate = (char *) malloc(50 * sizeof(char));
    t = time(NULL);
    tm = localtime(&t);
    sprintf(tdate, "%s", asctime(tm));
    return tdate;
}

//get  last modified time of file requisted by client/user
char *get_fmod(struct stat *st)
{
    char *filemod = (char *) malloc(50 * sizeof(char));
    sprintf(filemod, "%s", ctime(&(st->st_mtime)));
    return filemod;
}

//return the file extention
char *fextention(char *path)
{
    char *token, *file, *ftype, *type;
    type = (char *) malloc(15 * sizeof(char));
    token = strtok(path, "/");
    file = token;
    while ((token = strtok(NULL, "/")) != NULL) {
	file = token;
    }
    ftype = strtok(file, ".");
    ftype = strtok(NULL, ".");
    if ((strcmp(ftype, "txt") == 0) || (strcmp(ftype, "TXT") == 0)
	|| (strcmp(ftype, "html") == 0)) {
	sprintf(type, "txt/html");
    } else if ((strcmp(ftype, "JPF") == 0) || (strcmp(ftype, "jpg") == 0)
	       || (strcmp(ftype, "gif") == 0)) {
	sprintf(type, "image/gif");
    }
    return type;
}

//send client/user requisted file
void sendfile(char *FilePath, int userFd)
{
    int fd, fsize;
    char *Response, *content;
    struct stat *st;
    if ((fd = open(FilePath, O_RDONLY)) > 0) {

	st = get_fileinfo(FilePath);
	fsize = getfilesize(st); printf("%d\n", fsize);
	content = (char *) malloc(fsize * sizeof(char));
	    if (read(fd, content, fsize) > 0) {
	        Response = (char *) malloc((200 + fsize) * sizeof(char));
	        sprintf(Response,
		        "HTTP/1.1. 200 OK\nDate: %s\nServer: Localserver/1.0\nLast-Modified:%sContent-Length: %d\nAccept-Ranges: bytes\nConnection: close\nContent-Type: %s,charset=ISO-8859-1\n\n%s",
		        get_tdate(), get_fmod(st), fsize, fextention(FilePath),
		        content);





	        send(userFd, Response, strlen(Response), 0);
            close(userFd);
	        free(st);
	        free(Response);
	        free(content);
	    }
	// send 404 error
    } else {
	    Response = (char *) malloc(20 * sizeof(char));
	    sprintf(Response, "HTTP/1.1 404 Not Found");
	    send(userFd, Response, strlen(Response), 0);
        close(userFd);
        free(Response);
    }
}

// handing defferent types of client/user requists
void server_service(char *method, char *RequistURI, int newfd)
{
    if (strcmp(method, "GET") == 0) {

	sendfile(RequistURI, newfd);
    }
    if (strcmp(method, "POST") == 0) {
	;
    }
}


int main(void)
{
    char *Requist;
    char *method;
    char *RequistURI;
    pid_t pid;
    int sock, addrlen, newfd, fd;
    struct sockaddr_in serveraddr;
    struct sockaddr_storage clientaddr;
    socklen_t clientaddr_len;

    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8010);
    addrlen = sizeof serveraddr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    bind(sock, (struct sockaddr *) &serveraddr, addrlen);

    listen(sock, 5);

    clientaddr_len = sizeof clientaddr;

    while ((newfd =
	    accept(sock, (struct sockaddr *) &clientaddr,
		   &clientaddr_len)) > 0) {

	if ((pid = fork()) == 0) {
	    Requist = (char *) malloc(1024 * sizeof(char));

	    if ((recv(newfd, Requist, 1024 - 1, 0)) > 0) {
		Requist[1023] = '\0';

		method = getmethod(Requist);
		RequistURI = getRequistURI(Requist);

		server_service(method, RequistURI, newfd);


	    }
	    free(Requist);
	    exit(0);

	} else if (pid > 0) {
	    ;
	} else {
	    printf("fork error\n");
	}
    }
    return 0;
}
