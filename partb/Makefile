TARGETS=550server 550server.o tcp_server.o main_header.o threadpool.o

550server: 550server.o tcp_server.o main_header.o threadpool.o twowaypipe.o
	g++ -pthread -g -o 550server main_header.o tcp_server.o threadpool.o twowaypipe.o 550server.o
tcp_server.o: tcp_server.hpp tcp_server.cpp main_header.hpp
	g++ -Wall -g -c tcp_server.cpp
main_header.o: main_header.hpp main_header.cpp
	g++ -Wall -g -c main_header.cpp
550server.o: main_header.hpp 550server.cpp
	g++ -Wall -g -c 550server.cpp
# c++11 flag toinitalize arrays in declarations
threadpool.o: threadpool.hpp threadpool.cpp
	g++ -std=c++11  -Wall -g -c threadpool.cpp
twowaypipe.o: twowaypipe.hpp twowaypipe.cpp
	g++ -Wall -g -c twowaypipe.cpp

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
