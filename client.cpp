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

// C++ specific header
#include <iostream> // for cin, scanf is error prone
#include "crc_div.h"
#include "utils.h"

#define PORT "6969" // port number
#define BUF_SIZE 512

int main(int argc, char* argv[]){
	int socketfd, connect_fd;
	struct addrinfo hints, *serverinfo, *itr;

	// INET6_ADDRSTRLEN can store both ipv4/ipv6 addresses
	char server_addr[INET6_ADDRSTRLEN]; 

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

	for (itr = serverinfo; itr != NULL; itr = itr->ai_next){
		socketfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
		if (socketfd == -1){
			perror("socket error on client side");
			continue;
		}

		//connect
		status = connect(socketfd, serverinfo->ai_addr, serverinfo->ai_addrlen);
		if(status != 0){
			close(socketfd);
			perror("Connect Error");
			continue;
		}

		break;
	}

	if (itr == NULL){
		fprintf(stderr, "connection to server failed!");
		exit(EXIT_FAILURE);
	}

	// msg_to_send will point to the ascii input
	std::string msg_to_send;
	std::cout << "Message to send :";
	std::getline(std::cin, msg_to_send);

	// send_message will include binary representation of msg_to_send
	Message send_message;
	const char* s_msg = msg_to_send.c_str();
	set_message(&send_message, s_msg);
	char* msg = messageToChar(send_message, msg);
	std::cout << msg_to_send << " in binary : " << msg << std::endl;

	// crc divisor, must be same on the server and client side
	char divisor[] = "1010";
	std::cout << "Dvisior for CRC division : " << divisor << std::endl;

	const char* rem = crc_div(msg, divisor, strlen(msg), strlen(divisor));
	std::cout << "CRC division remainder on client side : " << rem << std::endl;

	// concatenate crc remainder to original message
	std::string code(msg);
	code = code + std::string(rem);
	std::cout << "CRC encoded message : " << code << std::endl;

	// send crc encoded message
	int send_length = send(socketfd, code.c_str(), code.size(), 0);
	if (send_length == -1){
		perror("client send message");
		exit(EXIT_FAILURE);
	}

	// receive encoded message from server
	char message[BUF_SIZE];
	int recv_length = recv(socketfd, message, BUF_SIZE, 0);
	message[recv_length] = '\0';
	printf("Echoed message from server : %s\n", message);

	delete rem;
	return 0;
}
