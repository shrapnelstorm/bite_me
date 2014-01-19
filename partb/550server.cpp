#include "main_header.hpp"
#include "tcp_server.hpp"
#include "threadpool.hpp"

int main(int argc, char** argv) {
	
	// check for cmd line arguments
	exitWithError( argc < 3, "Usage: ./550server <ip_addr> <port>", 0) ;

	TCPServer server(argv[1], argv[2]) ;
	server.runServer() ;
	return 0 ;
}
