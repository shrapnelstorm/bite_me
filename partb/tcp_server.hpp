#ifndef TCP_SERVER
#define TCP_SERVER

#include "main_header.hpp"
#include "twowaypipe.hpp"
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
//#include "pipe.hpp"

// TODO: the current # of connections to wait on is 32, is this adequate
// TODO: ensure server is closing unused tcp connections

class TCPServer {
	private:
		struct addrinfo server_info, *server_info_list ;
		int server_socket ;
		fd_set 	master_fds, working_fds ; // socket descriptors we're listening to
		TwoWayPipe* tcp_pipe ;
		int  max_sd, new_fd ; // max_fd stores highest valued fd

	public:
		TCPServer(char address[], char port[], TwoWayPipe* pipe) ;
		~TCPServer() ;
		void runServer() ;

	private:
		void initServer(char address[], char port[]) ;
		void bindSocketAndListen() ;
		void iterateFDSet(fd_set *ready_set, int &ready_count) ;
		void addClients() ;
		void removeClient(int client) ;
};



#endif // TCP_SERVER
