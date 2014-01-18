TARGETS=550server

550server: 550server.cpp pipe.hpp tcp_server.hpp 
	g++ -g -o 550server tcp_server.hpp 550server.cpp
#tcp_server: tcp_server.hpp 
#	g++ -g

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
