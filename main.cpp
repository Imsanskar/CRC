#include "utils.h"
#include <iostream>
int main(){
	Message m;
	set_message(&m, "abcdefg");
	print(m);
	char *c;
	messageToChar(m, c);
}