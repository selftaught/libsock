
#ifndef __sockit_h
#define __sockit_h

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
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#elif defined(__APPLE__) || defined(__linux__)

#define __NIX 1

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#endif

/**
 * Default recv char array lengths
 * for the UDP and TCP protocols.
 */
#define UDP_RECV_BUF_LEN 576
#define TCP_RECV_BUF_LEN 1500

/**
 * Buffer length for char errbuf[] in SocketException->init.
 * This length defines the max length of an error message 
 * that can be thrown by SockException.
 */
#define ERR_BUF_LEN 256

/**
 *
 */
#define DEFAULT_TV_SEC  5
#define DEFAULT_TV_USEC 0

/**
 * Not really neccessary because IPPROTO_TCP and IPPROTO_UDP
 * are both 0 but, meh. This function macro gets used in 
 * SocketBase::connect_client and SocketBase::connect_server.
 */
#define IPPROTO(TYPE) ((TYPE) == (SOCK_STREAM) ? (IPPROTO_TCP) : (IPPROTO_UDP))

enum SERVICE_TYPE {
    UNDEF,
    CLIENT,
    SERVER
};

enum IP {
    V4,
    V6
};

/**
 * Define the default socket value. This is done because without
 * it, we'd have to add preprocessor conditions to each constructor
 * of SocketBase checking and defining m_socket based on the current
 * system. However, by define DEFAULT_SOCKET_VAL, we don't need to do that.
 * We can simply define m_socket like m_socket(DEFAULT_SOCKET_VAL) instead.
 */
#if defined (__NIX)
	#define DEFAULT_SOCKET_VAL -1
#else
	#define DEFAULT_SOCKET_VAL INVALID_SOCKET
	#define WIN32_LEAN_AND_MEAN
#endif

/**
 * SocketException class
 *
 * @credits: http://stackoverflow.com/questions/8152720/correct-way-to-inherit-from-stdexception
 */
class SocketException : public std::exception {
public:
    /** 
     * Constructor (C strings).
     * @param message C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the char* lies
     *                 with the caller.
     */
    explicit SocketException(const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        this->init(fmt, ap);
        va_end(ap);
    }
    
    /** 
     * Constructor (C++ STL strings).
     * @param message The error message.
     */
    explicit SocketException(const std::string& message):
        msg_(message)
    {}
    
    /** 
     * Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~SocketException() throw() {}
    
    /** 
     * Returns a pointer to the (constant) error description.
     * @return A pointer to a const char*. The underlying memory
     *          is in posession of the Exception object. Callers must
     *          not attempt to free the memory.
     */
    virtual const char* what() throw() {
        return msg_.c_str();
    }
    
protected:
    /**
     * Error message.
     */
    std::string msg_;
    
    /**
     *
     */
    void init(const char* fmt, va_list ap) {
        char errbuf[256];
        vsnprintf(errbuf, sizeof(errbuf), fmt, ap);
        msg_ = errbuf;
    }
};

/**
 * Base socket class.
 *
 * This is the class which UdpSocket and TcpSocket are derived from.
 *
 */
class SocketBase {
protected:
    
    /**
     * IP version.
     */
    IP m_ip_version;
    
    /**
     * Hostname to connect to.
     */
    std::string m_hostname;
    
    /**
     * Port number to connect on.
     */
    uint16_t m_port;
    
    /**
     *
     */
    uint16_t m_buf_size;
    
    /**
     * Structure where connection info will be stored.
     */
    struct sockaddr_in m_sockaddr;
    
    /**
     * Address family.
     */
    int m_af;
    
    /**
     * Type (SOCK_STREAM or SOCK_DGRAM).
     */
    int m_type;

    /**
     * Maximum backlog size.
     */
    int m_backlog;
    
    /**
     * Protocol (IPPROTO_TCP or IPPROTO_UDP).
     */
    int m_protocol;
    
    /**
     * Service type (SYSTEM_TYPE::CLIENT or SYSTEM_TYPE::SERVER)
     */
    SERVICE_TYPE m_service_type;
    
    /**
     * File descriptor sets used for the
     * non-blocking implementation.
     */
    fd_set m_active_fd_set;
    fd_set m_read_fd_set;
    
    struct timeval m_time;
    
    /**
     * If the current system is *nix or apple
     */
#if defined(__NIX)
    
    /**
     * Socket file descriptors. 
     * m_socket_tcp is only for the TCP server socket.
     */
    int m_socket;
    int m_socket_tcp;

    struct hostent* m_host;

    /**
     * Else the system is windows.
     */
#else

    SOCKET  m_socket;
	SOCKET  m_socket_client;
    WSADATA m_wsa;

	struct addrinfo *m_result;
	struct addrinfo *m_ptr;
	struct addrinfo  m_hints;

#endif
    
    void connect_server();
    void connect_client();
    
public:
    
    /**
     * Constructors.
     */
    SocketBase(const int& type, uint16_t buf_size):
		m_socket(DEFAULT_SOCKET_VAL),
        m_port(0),
        m_af(AF_INET),
        m_type(type),
        m_backlog(5),
        m_buf_size(buf_size),
        m_service_type(SERVER) {
            m_time.tv_sec  = DEFAULT_TV_SEC;
            m_time.tv_usec = DEFAULT_TV_USEC;
        }
    
    SocketBase(const uint16_t& port, const int& type, uint16_t buf_size):
        m_socket(DEFAULT_SOCKET_VAL),
		m_port(port),
        m_af(AF_INET),
        m_type(type),
        m_backlog(5),
        m_buf_size(buf_size),
        m_service_type(SERVER) {
            m_time.tv_sec  = DEFAULT_TV_SEC;
            m_time.tv_usec = DEFAULT_TV_USEC;
        }
    
    SocketBase(const std::string& hostname, const uint16_t& port, const int& type, uint16_t buf_size):
		m_socket(DEFAULT_SOCKET_VAL),
        m_hostname(hostname),
        m_port(port),
        m_af(AF_INET),
        m_type(type),
        m_backlog(5),
        m_buf_size(buf_size),
        m_service_type(CLIENT) {
            m_time.tv_sec  = DEFAULT_TV_SEC;
            m_time.tv_usec = DEFAULT_TV_USEC;
        }
    
    SocketBase(const std::string&, const int&, uint16_t);
    SocketBase(const std::string&, const std::string&, const int&, uint16_t);
    
    /**
     * Destructor.
     */
    ~SocketBase();
    
    /**
     * Prototypes.
     */
    void disconnect();
    void connect();
    void setnonblocking(int);
    void setblocking();
    
    virtual void send(const std::string&) = 0;
    virtual std::string receive() = 0;
    
    /**
     * Setters
     */
    void set_hostname(const std::string& hostname) {
        m_hostname = hostname;
    }
    
    void set_port(const uint16_t& port) {
        m_port = port;
    }
    
    void set_port(const std::string& port) {
        m_port = atoi(port.c_str());
    }
    
    void set_address_family(const int& address_family) {
        m_af = address_family;
    }
    
    void set_type(const int& type) {
        m_type = type;
    }
    
    void set_backlog(const uint32_t& backlog) {
        m_backlog = backlog;
    }
    
    void set_ipv4() {
        m_ip_version = V4;
    }
    
    void set_ipv6() {
        m_ip_version =V6;
    }
    
    /**
     * Explicitly declare the service type (CLIENT or SERVER)
     * despite the fact the service type get's intelligently
     * set in the constructor.
     */
    void set_service_type(const SERVICE_TYPE& type) {
        m_service_type = type;
    }
};

/**
 * TCP (stream) socket class.
 */
class TcpSocket : public SocketBase {
public:
    /**
     * Constructors
     *
     * Default m_recv_buf_size is because the MTU (Max Transmission Unit)
     * for ethernet is TCP_RECV_BUF_LEN bytes.
     */
    TcpSocket(const uint16_t& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
    SocketBase(port, SOCK_STREAM, buf_size)
    { }
    
    TcpSocket(const std::string& hostname, const uint16_t& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
    SocketBase(hostname, port, SOCK_STREAM, buf_size)
    { }
    
    TcpSocket(const std::string& port):
    SocketBase(port, SOCK_STREAM, TCP_RECV_BUF_LEN)
    { }
    
    TcpSocket(const std::string& hostname, const std::string& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
    SocketBase(hostname, port, SOCK_STREAM, buf_size)
    { }
    
    void set_buf_size(const uint16_t& size) {
        m_buf_size = size;
    }
    
    void send(const std::string&);
    
    std::string receive();
};

/**
 * UDP (datagram) socket class.
 */
class UdpSocket : public SocketBase {
public:
    /**
     * Constructors
     */
    UdpSocket(const uint16_t& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
    SocketBase(port, SOCK_DGRAM, buf_size)
    { }
    
    UdpSocket(const std::string& hostname, const uint16_t& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
    SocketBase(hostname, port, SOCK_DGRAM, buf_size)
    { }
    
    UdpSocket(const std::string& port):
    SocketBase(port, SOCK_DGRAM, UDP_RECV_BUF_LEN)
    { }
    
    UdpSocket(const std::string& hostname, const std::string& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
    SocketBase(hostname, port, SOCK_DGRAM, buf_size)
    { }
    
    void send(const std::string&);
    
    std::string receive();
};

#endif
