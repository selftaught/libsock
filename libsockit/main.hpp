
#ifndef main_h
#define main_h

/**
 * Cross platform header files
 *  <cstdint>   - Is used for int typedefs.
 *  <exception> - Is used for creating our custom socket exception classes.
 *  <algorithm> - Is used for std::remove
 *  <cstdarg>   - Is used for va_list
 */
#include <iostream>
#include <exception>
#include <algorithm>
#include <cstdint>
#include <cstdarg>

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
#include "socket.hpp"
#include "udp.hpp"
#include "tcp.hpp"

#endif
