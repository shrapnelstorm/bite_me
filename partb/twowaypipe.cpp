#include "twowaypipe.hpp"

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

size_t TwoWayPipe::serverRead(void* buf, size_t nbytes) {
	return read(s_read, buf, nbytes) ;
}

size_t TwoWayPipe::serverWrite(const void* buf, size_t nbytes) {
	return write(s_write, buf, nbytes) ;
}

size_t TwoWayPipe::threadpoolRead(void* buf, size_t nbytes) {
	return read(tp_read, buf, nbytes) ;
}

//TODO: add code here to keep trying the write, unless 
// configured not to.
size_t TwoWayPipe::threadpoolWrite(const void* buf, size_t nbytes) {
	return write(tp_write, buf, nbytes) ;
}

