#ifndef __sock_defs_hpp
#define __sock_defs_hpp

#include <stdio.h>

#define POLL_EXPIRE      0
#define UDP_RECV_BUF_LEN 576
#define TCP_RECV_BUF_LEN 1500

/**
 * Make sure __cplusplus is defined because it's value will tell us what version of
 * C++ the compiler supports. It's value should be <= 199711L in pre-C++11 compilers.
 * A different solution may need to be implemented to detect whether or not the compiler
 * supports C++11. This isn't that reliable of a solution.
 *
 * @source: http://stackoverflow.com/questions/10717502/is-there-a-preprocessor-directive-for-detecting-c11x-support
 *
 */
#if defined(__cplusplus)
#    if __cplusplus >= 201103L
#        define CPP11 1
#    else
#        define CPP11 0
#    endif
#else
#    error("C++ compiler is required...")
#endif

#if defined(_WIN32) || defined(_WIN64)
#    define PREDEF_PLATFORM_WINDOWS 1
#elif defined(__APPLE__) || defined(__linux__)
#    define PREDEF_PLATFORM_LINUX 1
#endif

/**
 * Define the default socket value. This is done because without
 * it, we'd have to add preprocessor conditions to each constructor
 * of SockBase checking and defining m_socket based on the current
 * system. However, by define DEFAULT_SOCKET_VAL, we don't need to do that.
 * We can simply define m_socket like m_socket(DEFAULT_SOCKET_VAL) instead.
 *
 */
#if defined (PREDEF_PLATFORM_LINUX)
#    define DEFAULT_SOCKET_VAL -1
#elif defined(PREDEF_PLATFORM_WINDOWS)
#    define DEFAULT_SOCKET_VAL INVALID_SOCKET
#endif

/**
 * Debugging preprocessors.
 */
#define __DEBUGGING 1
#ifdef  __DEBUGGING
#    define DEBUG_STDERR(...) \
        (fprintf(stderr, "[ " << __FILE__ << " ] " \
                   << "(func " << __FUNCTION__ << "): " \
                   << "(line " << __LINE__ << "): " \
                   << __VA_ARGS__ \
                   << std::endl \
        )
#    define DEBUG_STDOUT(...) \
        (std::cout << "[ " << __FILE__ << " ] " \
                   << "(func " << __FUNCTION__ << "): " \
                   << "(line " << __LINE__ << "): " \
                   << __VA_ARGS__ \
                   << std::endl \
        )
#else
#    define DEBUG_STDERR(x)
#    define DEBUG_STDOUT(x)
#endif

#endif
