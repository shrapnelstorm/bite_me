TARGETS=550server 550server.o tcp_server.o main_header.o

550server: 550server.o pipe.hpp tcp_server.o main_header.o
	g++ -g -o 550server main_header.o tcp_server.o 550server.o
tcp_server.o: tcp_server.hpp tcp_server.cpp main_header.hpp
	g++ -Wall -g -c tcp_server.cpp
main_header.o: main_header.hpp main_header.cpp
	g++ -Wall -g -c main_header.cpp
550server.o: main_header.hpp 550server.cpp
	g++ -Wall -g -c 550server.cpp

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
