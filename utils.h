#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct Binary{
	// a bit binary value
	char *value;
	int no_bits;
};

/*
	Initialize the binary value with a vetain value
	Args:
		binary: pointer to the binary number you want to initialize
		val:	decimal value of which the binary number you want
		no_bits: number of bits in the binary number
*/
void initialize_binary(Binary *binary, int val, int no_bits){
	const int size = 8;
	binary->no_bits = no_bits;
	binary->value = (char *)malloc(sizeof(char) * (no_bits + 1));
	memset(binary->value, 0, size);
	binary->value[size - 1] = '\0';
	for(int i = val, j = size - 1; j >= 0; i = i / 2, j--){
		binary->value[j] = (char)(i % 2) + '0';
	}
}

void set_binary(Binary *binary, const char* str, int no_bits){
	binary->no_bits = no_bits;
	binary->value = (char *)malloc(sizeof(char) * (no_bits + 1));
	binary->value[no_bits] = '\0';
	for(int j = no_bits - 1; j >= 0; j--){
		binary->value[j] = str[j];
	}
}

/*
	Convertes binary to decimal
*/
int to_decimal(Binary binary){
	const int size = 8;
	int number = 0;
	for(int i = size - 1; i >= 0; i--){
		number += (int)(binary.value[i] - '0') * pow(2, size - i - 1);
	}
	return number;
}



/*
	Wrapper for sending the message
*/
struct Message{
	size_t size;
	Binary *message;
};

/*
	Initialize message of size bits
	Args:
		message: pointer to the message which you want to initialize
		size: no of characters in the message
*/
void initialize_message(Message* message, const int size){
	message->size = size;
	message->message = (Binary *)malloc(sizeof(Binary) * size);
	if(!message->size){
		printf("Allocation failed\n");
	}
	for(int i = message->size - 1; i >= 0; i--){
		initialize_binary(&message->message[i], 0, 8);
	}
}


/*
	Sets the string to the message
	Args:
		message: pointer to the message for which you want to set the string to
		str: value of the str
*/
void set_message(Message *message, const char *str){
	message->size = strlen(str);
	message->message = (Binary *)malloc(sizeof(Binary) * message->size);
	for(int i = message->size - 1; i >= 0; i--){
		initialize_binary(&message->message[i], (int)str[i], 8);
	}
}

void print(Message message){
	for(int i = 0; i < message.size; i++){
		printf("%c", to_decimal(message.message[i]));
	}
	printf("\n");
}

/*
	returns a message from string to size
*/
Message charToMessage(char *code, int size, int no_bits){
	Message message;
	initialize_message(&message, size);
	code[message.size * message.message[0].no_bits] = '\0';
	for(int i = 0; i < message.size * message.message[0].no_bits; i++){
		message.message[i / message.message[0].no_bits].value[i % message.message[0].no_bits] = code[i];
	}
	return message;
}

/*
	Message to character pointer
*/
char* messageToChar(Message message, char *code){
	code = (char *)malloc(sizeof(char) * (message.size * message.message[0].no_bits + 1));
	code[message.size * message.message[0].no_bits] = '\0';
	for(int i = 0; i < message.size * message.message[0].no_bits; i++){
		code[i] = message.message[i / message.message[0].no_bits].value[i % message.message[0].no_bits];
	}
	return code;
}

char* removeRemainderCRC(char *codeword, char *dataword, int remLength){
	dataword = (char *)malloc(sizeof(char) * (strlen(codeword) - remLength) + 1);
	dataword[strlen(codeword) - remLength] = '\0';
	for(int i = strlen(codeword) - remLength - 1; i >= 0; i--){
		dataword[i] = codeword[i];
	}
	return dataword;
}
