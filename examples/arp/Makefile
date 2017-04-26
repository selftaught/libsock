CC=g++
CFLAGS=-std=c++11 -Wall -I../../src -I.

all: mkdir_bin arpspoof arpping 

arpspoof:
	$(CC) $(CFLAGS) spoof.cpp -o bin/arpspoof

arpping:
	$(CC) $(CFLAGS) ping.cpp -o bin/arpping

mkdir_bin: ; if [ ! -d bin ]; then mkdir bin; fi
clean:     ; if [   -d bin ]; then rm -f bin/*; fi
