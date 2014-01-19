#ifndef MAIN_HEADER
#define MAIN_HEADER

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h> // for the ioctl() function call
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <sys/stat.h> // for file info

const int MAX_FILENAME = 100 ;
//const char* FILE_ERROR = "could not find file\n" ; 

// XXX: figure out weird default argument error
void exitWithError(bool , const char [], int ) ;
void exitWithError(bool , const char []) ;
off_t getByteSize(char* filename) ;

struct Thread_input{
	int socket_id;
	char filename[MAX_FILENAME];
};

struct Thread_output{
	void* memory_map;
	int socket_id;
	int fd;
	size_t num_bytes ;
};

struct thread_pool_input{
	int pipe_write;
	int socket_id;
	
};

#endif // MAIN_HEADER
