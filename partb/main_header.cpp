
#include "main_header.hpp"

void exitWithError(bool condition, const char msg[], int status) {
	if (condition) {
		perror(msg) ;
		exit(status) ;
	}
}

void exitWithError(bool condition, const char msg[]) {
	exitWithError(condition, msg, 1);
}
