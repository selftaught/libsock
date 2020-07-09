#ifndef __globals_hpp
#define __globals_hpp

#if defined(__cplusplus)
#    if __cplusplus >= 201103L
#        define CPP11 1
#    else
#        define CPP11 0
#    endif
#else
#    error("C++ compiler is required...")
#endif

// https://msdn.microsoft.com/en-us/library/b0084kay.aspx
#if defined(_WIN32) || defined(_WIN64)
#	define PREDEF_PLATFORM_WINDOWS 1
#elif defined(__APPLE__) || defined(__linux__)
#	define PREDEF_PLATFORM_LINUX 1
#endif

/**
 * Define the default socket value. This is done because without
 * it, we'd have to add preprocessor conditions to each constructor
 * of Socket checking and defining m_socket based on the current
 * system. However, by define DEFAULT_SOCKET_VAL, we don't need to do that.
 * We can simply define m_socket like m_socket(DEFAULT_SOCKET_VAL) instead.
 */
#if defined (PREDEF_PLATFORM_LINUX)
#    define DEFAULT_SOCKET_VAL -1
#elif defined(PREDEF_PLATFORM_WINDOWS)
#    define DEFAULT_SOCKET_VAL INVALID_SOCKET
#endif

// Debug toggle
#define DEBUG 1
#define DEFAULT_BACK_LOG_SIZE 1
#define DEFAULT_RECV_BUF_SIZE 512
#define DEFAULT_RECV_TIMEOUT  10

#endif