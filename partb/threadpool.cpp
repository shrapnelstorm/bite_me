/************************************************************
 * Program: Threadpool class implementation
 * Desc: This implements a theradpool which spawns 'k' helper 
         threads to handle the job of loading the file to the
         main memory. The threadpool and the helper functions
         communicate via pipes. The helper threads use 'mmap' 
	 to load files and sends the reference to the server.
 *************************************************************/

#include <iostream>
#include "threadpool.hpp"
#include "main_header.hpp"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// TODO:for debugging purposes only
#include <unistd.h>
#include <errno.h>

char FILE_ERROR[] = "could not find file\n" ; 
char MMAP_ERROR[] = "server out of memory\n" ; 


Thread_pool* Thread_pool::tp_instance = NULL ;

/*
Constructor which creates pipes to be used to 
communicate with the helper threads. Also, the 
availability of all helper threads is 
initialized to 1.
*/
Thread_pool::Thread_pool(){
	for ( int i = 0 ; i < thread_no ; i++ ) {
		pipe_threads[i] = new TwoWayPipe ; // helper thread communication
	}
	thread_pool() ;

	// set the availability of each thread to 1
	for ( int i = 0 ; i < thread_no; i++) {
		avail[i] = 1;
	}
}
/*
Destructor  - free's memory
*/
Thread_pool::~Thread_pool() {    // delete all the 2Way pipes
	for ( int i = 0 ; i < thread_no ; i++ ) 
		delete pipe_threads[i] ; 
}
/*
Creates a new thread pool instance
*/
Thread_pool* Thread_pool::getThreadPool() {
	if ( tp_instance == NULL) {
		tp_instance = new Thread_pool() ;
	} 
	
	return tp_instance ;
}

// ================== thread helper, helper functions ====================
/*
  Function used by the helper thread to communicate to the thread pool 
  on its availablility for the next job
*/
void Thread_pool::makeHelperAvailable(int helper_id) {   
		pthread_mutex_lock(&(thread_avail[helper_id]));
		avail[helper_id] = 1;
		pthread_mutex_unlock(&(thread_avail[helper_id]) );
}
/*
  The helper thread. Waits for input from the threadpool,
  fetches the file and writes detials back to the server
  for read operation.
*/
void* Thread_pool::helper_thread(void *t_id){

	const int thread_id = *((int*) t_id) ;
	delete ((int*) t_id) ; // free heap memory

	Thread_pool* tpool = Thread_pool::getThreadPool() ;
	TwoWayPipe* tpool_pipe = (tpool->pipe_threads[thread_id]) ;
	TwoWayPipe* tcp_pipe = tpool->p ;

	int fd;
	void* map;
	Thread_input input;
	while(1){
		int read_size = tpool_pipe->threadpoolRead(&input, sizeof(input));
		fd = open(input.filename, O_RDONLY);            // open the file requested for

		Thread_output output ;
		if(fd == -1){
			perror("Error opening file\n");
			output.memory_map = FILE_ERROR ;
			output.num_bytes = strlen(FILE_ERROR)*sizeof(char) ;
		} else { // mmap the file
			map = mmap(0,FILESIZE, PROT_READ, MAP_SHARED,fd,0);      // copy contents to the main memory

			if(map == MAP_FAILED){
				perror("Error mapping memory\n") ;
				output.memory_map = MMAP_ERROR ;
				output.num_bytes = strlen(MMAP_ERROR)*sizeof(char) ;
			} else {
				output.memory_map = map;                         // details to be communicated to the server
				output.num_bytes = getByteSize(input.filename) ;
			}
		}
		
		output.socket_id = input.socket_id ;
		close(fd) ;

		// send output via pipe
		pthread_mutex_lock(&(tpool->server_mutex));
		tcp_pipe->threadpoolWrite(&output,sizeof(output));              // Communicating details to the server.
		printf("writing to server with %i\n", tcp_pipe->getTPWrite() ) ;
		pthread_mutex_unlock(&(tpool->server_mutex));

		// signal ready for next task
		tpool->makeHelperAvailable(thread_id) ;
	}
	return NULL ;
}
/*
  Function that reads inputs from the server, assigns the 
  job to a helper thread. 
*/
void Thread_pool::runThreadpool() {    
	int i ;
	Thread_input input;
	bool job_assigned = true;
	while(1){
		if(job_assigned){
			p->threadpoolRead(&input,sizeof(input));
			job_assigned = false;
		}
		for(i=0;i<thread_no;i++){
			pthread_mutex_lock(&(thread_avail[i]));
			if(avail[i]){
				avail[i] = 0;
				pthread_mutex_unlock(&(thread_avail[i]));
				job_assigned = true;
				break;			
			}
			pthread_mutex_unlock(&(thread_avail[i]));		
		}
		pipe_threads[i]->serverWrite(&input, sizeof(input)) ;   // 'Instructions' sent
	}		
}

/*
  Function that creates all the required helper threads.
*/
void Thread_pool::thread_pool(){
	int i;
	for(i=0;i<thread_no;i++){
		int* thread_id  = new int ;
		*thread_id = i ;
		pthread_create(&thread[i],NULL,helper_thread,(void *)thread_id);    // Thread Creation!!	
	}
}
