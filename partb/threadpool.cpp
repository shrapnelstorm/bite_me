// XXX: WARNING, I've disabled all pipe write and read lines
// until we resolve a serialization issue.
#include <iostream>
#include "threadpool.hpp"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


Thread_pool* Thread_pool::tp_instance = NULL ;

Thread_output::Thread_output(void* mmp, int s_id, int fd){
	memory_map = mmp; // do we need a copy constructor here?
	socket_id = s_id;
	fd = fd;
}

Thread_pool::Thread_pool(){
	thread_pool() ;

	// set the availability of each thread to 1
	for ( int i = 0 ; i < thread_no; i++) {
		avail[i] = 1;
	}

}

Thread_pool* Thread_pool::getThreadPool() {
	if ( tp_instance == NULL) {
		tp_instance = new Thread_pool() ;
	} 
	
	return tp_instance ;
}

// ================== thread helper, helper functions ====================
void Thread_pool::makeHelperAvailable(int helper_id) {
		pthread_mutex_lock(&(thread_avail[helper_id]));
		avail[helper_id] = 1;
		pthread_mutex_unlock(&(thread_avail[helper_id]) );
}

void* Thread_pool::helper_thread(void *t_id){

	const int thread_id = *((int*) t_id) ;
	delete ((int*) t_id) ; // free heap memory

	Thread_pool* tpool = Thread_pool::getThreadPool() ;
	TwoWayPipe tpool_pipe = tpool->pipe_threads[thread_id] ;
	TwoWayPipe tcp_pipe = tpool->p ;

	int fd;
	void* map;
	int result;
	Thread_input input;
	//result = dup2(pipe_rw.read, STD_IN);
	//if(result == -1){
	//	perror("dup2 failed");	
	//}	
	while(1){
		//int read_size = tpool_pipe.threadpoolRead(input, sizeof(input));
		fd = open(input.filename, O_RDONLY);
		if(fd == -1){
			perror("Error opening file");
			// should we exit here?	
			// TODO: notify tcp server that file isn't available
		}
		map = mmap(0,FILESIZE, PROT_READ, MAP_SHARED,fd,0);
		if(map == MAP_FAILED){
			// TODO: how should we respond? should we keep trying?	
		}
		
		Thread_output output(map,input.socket_id,fd);
		// send output via pipe
		//tcp_pipe.threadpoolWrite(output,sizeof(output));
		tpool->makeHelperAvailable(thread_id) ;
	}
	return NULL ;
}

void Thread_pool::runThreadpool() {
	int i ;
	Thread_input input;
	while(1){
		//p.threadpoolRead(input,sizeof(input));
		for(i=0;i<thread_no;i++){
			// TODO: better way to handle notifications?
			// This will slow down our threadpool
			pthread_mutex_lock(&(thread_avail[i]));
			if(avail[i]){
				avail[i] = 0;
				pthread_mutex_unlock(&(thread_avail[i]));
				break;			
			}
			pthread_mutex_unlock(&(thread_avail[i]));		
		}
		//pipe_threads[i].serverWrite(&input, sizeof(input)) ;
	}		
}

// XXX: Siva, why did this originally have a void* param??
void Thread_pool::thread_pool(){
	int i;
	for(i=0;i<thread_no;i++){
		int* thread_id  = new int ;
		*thread_id = i ;
		pthread_create(&thread[i],NULL,helper_thread,(void *)thread_id);	
	}
}
