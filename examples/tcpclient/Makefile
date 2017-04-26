CC=g++
CFLAGS=-std=c++11 -Wall -I../../src -I.

all: mkdir_bin httpreq

httpreq:
	$(CC) $(CFLAGS) httpreq.cpp -o bin/httpreq

mkdir_bin: ; if [ ! -d bin ]; then mkdir bin; fi
clean:     ; if [   -d bin ]; then rm -f bin/*; fi
