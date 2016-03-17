CC=g++
CFLAGS=-std=c++11 -Wall
EXAMPLE_BIN=bin/examples
EXAMPLE_CPPS=main.cpp tcp/*.cpp udp/*.cpp

all: mkdir_bin examples 

examples: $(EXAMPLE_CPPS) 
	$(CC) $(CFLAGS) $(EXAMPLE_CPPS) -o $(EXAMPLE_BIN) 

mkdir_bin: ; if [ ! -d bin ]; then mkdir bin; fi
clean:     ; if [   -d bin ]; then rm -rf bin; fi
