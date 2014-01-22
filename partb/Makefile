TARGETS=550server 550server.o tcp_server.o main_header.o threadpool.o

550server: 550server.o tcp_server.o main_header.o threadpool.o twowaypipe.o
	g++ -std=c++11 -pthread -g -o 550server main_header.o tcp_server.o threadpool.o twowaypipe.o 550server.o
tcp_server.o: tcp_server.hpp tcp_server.cpp main_header.hpp
	g++ -std=c++11 -g -c tcp_server.cpp
main_header.o: main_header.hpp main_header.cpp
	g++ -std=c++11 -g -c main_header.cpp
550server.o: main_header.hpp 550server.cpp
	g++ -std=c++11 -g -c 550server.cpp
# c++11 flag toinitalize arrays in declarations
threadpool.o: threadpool.hpp threadpool.cpp
	g++ -std=c++11  -g -c threadpool.cpp
twowaypipe.o: twowaypipe.hpp twowaypipe.cpp
	g++ -std=c++11 -g -c twowaypipe.cpp

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
