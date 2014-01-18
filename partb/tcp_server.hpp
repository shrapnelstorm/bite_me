#ifndef TCP_SERVER
#define TCP_SERVER

#include "main_header.hpp"
/*
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h> // for the ioctl() function call
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
*/
#include "pipe.hpp"

// TODO: the current # of connections to wait on is 32, is this adequate
// TODO: ensure server is closing unused tcp connections

class TCPServer {
	private:
		struct addrinfo server_info, *server_info_list ;
		int server_socket ;
		fd_set 	master_fds, working_fds ; // socket descriptors we're listening to
		int  max_sd, new_fd ; // max_fd stores highest valued fd

	public:
		TCPServer(char address[], char port[]) ;
		~TCPServer() ;
		void runServer() ;

	private:
		void initServer(char address[], char port[]) ;
		void bindSocketAndListen() ;
		void iterateFDSet(fd_set *ready_set, int &ready_count) ;
		void addClients() ;
		void removeClient(int client) ;
};

TCPServer::TCPServer(char address[], char port[]) {
	initServer(address, port) ;
	bindSocketAndListen() ;
}

/* free address info list and close connection
 */
TCPServer::~TCPServer() {
	freeaddrinfo(server_info_list) ;
	close(server_socket) ;
}

void TCPServer::runServer() {
	
	fflush(stdout) ;
	// no timeout. Our server uses select() for polling
	timeval	timeout ;
	timeout.tv_sec = 0 ; 
	timeout.tv_usec = 0 ;

	max_sd = server_socket ; // max_fd stores highest valued fd
	FD_SET(server_socket, &master_fds) ; // add server socket to master_fds

	// listen for connections & process existing conn.
	int num_ready = 0 ;
	while (true) {
		// copy master set to working set
		memcpy(&working_fds, &master_fds, sizeof(master_fds)) ;

		// call select()
		//select(max_file_desc, read_set, write_set, exceptions, timeout)
		num_ready = select(max_sd + 1, &working_fds, NULL, NULL, &timeout) ;

		// check if select failed 
		if ( num_ready < 0 ) {
		}

		// select() timed out, this shouldn't happen since we're using
		// select() for polling
		else if ( num_ready == 0 ) {
		}
		else {
			printf("got a connection\n") ;
			fflush(stdout) ;
			// iterate through the processes that are ready
			iterateFDSet(&working_fds, num_ready) ;
			//iterateFDSet(&writing_fds, num_ready, max_sd, socket) ;
		}

	}
}

void TCPServer::iterateFDSet(fd_set *ready_set, int &ready_count) {
	printf("max sd is %i, server_sd is %i", max_sd, server_socket) ;
	fflush(stdout) ;
	for (int i=0 ; i <= max_sd && ready_count > 0; i++) {
		if ( !(FD_ISSET(i, ready_set)) )
			continue ;

		if (i == server_socket ) {
		// accept all incomming connections
			addClients() ;
		} else {
			printf("got a message") ;
	fflush(stdout) ;
			char buffer[255] ;
			int recv_count ;
			memset(buffer, 0, 255) ;
			recv_count = recv(i, buffer, 255, 0 ) ;
			if ( recv_count <= 0 ) {
				removeClient(i) ;
			} else {
				printf("received message:%s\n", buffer) ;
			}
		}
	}
}

void TCPServer::addClients() {
	int new_socket ;

	do {
		// TODO: should we instantiate sockaddr_in objects ??
		new_socket = accept(server_socket, NULL, NULL) ;
		printf("adding new client with socket %i \n", new_socket) ;
		exitWithError(new_socket < 0 && errno != EWOULDBLOCK, "accept failed\n") ;

		FD_SET(new_socket, &master_fds) ; // add new client to fd_list

		// TODO: initialize new client struct !!

		if (new_socket > max_sd) {
			this->max_sd = new_socket ;
			printf("max_sd is now %i\n", new_socket) ;
		}

	} while (new_socket != -1) ;

}

void TCPServer::removeClient(int client) {
	// TODO: remove any data too
	// TODO: remove isn't happening until some other event occurs
	printf("client %i removed", client) ;
	FD_CLR(client, &master_fds) ;
}

/*
 * create a server socket for specified port and address
 */
void TCPServer::initServer(char address[], char port[]) {
	
	// setup server address info
	memset(&server_info, 0, sizeof server_info) ;
	server_info.ai_family	= AF_UNSPEC ;
	server_info.ai_socktype = SOCK_STREAM ;
	
	// change argv[1] to NULL to automatically use localhost
	int status = getaddrinfo(address, port, &server_info, &server_info_list) ;
	exitWithError( status != 0, "getaddrinfo() failed: ") ;
	
	// create the socket
	server_socket =	socket(server_info_list->ai_family, server_info_list->ai_socktype, server_info_list->ai_protocol) ;
	exitWithError(server_socket == -1, "error creating server socket") ;
}

void TCPServer::bindSocketAndListen() {
	// bind socket to port
	int yes=1, status;
	// ensure socket is reusable right after being closed
	status = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int) );
	status = bind(server_socket, server_info_list->ai_addr, server_info_list->ai_addrlen) ;

	exitWithError(status == -1, "bind error") ;

	// ensure the parameters to listen() are correct, buffer size is 32
	exitWithError((listen(server_socket, 32) == -1), "listen error") ;
}


#endif // TCP_SERVER
