##
# Project Title
#
# @file
# @version 0.1

CC=g++

default: server client
	echo "Server and client have been built!"

server: server.cpp
	$(CC) $^ -o $@

client: client.cpp
	$(CC) $^ -o $@

.PHONY: clean
clean:
	rm server client

# end
