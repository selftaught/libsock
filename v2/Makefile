ROOT_DIR=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

CC=gcc
CFLAGS=-I${ROOT_DIR}/src -c -v
LDFLAGS=

LIB_DIR=${ROOT_DIR}/lib
OBJ_DIR=${ROOT_DIR}/obj

SOURCES=base raw tcp udp
SOURCE_DIR=${ROOT_DIR}/src

# SO = Shared Object = .so
SO_NAME=libsock.so
SO_LDFLAGS=
SO_CFLAGS=\
		   -Wno-unused-variable    \
		   -Wno-unused-function    \
		   -shared                 \
		   -g                      \
		   -Wl -v


all: print_make_vars clean setup $(SOURCES) build

print_make_vars:
	@echo "CC: ${CC}"
	@echo "CFLAGS: ${CFLAGS}"
	@echo "LDFLAGS: ${LDFLAGS}"
	@echo "LIB_DIR: ${LIB_DIR}"
	@echo "SOURCE_DIR: ${SOURCE_DIR}"

setup:
	@if [[ ! -d ${LIB_DIR} ]]; then mkdir -p ${LIB_DIR}; fi
	@if [[ ! -d ${OBJ_DIR} ]]; then mkdir -p ${OBJ_DIR}; fi

clean:
	@rm -rf ${OBJ_DIR} ${LIB_DIR}

$(SOURCES):
	@echo "\033[92mCompiling object file from\033[0m: ${SOURCE_DIR}/${@}.cpp"
	${CC} ${CFLAGS} ${SOURCE_DIR}/$@.cpp -o ${OBJ_DIR}/$@.o

build:
	@echo "\033[93mBuilding\033[0m..."
	$(CC) $(SO_CFLAGS) $(SO_LDFLAGS) ${OBJ_DIR}/*.o -o ${ROOT_DIR}/${SO_NAME}


