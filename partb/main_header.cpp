
#include "main_header.hpp"
#include <sys/stat.h>

void exitWithError(bool condition, const char msg[], int status) {
	if (condition) {
		perror(msg) ;
		exit(status) ;
	}
}

void exitWithError(bool condition, const char msg[]) {
	exitWithError(condition, msg, 1);
}

off_t getByteSize(char* file) {
	struct stat info ;
	stat(file, &info) ;

	return info.st_size ;
}
