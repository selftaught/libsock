
#ifndef main_h
#define main_h

/**
 * Cross platform header files
 *  <cstdint>   - Is used for int typedefs.
 *  <exception> - Is used for creating our custom socket exception classes.
 *  <algorithm> - Is used for std::remove
 *  <cstdarg>   - Is used for va_list in SocketException
 *  <string>    - Is used so VS doesn't bitch when writing a simple line like 'std::cout << string << std::endl;'
 *  <memory>    - Is used for smart pointers.
 */
#include <iostream>
#include <exception>
#include <algorithm>
#include <cstdint>
#include <cstdarg>
#include <string>
#include <memory>

/**
 * Make sure __cplusplus is defined because it's value will tell us what version of 
 * C++ the compiler supports. It's value should be <= 199711L in pre-C++11 compilers.
 * A different solution may need to be implemented to detect whether or not the compiler
 * supports C++11. This isn't that reliable of a solution.
 *
 * @source: http://stackoverflow.com/questions/10717502/is-there-a-preprocessor-directive-for-detecting-c11x-support
 */
#if defined(__cplusplus)
	/**
	 * If the 
	 */
	#if __cplusplus > 199711L
		#define CPP11 1
	#else
		#define CPP11 0
	#endif
#else
	#error A C++ compiler is required...
#endif

#if defined(_WIN32) || defined(_WIN64)

    #define __WIN 1

	#include <WinSock2.h>

	#pragma comment(lib, "ws2_32.lib")

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
