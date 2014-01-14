#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>


int main(int argc, char** argv) {
	
	// check for cmd line arguments
	if (argc < 3) {
		printf("usage: ./550server <ip_addr> <port>") ;
	}

	// create listen socket
	int	server_socket = socket(AF_INET, SOCK_STREAM, 0) ;
	if (server_socket < 0) {
		perror("could not create socket") ;
		exit(1) ;
	}

	struct sockaddr_in addr; 	// internet socket address data structure
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[1])); // byte order is significant
	// does this need to be fixed???
	addr.sin_addr.s_addr = INADDR_ANY; // listen to all interfaces

	int yes=1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt failed");
		exit(1);
	}

	return 0 ;
}


