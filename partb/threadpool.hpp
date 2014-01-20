// XXX: Ensure TWoWayPipe copies are passed as pointers !!!

/* This file contains code to run the threadpool of an AMTED server
 *
 *
 */

#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP
#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include  "twowaypipe.hpp"
#include "main_header.hpp"

#define FILESIZE (1000 * sizeof(char)) // how to fix this?
#define STD_IN 0
#define STD_OUT 1
#define thread_no 50


// this class is a singleton
class Thread_pool{
	private:
		TwoWayPipe p; // communicate with server
		TwoWayPipe pipe_threads[thread_no]; // helper thread communication

		// pthreads 
		pthread_t thread[thread_no];
		int avail[thread_no];
		pthread_mutex_t thread_avail[thread_no] = {PTHREAD_MUTEX_INITIALIZER} ;
		pthread_mutex_t server_mutex = PTHREAD_MUTEX_INITIALIZER ;


		static Thread_pool* tp_instance ;
	public:
		void runThreadpool() ;
		static Thread_pool* getThreadPool() ;
		
		// TODO: be sure this code is called, before we run!!!
		void setTCPPipe(TwoWayPipe p) { this->p = p ; } 
	private:
		Thread_pool(); // thread pool will be a singleton
		static void* helper_thread(void *pipe_rw);
		void thread_pool();

		// functions for the helper threads to use
		void makeHelperAvailable(int helper_id) ;
		
};


#endif // THREADPOOL_HPP
