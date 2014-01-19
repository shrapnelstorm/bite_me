#include "tcp_server.hpp"
#include <iostream>

TCPServer::TCPServer(char address[], char port[], TwoWayPipe* pipe) {
	initServer(address, port) ;
	bindSocketAndListen() ;
	tcp_pipe = pipe ;
}

/* free address info list and close connection
 */
TCPServer::~TCPServer() {
	freeaddrinfo(server_info_list) ;
	close(server_socket) ;
}

void TCPServer::runServer() {
	
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
			// iterate through the processes that are ready
			iterateFDSet(&working_fds, num_ready) ;
			//iterateFDSet(&writing_fds, num_ready, max_sd, socket) ;
		}

	}
}

void TCPServer::iterateFDSet(fd_set *ready_set, int &ready_count) {

	int loop_max = max_sd ; // max_sd could change, when addClients is called
	for (int i=0 ; i <= loop_max && ready_count > 0; i++) {
		if ( !(FD_ISSET(i, ready_set)) )
			continue ;

		if (i == server_socket ) {
		// accept all incomming connections
			addClients() ;
		} else {
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
		exitWithError(new_socket < 0 && errno != EWOULDBLOCK, "accept failed\n") ;

		// add new client to map and fd_set
		FD_SET(new_socket, &master_fds) ; 
		ClientData new_client ;
		client_map[new_socket] = new_client ; 

		// TODO: initialize new client struct !!

		if (new_socket > max_sd) {
			this->max_sd = new_socket ;
		}

	} while (new_socket != -1) ;
}

void TCPServer::removeClient(int client) {
	// TODO: remove any data too
	// TODO: remove isn't happening until some other event occurs
	FD_CLR(client, &master_fds) ;
	client_map.erase(client) ;
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

	// set the socket to nonblocking
	fcntl(server_socket, F_SETFL, O_NONBLOCK) ;
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


// code to communicate with clients
ClientData TCPServer::sentToClient(ClientData data) {
	return data ;	
}
