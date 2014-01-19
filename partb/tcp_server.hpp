#ifndef TCP_SERVER
#define TCP_SERVER

#include "main_header.hpp"
#include "twowaypipe.hpp"
#include <map>

// TODO: the current # of connections to wait on is 32, is this adequate
// TODO: ensure server is closing unused tcp connections

enum State { LOADING_FILE, SENDING_FILE, DONE, READY } ;
class ClientData {
	public:
		size_t bytes_sent ;
		size_t total_file_size ;
		void* file_addr ;
		State state ;
	
	public:
		ClientData() { 
			bytes_sent = 0;
			total_file_size = 0 ;
			file_addr = NULL ;
			state = READY ;
		}
};

class TCPServer {
	private:
		struct addrinfo server_info, *server_info_list ;
		int server_socket ;
		fd_set 	master_fds, working_fds ; // socket descriptors we're listening to
		TwoWayPipe* tcp_pipe ;
		int  max_sd, new_fd ; // max_fd stores highest valued fd

		// socket_id --> client state
		std::map<int, ClientData> client_map ;

	public:
		TCPServer(char address[], char port[], TwoWayPipe* pipe) ;
		~TCPServer() ;
		void runServer() ;

	private:
		void initServer(char address[], char port[]) ;
		void bindSocketAndListen() ;
		void iterateFDSet(fd_set *ready_set, int &ready_count) ;
		void addClients() ;
		void removeClient(int client) ;

		ClientData sentToClient(ClientData data) ;
};




#endif // TCP_SERVER
