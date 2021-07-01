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

#include "utils.h"
#include "crc_div.h"
// C++ specific headers
#include <iostream>

#define PORT "6969" // port number
#define BACKLOG 10 // total number of backlog connection
#define BUF_SIZE 512

int main(int argc, char* argv[]){
	int socketfd, connect_fd;
	struct addrinfo hints, *serverinfo, *itr;

	memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo("127.0.0.1", PORT, &hints, &serverinfo);
	if (status != 0){
		fprintf(stderr, "getaddrinfo: %s", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	int _y = 1; // throwaway optval parameter for setsockopt
	// traverse the result linked list and bind to the first valid one
	for (itr = serverinfo; itr != NULL; itr = itr->ai_next){
		socketfd = socket(itr->ai_family, itr->ai_socktype, itr->ai_protocol);
		if (socketfd == -1){
			perror("socket error on server side");
			continue;
		}

		// only for localhost testing purposes, disable SO_REUSEADDR for deployment
		// allows you to forcibly connect to a port, for address already in use binding error
		if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &_y, sizeof(int)) == -1){
			// failing to set sockopt is not a serious errror
			// but something not to be missed
            perror("setsockopt");
        }

		if(bind(socketfd, itr->ai_addr, itr->ai_addrlen) == -1){
			close(socketfd);
			perror("bind");
			continue;
		}

		break;
	}

	if (itr == NULL){
		fprintf(stderr, "server: failed to bind an address\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(serverinfo);

	// listen
	status = listen(socketfd, BACKLOG);
	if(status == -1){
		perror("Listen error");
		exit(EXIT_FAILURE);
	}

	// socklen_t addr_s
	struct sockaddr_storage client_info;
	socklen_t length = sizeof(client_info);

	// crc divisor, must be same on the server and the client side
	char divisor[] = "1010";
	std::cout << "Dvisior for CRC division : " << divisor << std::endl;

	char recv_msg[BUF_SIZE];
	int len, bytes_sent, bytes_recv;
	memset(recv_msg, 0, BUF_SIZE);

	while (true){
		connect_fd = accept(socketfd, (struct sockaddr*)&client_info,
							&length);
		if(connect_fd < 0){
			perror("accept error");
			exit(EXIT_FAILURE);
		}

		// recieve encoded message from client
		bytes_recv = recv(connect_fd, &recv_msg, BUF_SIZE, 0);
		if (bytes_recv == -1){
			perror("recieve error");
			continue;
		}
		const char* rem = crc_div(recv_msg, divisor, strlen(recv_msg), strlen(divisor));
		std::cout << "Received encoded message : " << recv_msg << std::endl;
		std::cout << "CRC division remainder on server side : " << rem << std::endl;
		char *dataword;
		dataword = removeRemainderCRC(recv_msg, dataword, 3);
		printf("Message: %s\n", dataword);
		Message message = charToMessage(dataword, strlen(dataword) / 8, 8);
		// set_message(&message, dataword);
		// printf("Message size: %d\n", message.size);
		print(message);
		if ((bytes_sent = send(connect_fd, &recv_msg, bytes_recv, 0)) == -1){
			perror("send error");
		}

		delete rem;
	}

	return 0;
}
