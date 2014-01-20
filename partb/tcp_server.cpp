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
	FD_SET(server_socket, &master_read_fds) ; // add server socket 
	FD_SET(tcp_pipe->getSRead(), &master_read_fds) ; // add tpool pipe 

	// listen for connections & process existing conn.
	int num_ready = 0 ;
	while (true) {
		// copy master set to working set
		memcpy(&tmp_read_fds, &master_read_fds, sizeof(master_read_fds)) ;
		memcpy(&tmp_write_fds, &master_write_fds, sizeof(master_write_fds)) ;

		// call select()
		//select(max_file_desc, read_set, write_set, exceptions, timeout)
		num_ready = select(max_sd + 1, &tmp_read_fds, &tmp_write_fds, NULL, &timeout) ;

		// check if select failed 
		if ( num_ready < 0 ) {
		}

		// select() timed out, this shouldn't happen since we're using
		// select() for polling
		else if ( num_ready == 0 ) {
		}
		else {
			// iterate read and then write sets.
			iterateWriteSet(&tmp_write_fds, iterateReadSet(&tmp_read_fds, num_ready)) ;
		}

	}
}

int TCPServer::iterateReadSet(fd_set *ready_set, int ready_count) {

	int loop_max = max_sd ; // max_sd could change, when addClients is called
	for (int i=0 ; i <= loop_max && ready_count > 0; i++) {
		if ( !(FD_ISSET(i, ready_set)) )
			continue ;

		ready_count--;
		if (i == server_socket ) {
			addClients() ;
		} else if ( i == tcp_pipe->getSRead() ) {

			// read threadpool message
			Thread_output* helper = new Thread_output ;
			read(i, helper, sizeof(Thread_output) ) ;

			// update client data
			ClientData data = client_map[helper->socket_id] ;
			data.file_addr = helper->memory_map ;
			data.total_bytes = helper->num_bytes ;
			data.state = SENDING_FILE ;
			client_map[helper->socket_id] = data;

			delete helper ;

		} else {
			Thread_input to_thread ;
			to_thread.socket_id = i;

			int recv_count ;
			recv_count = recv(i, to_thread.filename, MAX_FILENAME - 1, 0 ) ; // TODO: loop until newline?
			if ( recv_count <= 0 ) {
				removeClient(i) ;
			} else {
				to_thread.filename[recv_count] = 0 ; // add null terminator
				printf("received message:%s\n", to_thread.filename) ;
				ClientData data = client_map[i];
				data.state = WAITING_MMAP ;

				// switch to write set
				FD_CLR(i, &master_read_fds) ; 
				FD_SET(i, &master_write_fds) ;
				client_map[i] = data ;

				// send request to threadpool
				// TODO: wait until full request received
				tcp_pipe->serverWrite(&to_thread, sizeof(Thread_input) ) ;

			}
		}
	}
	return ready_count ;
}

int TCPServer::iterateWriteSet(fd_set *write_set, int ready_count) {

	int loop_max = max_sd ; // max_sd could change, when addClients is called
	for (int i=0 ; i <= loop_max && ready_count > 0; i++) {
		if ( !(FD_ISSET(i, write_set)) )
			continue ;

		ready_count--;

		ClientData data = client_map[i] ;
		data.bytes_sent += send( i, ((char *)data.file_addr) + data.bytes_sent, data.total_bytes - data.bytes_sent, 0) ;
		client_map[i] = data ;

		if ( data.bytes_sent >= data.total_bytes ) {
			removeClient(i) ;
		} 
	}

	return ready_count ;
}


void TCPServer::addClients() {
	int new_socket ;

	do {
		// TODO: should we instantiate sockaddr_in objects ??
		new_socket = accept(server_socket, NULL, NULL) ;
		exitWithError(new_socket < 0 && errno != EWOULDBLOCK, "accept failed\n") ;

		// add new client to map and fd_set
		FD_SET(new_socket, &master_read_fds) ; 
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
	FD_CLR(client, &master_read_fds) ;
	FD_CLR(client, &master_write_fds) ;
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
