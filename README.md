# CRC
 
 Cyclic Redundancy Check (CRC) implementation in C++.
 
## Requirements

- Linux/UNIX based Operating System (doesn't work with Windows)
- GNU Make (optional as build systems)

## Build Steps

1. `git clone` this repository.
2. `cd` into the directory 
3. `make` to generate the build files using GNU Make. If you don't have make execute the following commands manually -

``` shell
$ g++ server.cpp -o server
$ g++ client.cpp -o client
```

## How to run?

1. Open a terminal session and execute the server build `./server`. By default the server will start on port 6969. If you want to connect to any other port, change the `PORT` macro in `server.cpp` and re-build the system.
2. Open another terminal session while the server is active and execute `./client`. By default, the client connects to localhost on `127.0.0.1`. If server is running on a remote session, then you can specify the server address as command line argument -

``` shell
$ ./client remote-ip
```
IP can be either ipv4 or ipv6, both formats supported.

## How can I contribute?

You can fork the repository, make adequate changes then make a pull request to our original repository. Following points might give you can idea about how you can contribute - 

1. Separate the definition in header files into their own files.
2. Make a multi-process/multi-thread supporting server on top of the existing code.
3. Artificial noise introduction for real world CRC check simulation.
4. CRC code check on client side after response from server is remaining, although it should be fairly simple and straightforward as in `server.cpp`.

