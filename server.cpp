#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "6969" // port number
#define BACKLOG 10 // total number of backlog connection



//TODO: Fix the message bug

int main(int argc, char* argv[]){
	int socketfd, connect_fd;
	struct addrinfo hints, *serverinfo, *itr;

	memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo("127.0.0.1", "6969", &hints, &serverinfo);
	
	socketfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);

	bind(socketfd, serverinfo->ai_addr, serverinfo->ai_addrlen);

	//listen
	status = listen(socketfd, 2);
	if(status < 0){
		perror("Listen error");
	}
	// socklen_t addr_s
	socklen_t length = serverinfo->ai_addrlen;
	connect_fd = accept(socketfd, serverinfo->ai_addr, &length);

	if(connect_fd < 0){
		perror("accept error");
	}

	char *msg = "Hello there!";
	int len, bytes_sent;
	len = strlen(msg);
	bytes_sent = send(connect_fd, msg, len, 0);
}