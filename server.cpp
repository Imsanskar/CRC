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
	if (socketfd == -1){
		perror("socket error on server side");
	}

	int yes = 1; // throwaway optval parameter for setsockopt
	// only for localhost testing purposes, disable SO_REUSEADDR for deployment
	// allows you to forcibly connect to a port, for address already in use binding error
	if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
            perror("setsockopt");
            exit(1); // exit if we can't set socket option? hmmm
        }

	if(bind(socketfd, serverinfo->ai_addr, serverinfo->ai_addrlen) == -1){
		perror("bind");
		exit(1);
	}

	//listen
	status = listen(socketfd, 2);
	if(status == -1){
		perror("Listen error");
		exit(1);
	}

	// socklen_t addr_s
	struct sockaddr_storage client_info;
	socklen_t length = sizeof(client_info);
	connect_fd = accept(socketfd, (struct sockaddr*)&client_info, 
		&length);
	if(connect_fd < 0){
		perror("accept error");
		exit(1);
	}

	const char *msg = "Hello there!";
	int len, bytes_sent;
	len = strlen(msg);
	if ((bytes_sent = send(connect_fd, msg, len, 0)) == -1){
		perror("send error");
		exit(1);
	}
}
