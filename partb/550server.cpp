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


#include <iostream>

void run_server(int socket) ;
void addClient(int server_sock, fd_set* master_fds) ;
void iterateFDSet(fd_set *ready_set, int &ready_count, int &max_socket, int serv_socket) ;

// XXX: figure out weird default argument error
void exitWithError(bool condition, const char msg[], int status=1) {
	if (condition) {
		perror(msg) ;
		exit(status) ;
	}
}

int main(int argc, char** argv) {
	
	// check for cmd line arguments
	exitWithError( argc < 3, "Usage: ./550server <ip_addr> <port>", 0) ;

	// setup server address info
	struct addrinfo server_info, *server_info_list ;
	memset(&server_info, 0, sizeof server_info) ;
	server_info.ai_family	= AF_UNSPEC ;
	server_info.ai_socktype = SOCK_STREAM ;

	// change argv[1] to NULL to automatically use localhost
	int status = getaddrinfo(argv[1], argv[2], &server_info, &server_info_list) ;
	exitWithError( status != 0, "getaddrinfo() failed: ") ;

	// create the socket
	int server_socket =	socket(server_info_list->ai_family, server_info_list->ai_socktype, server_info_list->ai_protocol) ;
	exitWithError(server_socket == -1, "error creating server socket") ;
	
	// TODO: set the socket to non-blocking
	int on=1 ;
	ioctl(server_socket, FIONBIO, (char *)&on) ;
	
	// bind socket to port
	int yes=1 ;
	status = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int) );
	status = bind(server_socket, server_info_list->ai_addr, server_info_list->ai_addrlen) ;

	exitWithError(status == -1, "bind error") ;

	// ensure the parameters to listen() are correct, buffer size is 32
	exitWithError((listen(server_socket, 32) == -1), "listen error") ;

	run_server(server_socket) ;

	// free the server_info_list  & close socket
	freeaddrinfo(server_info_list) ;
	close(server_socket) ;
	return 0 ;
}


void run_server(int socket) {
	// setup the select data structures that are necessary
	fd_set 	master_fds, working_fds ;
	timeval	timeout ;
	int  max_sd = socket, new_fd ; // max_fd stores highest valued fd

	// no timeout. Our server uses select() for polling
	timeout.tv_sec = 0 ; 
	timeout.tv_usec = 0 ;


	// listen for connections & process existing conn.
	int num_ready ;
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
			// iterate through the processes that are ready
			iterateFDSet(&working_fds, num_ready, max_sd, socket) ;
			//iterateFDSet(&writing_fds, num_ready, max_sd, socket) ;
		}


	}
}

/*
 * @return the new max socket descriptor
 */
void iterateFDSet(fd_set *ready_set, int &ready_count, int &max_socket, int serv_socket) {
	for (int i=0 ; i <= max_socket && ready_count > 0; i++) {
		if ( !(FD_ISSET(i, ready_set)) )
			continue ;

		if (i == serv_socket ) {
		// accept all incomming connections
		}
	}
}
// TODO: make this object oriented, so passing parameters is less painful.
/*
 * Creates state struture for new client and adds new socket to
 * the master fd_set
 * @param server_sock socket descriptor for server
 * @param master_fds master list of open socket file descriptors
 */
void addClient(int server_sock, fd_set* master_fds) {
	int new_socket ;
	struct sockaddr_in remote_addr ;
	unsigned int socklen = sizeof(remote_addr) ;

	do {
		new_socket = accept(server_sock, (struct sockaddr*)&remote_addr, &socklen) ;
		exitWithError(new_socket < 0 && errno != EWOULDBLOCK, "accept failed\n") ;

		FD_SET(new_socket, master_fds) ; // add new client to fd_list

		// TODO: initialize new client struct !!

	} while (new_socket != -1) ;

}



