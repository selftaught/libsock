CC=g++
CFLAGS=-c -Wall

all: mkdir_bin tcp_client_example tcp_server_example udp_client_example udp_server_example

mkdir_bin:
	if [ ! -d bin ]; then mkdir bin; fi

clean:
	if [ -d bin ]; then rm -rf bin; fi
