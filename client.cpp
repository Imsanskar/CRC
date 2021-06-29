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

int main(int argc, char* argv[]){
	int socketfd, connect_fd;
	struct addrinfo hints, *serverinfo, *itr;

	memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo("127.0.0.1", "6969", &hints, &serverinfo);
	if(status != 0){
		printf("Getaddrinfo error: %s\n", gai_strerror(status));
	}
	
	socketfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
	if (socketfd == -1){
		perror("socket error on client side")
	}

	//connect
	status = connect(socketfd, serverinfo->ai_addr, serverinfo->ai_addrlen);
	if(status != 0){
		perror("Connect Error");
	}

	char message[12];
	int bytesLength = recv(socketfd, message, 12, 0);
	printf("%s\n", message);
}