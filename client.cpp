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

int main(int argc, char* argv[]){
	int socketfd, connect_fd;
	struct addrinfo hints, *serverinfo, *itr;

	// INET6_ADDRSTRLEN can store both ipv4/ipv6 addresses
	char server_addr[INET6_ADDRSTRLEN]; 
	bool loopback_connection = true;

	// maybe need to add option for port as well, or separate this into
	// another function, TODO
	switch(argc){
		case 1:
			strcpy(server_addr, "127.0.0.1\0"); 
			break;
		case 2:
			strcpy(server_addr, argv[1]);
			break;
		default:
			fprintf(stderr, 
					"Program exited due to too many parameters. \
					Provide only one server address to connect to.");
			exit(1);
			break;
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(server_addr, PORT, &hints, &serverinfo);
	if(status != 0){
		printf("Getaddrinfo error: %s\n", gai_strerror(status));
	}
	
	socketfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
	if (socketfd == -1){
		perror("socket error on client side");
	}

	//connect
	status = connect(socketfd, serverinfo->ai_addr, serverinfo->ai_addrlen);
	if(status != 0){
		perror("Connect Error");
	}

	char message[100];
	int bytesLength = recv(socketfd, message, 100, 0);
	message[bytesLength] = '\0';
	printf("%s\n", message);
}
