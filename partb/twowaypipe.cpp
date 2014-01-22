/************************************************************
 * Desc: This implements a two way pipe.
 *************************************************************/

#include "twowaypipe.hpp"

/*
  Constructor: Creates 2 pipes
*/
TwoWayPipe::TwoWayPipe() {
	setupSinglePipe(s_read, tp_write) ;
	setupSinglePipe(tp_read, s_write) ;
}

/*
 Destructor: Closes the pipes.
*/
TwoWayPipe::~TwoWayPipe() {
	close(s_read);
	close(s_write);
	close(tp_read);
	close(tp_write);
}

/*
 Function that creates a single pipe
*/
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

/*
  Function that performs a read on the server end
*/
size_t TwoWayPipe::serverRead(void* buf, size_t nbytes) {
	return read(s_read, buf, nbytes) ;
}

/*
  Function that performs a write on the server end
*/
size_t TwoWayPipe::serverWrite(const void* buf, size_t nbytes) {
	return write(s_write, buf, nbytes) ;
}

/*
  Function that performs a read on the threadpool end
*/
size_t TwoWayPipe::threadpoolRead(void* buf, size_t nbytes) {
	return read(tp_read, buf, nbytes) ;
}

/*
  Function that performs a read on the threadpool end
*/
size_t TwoWayPipe::threadpoolWrite(const void* buf, size_t nbytes) {
	return write(tp_write, buf, nbytes) ;
}

