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

// XXX: figure out weird default argument error
void exitWithError(bool condition, const char msg[], int status=1) {
	if (condition) {
		perror(msg) ;
		exit(status) ;
	}
}
#endif // MAIN_HEADER