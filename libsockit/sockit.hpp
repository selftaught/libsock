
#ifndef __sockit_h
#define __sockit_h

#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

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
#include <poll.h>

#endif

/**
 *
 */
#define POLL_EXPIRE (0)

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
 * Not really neccessary because IPPROTO_TCP and IPPROTO_UDP
 * are both 0 but, meh. This function macro gets used in 
 * Socket::connect_client and Socket::connect_server.
 */
#define IPPROTO(TYPE) ((TYPE) == (SOCK_STREAM) ? (IPPROTO_TCP) : (IPPROTO_UDP))

/**
 * Enums.
 */
enum SERVICE_TYPE {
    UNDEF,
    CLIENT,
    SERVER
};

enum SOCKET_TYPE {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM,
    RAW = SOCK_RAW,
    SEQ = SOCK_SEQPACKET,
    RDM = SOCK_RDM
};

enum IP {
    V4,
    V6
};

/**
 * Define the default socket value. This is done because without
 * it, we'd have to add preprocessor conditions to each constructor
 * of Socket checking and defining m_socket based on the current
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
template<SOCKET_TYPE socket_t, SERVICE_TYPE service_t>
class Socket {
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
     * Recv buffer size.
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
     * See the SOCKET_TYPE enum above for options.
     */
    SOCKET_TYPE m_type;

    /**
     * Maximum backlog size.
     */
    int m_backlog;
    
    /**
     * Service type (SYSTEM_TYPE::CLIENT or SYSTEM_TYPE::SERVER)
     */
    SERVICE_TYPE m_service_type;
    
    /**
     *
     */
    struct pollfd m_pfd[1];
    
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
    Socket(const uint16_t& port):
		m_socket(DEFAULT_SOCKET_VAL),
        m_port(port),
        m_af(AF_INET),
        m_type(socket_t),
        m_backlog(5),
        m_service_type(service_t)
    { }

    Socket(const std::string& hostname, const uint16_t& port):
		m_socket(DEFAULT_SOCKET_VAL),
        m_hostname(hostname),
        m_port(port),
        m_af(AF_INET),
        m_type(socket_t),
        m_backlog(5),
        m_service_type(service_t)
    { }
    
    Socket(const std::string&, uint16_t);
    Socket(const std::string&, const std::string&, uint16_t);
    
    /**
     * Destructor.
     */
    ~Socket();
    
    /**
     * Prototypes.
     */
    
    void init();
    void disconnect();
    void connect();
    void send(const std::string&);
    
    bool ready(const uint32_t&);
    bool set_blocking(int, bool blocking = false);
    
    std::string receive();
    
    /**
     * Setters
     */
    void set_hostname      (const std::string& hostname) { m_hostname   = hostname;           }
    void set_port          (const uint16_t   & port    ) { m_port       = port;               }
    void set_port          (const std::string& port    ) { m_port       = atoi(port.c_str()); }
    void set_address_family(const int        & addrf   ) { m_af         = addrf;              }
    void set_backlog       (const uint32_t   & backlog ) { m_backlog    = backlog;            }
    void set_ipv4          (                           ) { m_ip_version = V4;                 }
    void set_ipv6          (                           ) { m_ip_version = V6;                 }
    
    /**
     *
     */
    pollfd* pfd() {
        return m_pfd;
    }
};

template<SOCKET_TYPE socket_t, SERVICE_TYPE service_t>
Socket<socket_t, service_t>::Socket(const std::string& port, uint16_t buf_size) {
    m_socket	   = DEFAULT_SOCKET_VAL;
    m_port         = atoi(port.c_str());
    m_af           = AF_INET;
    m_type         = socket_t;
    m_backlog      = 5;
    m_buf_size     = buf_size;
    m_service_type = service_t;
}

template<SOCKET_TYPE socket_t, SERVICE_TYPE service_t>
Socket<socket_t, service_t>::Socket(const std::string& hostname, const std::string& port, uint16_t buf_size) {
    m_socket	   = DEFAULT_SOCKET_VAL;
    m_hostname     = hostname;
    m_port         = atoi(port.c_str());
    m_af           = AF_INET;
    m_type         = socket_t;
    m_backlog      = 5;
    m_buf_size     = buf_size;
    m_service_type = service_t;
}

template<SOCKET_TYPE socket_t, SERVICE_TYPE service_t>
Socket<socket_t, service_t>::~Socket() {
#if defined(__NIX)
    
#else
    
#endif
    disconnect();
}

/**
 * Calls the correct connect function (server or client)
 * based on the value of m_service_type;
 */
template<SOCKET_TYPE socket_t, SERVICE_TYPE service_t>
void Socket<socket_t, service_t>::connect() {

    /**
     * TCP specific declarations
     */
    if(m_type == TCP) {
        
        LOG(INFO) << "setting TCP recv buffer len to " << TCP_RECV_BUF_LEN;
        
        m_buf_size = TCP_RECV_BUF_LEN;
    }
    /**
     * UDP specific declarations
     */
    else if(m_type == UDP) {
        
        LOG(INFO) << "setting UDP recv buffer len to " << UDP_RECV_BUF_LEN;
        
        m_buf_size = UDP_RECV_BUF_LEN;
    }
    
    if(m_service_type == UNDEF) {
        
        LOG(FATAL) << "service_type_not_specified";
        
        throw SocketException("service_type_not_specified");
    }
    
         if(m_service_type == SERVER) { connect_server(); }
    else if(m_service_type == CLIENT) { connect_client(); }
    else {
        
        LOG(FATAL) << "invalid_service_type";
        
        throw SocketException("invalid_service_type");
    }
    
    m_pfd[ 0 ].fd     = m_socket;
    m_pfd[ 0 ].events = POLLIN;
}

/**
 * Closes the socket if it's open.
 */
template<SOCKET_TYPE socket_t, SERVICE_TYPE service_t>
void Socket<socket_t, service_t>::disconnect() {
    
    LOG(INFO) << "disconnecting";
    
#if defined(__NIX)
    if(m_socket != -1) {
        close(m_socket);
    }
#else
    if (m_result != NULL) {
        freeaddrinfo(m_result);
    }
    
    WSACleanup();
#endif
}

/**
 * Does the needful to setup a server socket.
 */
template<SOCKET_TYPE socket_t, SERVICE_TYPE service_t>
void Socket<socket_t, service_t>::connect_server() {
    
    /**
     * Throw an exception if the port hasn't be defined by the user.
     */
    if(!m_port) {
        throw SocketException("port_not_defined");
    }
    
    LOG(INFO) << "Connecting server socket on port " << m_port;
    
#if defined(__NIX)
    m_socket = socket(m_af, m_type, IPPROTO(m_type));
    
    /**
     * Throw an exception if the socket connection failed.
     */
    if(m_socket == -1) {
        throw SocketException("socket_failed: %s", strerror(errno));
    }
    
    /**
     * Set all bytes of m_host to zero. memset() is MT-Safe
     * See: http://man7.org/linux/man-pages/man3/memset.3.html
     */
    memset(m_host, sizeof(struct sockaddr_in), 0);
    
    /**
     * Connection info.
     */
    m_sockaddr.sin_family      = m_af;
    m_sockaddr.sin_addr.s_addr = INADDR_ANY;
    m_sockaddr.sin_port        = htons(m_port);
    
    /**
     * If binding fails, throw an exception.
     */
    if(bind(m_socket, (struct sockaddr*)&m_sockaddr, sizeof(struct sockaddr_in)) == -1) {
        close(m_socket);
        throw SocketException("bind_failed: %s", strerror(errno));
    }
    
    /**
     * If this is a TCP server then we need to
     * put the socket in listening mode.
     */
    if(m_type == SOCK_STREAM) {
        listen(m_socket, m_backlog);
    }
    
#else
    /**
     * Initiate the use of Winsock DLL
     */
    int err = WSAStartup(MAKEWORD(2, 2), &m_wsa);
    
    if (err != 0) {
        throw SocketException("WSAStartup failed: %d", err);
    }
    
    ZeroMemory(&m_hints, sizeof(m_hints));
    
    m_hints.ai_family	= m_af;
    m_hints.ai_socktype = m_type;
    m_hints.ai_protocol = IPPROTO(m_type);
    m_hints.ai_flags	= AI_PASSIVE;
    
    err = getaddrinfo(NULL, std::to_string(m_port).c_str(), &m_hints, &m_result);
    
    if (err != 0) {
        disconnect();
        throw SocketException("getaddrinfo failed: %d", err);
    }
    
    m_socket = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol);
    
    if (m_socket == INVALID_SOCKET) {
        disconnect();
        throw SocketException("socket failed with error: %ld", WSAGetLastError());
    }
    
#endif
}

/**
 * Establishes a client connection.
 */
template<SOCKET_TYPE socket_t, SERVICE_TYPE service_t>
void Socket<socket_t, service_t>::connect_client() {
    /**
     * Throw an exception if the port hasn't be defined by the user.
     */
    if(!m_port) {
        throw SocketException("port isn't defined");
    }
    
    /**
     * Throw an exception if the hostname hasn't be defined by the user.
     */
    if(m_hostname.empty()) {
        throw SocketException("hostname isn't defined");
    }
    
#if defined(__NIX)
    m_socket = socket(m_af, m_type, IPPROTO(m_type));
    
    if(m_socket == -1) {
        throw SocketException("socket_failed: %s", strerror(errno));
    }
    
    /**
     * So that we can re-bind to it without TIME_WAIT problems
     */
    int ra = 1;
    int rc = setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &ra, sizeof(ra));
    
    if(rc == -1) {
        throw SocketException("setsockopt failed: %s", strerror(errno));
    }
    
    set_blocking(m_socket, false);
    
    m_host = gethostbyname(m_hostname.c_str());
    
    if(m_host == NULL) {
        throw SocketException("gethostbyname failed: %s", hstrerror(h_errno));
    }
    
    /**
     * Zero out m_sockaddr struct and then copy the
     * host address to it's sin_addr member variable.
     */
    memset(&m_sockaddr, 0, sizeof(struct sockaddr_in));
    bcopy((char*)m_host->h_addr, (char*)&m_sockaddr.sin_addr, m_host->h_length);
    
    /**
     * Set address family and port.
     */
    m_sockaddr.sin_family = m_af;
    m_sockaddr.sin_port   = htons(m_port);
    
    /**
     * TCP
     */
    if(m_type == TCP) {
        if(::connect(m_socket, (struct sockaddr*)&m_sockaddr, sizeof(struct sockaddr_in)) == -1) {
            /*if(errno == EINPROGRESS) {
             FD_ZERO(&m_active_fd_set);
             FD_SET(m_socket, &m_active_fd_set);
             
             if (select(m_socket + 1, NULL, &m_active_fd_set, NULL, &m_time) > 0) {
             socklen_t lon = sizeof(int);
             int optval;
             getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &optval, &lon);
             
             if(optval) {
             throw SocketException("getsockopt failed: %s", strerror(optval));
             }
             }
             }
             else {
             throw SocketException("connect failed: %s", strerror(errno));
             }*/
        }
        else {
            throw SocketException("connect failed: %s", strerror(errno));
        }
    }
    else if(m_type == UDP) {
        socklen_t sock_size = sizeof(struct sockaddr*);
        
        if(bind(m_socket, (struct sockaddr*)&m_sockaddr, sock_size) == -1) {
            throw SocketException("bind_failed: ", strerror(errno));
        }
    }
    
#else
    /**
     * @TODO: implement winsock
     */
#endif
}

/**
 *
 */
template<SOCKET_TYPE socket_t, SERVICE_TYPE service_t>
bool Socket<socket_t, service_t>::ready(const uint32_t& events) {
    
    if(!(m_pfd[ 0 ].events & events)) {
        return false;
    }
    
    return true;
}

/**
 *
 */
template<SOCKET_TYPE socket_t, SERVICE_TYPE service_t>
bool Socket<socket_t, service_t>::set_blocking(int fd, bool blocking) {
    
    if(fd < 0) {
        return false;
    }
    
    uint32_t rc = -1;
    
#ifdef __WIN
    uint32_t mode = blocking ? 0 : 1;
    rc   = ioctlsocket(fd, FIONBIO, &mode)
    
#else
    int32_t flags = fcntl(fd, F_GETFL, 0);
    
    if(flags < 0) {
        return false;
    }
    
    flags = (blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK));
    rc    = fcntl(fd, F_SETFL, flags);
    
#endif
    
    return (rc == 0 ? true : false);
}

/**
 *
 */
template<SOCKET_TYPE socket_t, SERVICE_TYPE service_t>
std::string Socket<socket_t, service_t>::receive() {
        
    char buffer[ m_buf_size ];
    socklen_t sock_size = sizeof(struct sockaddr_in);
    memset(buffer, 0, m_buf_size);
    
#if defined(__NIX)
    
    if(m_type == TCP) {
        if(m_service_type == SERVER) {
            m_socket_tcp = accept(m_socket, (struct sockaddr*)&m_sockaddr, &sock_size);
            
            if(m_socket_tcp == -1) {
                close(m_socket_tcp);
                throw SocketException("accept_failed: %s", strerror(errno));
            }
            
            if(read(m_socket_tcp, &buffer, m_buf_size) == -1) {
                close(m_socket_tcp);
                throw SocketException("read_failed: %s", strerror(errno));
            }
        }
        
        else {            
            if(read(m_socket, buffer, m_buf_size) == -1) {
                close(m_socket);
                throw SocketException("read_failed: %s", strerror(errno));
            }
        }
    }
    else if(m_type == UDP) {
        if(recvfrom(m_socket, buffer, m_buf_size, 0, (struct sockaddr*)&m_sockaddr, &sock_size) == -1) {
            close(m_socket);
            throw SocketException("recvfrom_failed: %s", strerror(errno));
        }
    }
#else
    
#endif
    
    return std::string(buffer);
}

/**
 *
 */
template<SOCKET_TYPE socket_t, SERVICE_TYPE service_t>
void Socket<socket_t, service_t>::send(const std::string& message) {
    
    if(m_socket == -1) {
        throw SocketException("socket_not_established");
    }
    
#if defined(__NIX)
    if(m_type == TCP) {
        int socket = (m_service_type == SERVER ? m_socket_tcp : m_socket);
        
        if(socket == -1) {
            throw SocketException("socket_not_established");
        }
        
        size_t n = write(socket, message.c_str(), message.size());
        
        if(n == -1) {
            throw SocketException("write_failed: %s", strerror(errno));
        }
    }
    else if(m_type == UDP) {
        char buffer[ m_buf_size ];
        
        memset(buffer, 0, m_buf_size);
        
        socklen_t sock_len = sizeof(struct sockaddr_in);
        ssize_t n          = recvfrom(m_socket, buffer, m_buf_size, 0, (struct sockaddr*)&m_sockaddr, &sock_len);
        
        if(n == -1) {
            close(m_socket);
            throw SocketException("recvfrom_failed: %s", strerror(errno));
        }
    }
#else
    
#endif
}

#endif
