CC=g++
CFLAGS=-std=c++11 -Wall -I../src

all: \
	mkdir_bin \
	tcp_server \
	tcp_client \
	udp_server \
	udp_client

tcp_server:
	$(CC) $(CFLAGS) tcp_server.cpp -o bin/tcp_server 

tcp_client:
	$(CC) $(CFLAGS) tcp_client.cpp -o bin/tcp_client

udp_server:
	$(CC) $(CFLAGS) udp_server.cpp -o bin/udp_server 

udp_client:
	$(CC) $(CFLAGS) udp_client.cpp -o bin/udp_client

mkdir_bin: ; if [ ! -d bin ]; then mkdir bin; fi
clean:     ; if [   -d bin ]; then rm -f bin/*; fi
