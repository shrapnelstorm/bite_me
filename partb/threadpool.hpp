/************************************************************
 * Program: Threadpool class declaration
 *
 * Desc: 	Specification for the threadpool of an AMTED server.
 * 			We create thread_no pthreads which run a helper function
 * 			to load mmap()ed files. A separate pipe exists between
 * 			each helper thread and the main threadpool.
 * 			Helper threads do a blocking read on the pipe
 * 			to avoid busy waiting.
 *************************************************************/
#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP
#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include  "twowaypipe.hpp"
#include "main_header.hpp"

#define FILESIZE (1000 * sizeof(char)) // TODO: how to fix this?
#define STD_IN 0
#define STD_OUT 1
#define thread_no 10


// this class is a singleton
class Thread_pool{               // The threadpool class. Main function: Create helper threads, dispatch jobs to the helper threads.
	private:
		TwoWayPipe* p; // communicate with server
		TwoWayPipe* pipe_threads[thread_no]; // helper thread communication

		// pthreads 
		pthread_t thread[thread_no];
		int avail[thread_no];
		pthread_mutex_t thread_avail[thread_no] = {PTHREAD_MUTEX_INITIALIZER} ;  // help in concurrency. 'Locks' to check thread availability.
		pthread_mutex_t server_mutex = PTHREAD_MUTEX_INITIALIZER ;


		static Thread_pool* tp_instance ;
	public:
		~Thread_pool() ;
		void runThreadpool() ;
		static Thread_pool* getThreadPool() ;
		
		// TODO: be sure this code is called, before we run!!!
		void setTCPPipe(TwoWayPipe* p) { this->p = p ; } 
	private:
		Thread_pool(); // thread pool will be a singleton
		static void* helper_thread(void *pipe_rw);
		void thread_pool();

		// functions for the helper threads to use
		void makeHelperAvailable(int helper_id) ;
		
};


#endif // THREADPOOL_HPP
