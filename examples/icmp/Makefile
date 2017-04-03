CC=g++
CFLAGS=-std=c++11 -Wall -I../../src

all: mkdir_bin webserver

webserver:
	$(CC) $(CFLAGS) -I. webserver.cpp -o bin/webserver

mkdir_bin: ; if [ ! -d bin ]; then mkdir bin; fi
clean:     ; if [   -d bin ]; then rm -f bin/*; fi
