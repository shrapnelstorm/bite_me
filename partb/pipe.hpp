#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>

// creates a two way pipe with nonblocking reads and writes
class TwoWayPipe {
	
	private:
	// write and read fds for the server (s) and threadpool (tp)
	int s_read, s_write, tp_read, tp_write ;

	public:
		TwoWayPipe() ;
		~TwoWayPipe() ;
		void serverRead() ;
		void serverWrite() ;

		void threadpoolRead() ;
		void threadpoolWrite() ;

		// accessor functions 
		int getSRead()		{ return s_read ; }
		int getSWrite()		{ return s_write ; }
		int getTPRead() 	{ return tp_read ; }
		int getTPWrite() 	{ return tp_write ; }

		static int setupSinglePipe(int &read, int &write) ;
};

TwoWayPipe::TwoWayPipe() {
	setupSinglePipe(s_read, tp_write) ;
	setupSinglePipe(tp_read, s_write) ;
}

// just close the pipes
TwoWayPipe::~TwoWayPipe() {
	close(s_read);
	close(s_write);
	close(tp_read);
	close(tp_write);
}

int TwoWayPipe::setupSinglePipe(int &read, int &write) {
  int pip[2];
  int result;
  result = pipe(pip);
  fflush(stdout) ;
  if(result == -1){
    perror("Unable to create a pipe");
    exit(1);
  }
  else {
    write	= pip[1];
    read	= pip[0];
  }
  return 1;
}

void TwoWayPipe::serverRead() {
}

void TwoWayPipe::serverWrite() {
}

void TwoWayPipe::threadpoolRead() {
}

void TwoWayPipe::threadpoolWrite() {
}
