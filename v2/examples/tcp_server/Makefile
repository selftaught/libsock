ROOT_DIR=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

CC=g++
CFLAGS=-I$(ROOT_DIR)/../../src
LDFLAGS=-L$(ROOT_DIR)/../../ -lsock -ldl
SOURCES=tcp_server.cpp

all: clean build clean

clean:
	rm -f *.o

build:
	$(CC) $(CFLAGS) -c $(SOURCES)
	$(CC) $(CFLAGS) -o tcp_server $(SOURCES) ../../libsock.so
	$(CC) $(CFLAGS) $(LDFLAGS) *.o -o tcp_server
