#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>
void server_listen(int socket) ;

int main(int argc, char** argv) {
	
	// check for cmd line arguments
	if (argc < 3) {
		printf("usage: ./550server <ip_addr> <port>") ;
	}

	// setup server address info
	struct addrinfo server_info, *server_info_list ;
	memset(&server_info, 0, sizeof server_info) ;
	server_info.ai_family	= AF_UNSPEC ;
	server_info.ai_socktype = SOCK_STREAM ;

	// change argv[1] to NULL to automatically use localhost
	int status = getaddrinfo(argv[1], argv[2], &server_info, &server_info_list) ;
	if (status != 0) std::cout << "getaddrinfo() failed: " << gai_strerror(status) ;


	// create the socket
	int server_socket =	socket(server_info_list->ai_family, server_info_list->ai_socktype, server_info_list->ai_protocol) ;
	if (server_socket == -1) {
		std::cout << "error creating server socket" ;
		exit(1) ;
	}
	
	// bind socket to port
	int yes=1 ;
	status = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int) );
	status = bind(server_socket, server_info_list->ai_addr, server_info_list->ai_addrlen) ;

	if (status == -1) {
		std::cout << "bind error" << std::endl ;
	}


	// ensure the parameters to listen() are correct
	if ( listen(server_socket, 6) == -1)  {// can buffer 6 connections
		std::cout << "listen error" << std::endl ;
		exit(1) ;
	}


	// free the server_info_list 
	return 0 ;
}


void server_listen(int socket) {
	// setup the select data structures that are necessary


	// listen for connections & process existing conn.
	while (true) {
	}
}


