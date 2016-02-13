
#ifndef main_h
#define main_h

#include <iostream>
#include <exception>
#include <cerrno>

/**
 * Cross platform header files
 *  <cstdint> - Is used for int typedefs.
 */
#include <cstdint>

#if defined(__WIN32) || defined(__WIN64)

    #define __WIN 1

    #include <WinSock2.h>

#elif defined(__APPLE__) || defined(__linux__)

    #define __NIX 1

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <unistd.h>
#endif

/**
 * Project headers
 */
#include "socket.h"
#include "udp.h"
#include "tcp.h"

#endif
