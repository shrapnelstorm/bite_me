#ifndef TWOWAYPIPE_HPP
#define TWOWAYPIPE_HPP
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
// TODO: worry about blocking / non blocking read/write
// creates a two way pipe with nonblocking reads and writes
class TwoWayPipe {
	
	private:
	// write and read fds for the server (s) and threadpool (tp)
	int s_read, s_write, tp_read, tp_write ;

	public:
		TwoWayPipe() ;
		~TwoWayPipe() ;
		size_t serverRead(void* buf, size_t nbytes) ;
		size_t serverWrite(const void* buf, size_t nbytes) ;

		size_t threadpoolRead(void* buf, size_t nbytes) ;
		size_t threadpoolWrite(const void* buf, size_t nbytes) ;

		// accessor functions 
		int getSRead()		{ return s_read ; }
		int getSWrite()		{ return s_write ; }
		int getTPRead() 	{ return tp_read ; }
		int getTPWrite() 	{ return tp_write ; }

		static int setupSinglePipe(int &read, int &write) ;
};


#endif //TWOWAYPIPE_HPP
