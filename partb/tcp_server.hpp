#ifndef TCP_SERVER
#define TCP_SERVER

#include "main_header.hpp"
#include "twowaypipe.hpp"
#include <map>

// NOTE: the current # of connections to wait on is 32, is this adequate

enum State { WAITING_MMAP, SENDING_FILE, DONE, READY } ;
class ClientData {
	public:
		size_t bytes_sent ;
		size_t total_bytes ;
		void* file_addr ;
		State state ;
	
	public:
		ClientData() { 
			bytes_sent = 0;
			total_bytes = 0 ;
			file_addr = NULL ;
			state = READY ;
		}
};

class TCPServer {
	private:
		struct addrinfo server_info, *server_info_list ;
		int server_socket ;
		
		// socket descriptors we're listening to
		fd_set 	master_read_fds, tmp_read_fds ; 
		fd_set 	master_write_fds, tmp_write_fds ; 

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
		int iterateReadSet(fd_set *ready_set, int ready_count) ;
		int iterateWriteSet(fd_set *write_set, int ready_count) ;
		void addClients() ;
		void removeClient(int client) ;

};




#endif // TCP_SERVER
