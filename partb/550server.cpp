#include "main_header.hpp"
#include "tcp_server.hpp"
#include "threadpool.hpp"
#include <pthread.h>

void* runThreadPool(void* pipe) ;

int main(int argc, char** argv) {
	
	// check for cmd line arguments
	exitWithError( argc < 3, "Usage: ./550server <ip_addr> <port>", 0) ;

	// create pipe, and run server
	TwoWayPipe tcp_pipe ;
	TCPServer server(argv[1], argv[2], &tcp_pipe) ;

	// run thread pool in separate thread
	pthread_t tp_thread ;
	pthread_create(&tp_thread,NULL,runThreadPool,(void *)&tcp_pipe);	
	
	server.runServer() ;

	return 0 ;
}

void* runThreadPool(void* pipe) {
	Thread_pool* thread_pool = Thread_pool::getThreadPool() ;
	thread_pool->setTCPPipe( *((TwoWayPipe *)pipe) ) ;
}
