/************************************************************
 * Program: 550server
 *
 * Desc:	An implementation of an amted server. This
 * 			file instantiates a threadpool and tcp_server
 * 			with a two way pipe between them. The
 * 			server is run on the main thread and the 
 * 			threadpool is run on a separate p_thread 
 *************************************************************/

#include "main_header.hpp"
#include "tcp_server.hpp"
#include "threadpool.hpp"
#include <pthread.h>

void* runThreadPool(void* pipe) ;

int main(int argc, char** argv) {
	
	// check for cmd line arguments, fail if none given
	exitWithError( argc < 3, "Usage: ./550server <ip_addr> <port>", 0) ;

	// create pipe, and run server
	TwoWayPipe tcp_pipe ;
	TCPServer server(argv[1], argv[2], &tcp_pipe) ;

	// run thread pool in separate thread
	pthread_t tp_thread ;
	pthread_create(&tp_thread,NULL,runThreadPool,(void *)&tcp_pipe);	
	
	// main thread runs the server
	server.runServer() ;

	return 0 ;
}

/* 
 * function executed by a pthread to run a threadpool implementation.
 */
void* runThreadPool(void* pipe) {
	Thread_pool* thread_pool = Thread_pool::getThreadPool() ;
	thread_pool->setTCPPipe( (TwoWayPipe *)pipe ) ;
	thread_pool->runThreadpool() ;
	return NULL ;
}
