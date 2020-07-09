MF_ABS_PATH := $(shell pwd)/$(shell echo ${MAKEFILE_LIST} | tr -d ' ' | sed -e 's/Makefile//g')
CC=g++
CFLAGS=-std=c++11 -Wall -I${MF_ABS_PATH}/../../src

all: mkdir_bin webserver

webserver:
	$(CC) $(CFLAGS) -I. main.cpp webserver.cpp -o bin/webserver

mkdir_bin: ; if [ ! -d bin ]; then mkdir bin; fi
clean:     ; if [   -d bin ]; then rm -f bin/*; fi
