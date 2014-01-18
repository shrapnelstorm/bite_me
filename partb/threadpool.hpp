/* This file contains code to run the threadpool of an AMTED server
 *
 *
 */

class Threadpool {
	private:
		const int MAX_THREADS = 30 ; // max pthreads to be dispatched
		TwoWayPipe* pipe ;

		// insert some synchronization mechanisms here
	public:
		Threadpool(TwoWayPipe* p) { pipe = p ; }
		~Threadpool() ;

		void threadPoolMain();

	private:
		void threadLoadFile(file_name) ;


} ;

TwoWayPipe::~TwoWayPipe() {
	// close all open pthreads, don't close the pipe
}
