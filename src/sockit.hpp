#ifndef __sockit_hpp
#define __sockit_hpp

#include <iostream>
#include <exception>
#include <cstdint>
#include <cstdarg>
#include <string>
#include <cstring>
#include <errno.h>
#include <string>
#include <vector>

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

/**
 * WINDOWS
 * @resource: https://msdn.microsoft.com/en-us/library/b0084kay.aspx
 */
#if defined(_WIN32) || defined(_WIN64)

#define PREDEF_PLATFORM_WINDOWS 1

/**
 * @include: <WinSock2.h>
 * @platform: windows
 */
#include <WinSock2.h>
/**
 * @include: <ws2tcpip.h>
 * @platform: windows
 */
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

/**
 * DARWIN / LINUX
 */
#elif defined(__APPLE__) || defined(__linux__)

#define PREDEF_PLATFORM_LINUX 1

/**
 * @include: <sys/socket.h>
 * @reference: https://linux.die.net/man/3/gethostbyaddr
 * @functions: gethostbyaddr
 * @platform: linux/darwin
 */
#include <sys/socket.h>

/**
 * @include: <netdb.h>
 * @reference: http://pubs.opengroup.org/onlinepubs/7908799/xns/netdb.h.html
 * @platform: linux/darwin
 */
#include <netdb.h>

/**
 * @include: <netinet/tcp.h>
 * @reference: http://unix.superglobalmegacorp.com/BSD4.4/newsrc/netinet/tcp.h.html
 * @variables: (provides the tcp header struct)
 * @platform: linux/darwin
 */
#include <netinet/tcp.h>

/**
 * @include: <netinet/ip.h>
 * @reference: http://unix.superglobalmegacorp.com/BSD4.4/newsrc/netinet/ip.h.html
 * (provides the ip header struct which is used for raw packets)
 * @platform: linux/darwin
 */
#include <netinet/ip.h>

/**
 * @include: <arpa/inet.h>
 * @reference: https://linux.die.net/man/3/inet_ntoa
 * (inet_ntoa, etc)
 * @platform: linux/darwin
 */
#include <arpa/inet.h>

/**
 * @include: <unistd.h>
 * @reference: http://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html
 * (standard symbolic constants and types)
 * @platform: linux/darwin
 */
#include <unistd.h>

/**
 * @include: <fcntl.h>
 * @reference: http://pubs.opengroup.org/onlinepubs/009695399/basedefs/fcntl.h.html
 * (file control options)
 * @platform: linux/darwin
 */
#include <fcntl.h>

/**
 * @include: <poll.h>
 * @reference: http://pubs.opengroup.org/onlinepubs/7908799/xsh/poll.h.html
 * (defines the pollfd structure)
 * @platform: linux/darwin
 */
#include <poll.h>

/**
 * @include: <net/if.h>
 * @reference: http://pubs.opengroup.org/onlinepubs/009696699/basedefs/net/if.h.html
 * (provides interface index / name functions)
 * @platform: linux/darwin
 */
#include <net/if.h>

/**
 * @include: <ifaddrs.h>
 * @reference: http://man7.org/linux/man-pages/man3/getifaddrs.3.html
 * (gets interface addresses)
 * @platform: linux/darwin
 */
#include <ifaddrs.h>

#endif

/**
 *
 */
#define POLL_EXPIRE 0
#define BACKLOG_LEN 5

/**
 * Default recv char array lengths for UDP and TCP.
 */
#define UDP_RECV_BUF_LEN 576
#define TCP_RECV_BUF_LEN 1500

/**
 * @enum: PROC_TYPE
 * @enumerator: UNDEF = 0
 * @enumerator: CLIENT = 1
 * @enumerator: SERVER = 2
 * @description: Process type.
 */
enum PROC_TYPE {
    UNDEF,
    CLIENT,
    SERVER
};

/**
 * ICMP control message enums.
 * @reference: https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol#Control_messages
 * @reference: https://tools.ietf.org/html/rfc792
namespace ICMP_CM {

};
*/

/**
 * @enum: SOCKET_TYPE
 * @enumerator: TCP = SOCK_STREAM
 * @enumerator: UDP = SOCK_DGRAM
 * @enumerator: RAW = SOCK_RAW
 * @enumerator: SEQ = SOCK_SEQPACKET
 * @enumerator: RDM = SOCK_RDM
 */
enum SOCKET_TYPE {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM,
    RAW = SOCK_RAW,
    SEQ = SOCK_SEQPACKET,
    RDM = SOCK_RDM
};

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

// Debug preprocessors
#define __DEBUGGING 1
#ifdef  __DEBUGGING
    #define DEBUG_STDERR(x) \
        (std::cout << "[ " << __FILE__ << " ] " \
                   << "(func " << __FUNCTION__ << "): " \
                   << "(line " << __LINE__ << "): " \
                   << (x) \
                   << std::endl \
        )
    #define DEBUG_STDOUT(x) \
        (std::cout << "[ " << __FILE__ << " ] " \
                   << "(func " << __FUNCTION__ << "): " \
                   << "(line " << __LINE__ << "): " \
                   << (x) \
                   << std::endl \
        )
#else
    #define DEBUG_STDERR(x)
    #define DEBUG_STDOUT(x)
#endif

/**
 * @class: SocketException
 * @parent: (public) std::exception
 * @credits: http://stackoverflow.com/questions/8152720/correct-way-to-inherit-from-stdexception
 */
class SocketException : public std::exception {
public:
    /**
     * @function: SocketException
     * @class: SocketException
     * @param: (const char*) format - message C-style string error message.
     *                                The string contents are copied upon construction.
     *                                Hence, responsibility for deleting the char* lies
     *                                with the caller.
     */
    explicit SocketException(const char* format, ...) {
        va_list ap;
        va_start(ap, format);
        this->init(format, ap);
        va_end(ap);
    }

    /**
     * @function: SocketException
     * @class: SocketException
     * @param: (const std::string&) message - exception message.
     */
    explicit SocketException(const std::string& message):
        msg_(message)
    {}

    /**
     * @function: ~SocketException
     * @class: SocketException
     */
    virtual ~SocketException() throw() {}

    /**
     * Need to use std::exception::what otherwise
     * g++ will generate these warnings: 'hides overloaded virtual function'
     */
    using std::exception::what;

    /**
     * @function: what
     * @class: SocketException
     * @return: const char*
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
     * @function: init
     * @class: SocketException
     * @param: (const char*) format
     * @param: (va_list) ap
     */
    void init(const char* fmt, va_list ap) {
        char errbuf[ 256 ];
        vsnprintf(errbuf, sizeof(errbuf), fmt, ap);
        msg_ = errbuf;
    }
};


/**
 * @class: Socket
 * @description:
 *  Base socket class.
 */
template<SOCKET_TYPE socket_t, PROC_TYPE proc_t>
class Socket {
protected:

    /**
     * @membervar: (std::string) m_hostname
     * @class: Socket
     * @accessor: protected
     */
    std::string m_hostname;

    /**
     * @membervar: (uint16_t) m_port
     * @class: Socket
     * @accessor: protected
     */
    uint16_t m_port;

    /**
     * @membervar: (uint16_t) m_buf_size - Receiving buffer size.
     * @class: Socket
     * @accessor: protected
     */
    uint16_t m_buf_size;

    /**
     * @membervar: (sockaddr_in) m_sockaddr
     * @class: Socket
     * @accessor: protected
     */
    struct sockaddr_in m_sockaddr;

    /**
     * @membervar: (sockaddr_in) m_client_sockaddr
     * @class: Socket
     * @accessor: protected
     */
    struct sockaddr_in m_client_sockaddr;

    /**
     * @membervar: (int) m_af - Address family.
     * @class: Socket
     * @accessor: protected
     */
    int m_af;

    /**
     * @membervar: (int) m_backlog
     * @class: Socket
     * @accessor: protected
     */
    int m_backlog;

    /**
     * @membervar: (int) m_protocol
     * @class: Socket
     * @accessor: protected
     */
    int m_protocol;

    /**
     * @membervar: (struct pollfd) m_pfd[ 1 ]
     * @class: Socket
     * @accessor: protected
     */
    struct pollfd m_pfd[ 1 ];

    /**
     * @membervar: (char*) m_client_addr
     * @class: Socket
     * @accessor: protected
     */
    char* m_client_addr;

    /**
     * @membervar: (double) m_recv_timeout
     * @class: Socket
     * @accessor: protected
     */
     double m_recv_timeout;

// If the current platform is *nix or darwin
#if defined(PREDEF_PLATFORM_LINUX)

    /**
     * @membervar: (int) m_socket
     * @accessor: protected
     * @platform: linux
     */
    int m_socket;

    /**
     * @membervar: (int) m_tcp_socket
     * @accessor: protected
     * @platform: linux
     */
    int m_tcp_socket;

    /**
     * @membervar: (struct hostent*) m_host
     * @accessor: protected
     * @platform: linux
     */
    struct hostent* m_host;

    /**
     * @membervar: (struct hostent*) m_client_host
     * @accessor: protected
     * @platform: linux
     */
    struct hostent* m_client_host;

    /**
     * Or the platform is windows.
     */
#elif defined(PREDEF_PLATFORM_WINDOWS)

    /**
     * @membervar: (SOCKET) m_result
     * @accessor: protected
     * @platform: windows
     */
    SOCKET  m_socket;

    /**
     * @membervar: (SOCKET) m_result
     * @accessor: protected
     * @platform: windows
     */
	SOCKET  m_socket_client;

    /**
     * @membervar: (SOCKET) m_result
     * @accessor: protected
     * @platform: windows
     */
    WSADATA m_wsa;

    /**
     * @membervar: (struct addrinfo*) m_result
     * @accessor: protected
     * @platform: windows
     */
	struct addrinfo* m_result;

    /**
     * @membervar: (struct addrinfo*) m_ptr
     * @accessor: protected
     * @platform: windows
     */
	struct addrinfo* m_ptr;

    /**
     * @membervar: (struct addrinfo*) m_hints
     * @accessor: protected
     * @platform: windows
     */
	struct addrinfo  m_hints;

#endif

    void connect_server();
    void connect_client();

public:

    /**
     * @function: Socket
     * @class: Socket
     * @param: (const uint16_t&) port
     * @param: (int) (default: 0) protocol
     * @accessor: public
     * @description:
     *  Explicitly disconnect during destruction.
     */
    Socket(const uint16_t& port, int protocol = 0):
        m_port(port),
        m_af(AF_INET),
        m_backlog(BACKLOG_LEN),
        m_protocol(protocol),
        m_socket(DEFAULT_SOCKET_VAL)
    {}

    /**
     * @function: Socket
     * @class: Socket
     * @param: (const std::string&) hostname
     * @param: (const uint16_t&) port
     * @param: (int) (default: 0) protocol
     * @accessor: public
     * @description:
     *  Explicitly disconnect during destruction.
     */
    Socket(const std::string& hostname, const uint16_t& port, int protocol = 0):
        m_hostname(hostname),
        m_port(port),
        m_af(AF_INET),
        m_backlog(BACKLOG_LEN),
        m_protocol(protocol),
        m_socket(DEFAULT_SOCKET_VAL)
    {}

    /**
     * @function: Socket
     * @class: Socket
     * @param: (const std::string&) hostname
     * @param: (const std::string&) port
     * @param: (int) (default: 0) protocol
     * @accessor: public
     * @description:
     *  Explicitly disconnect during destruction.
     */
    Socket(const std::string& hostname, const std::string& port, int protocol = 0):
        m_hostname(hostname),
        m_port(std::stoi(port)),
        m_af(AF_INET),
        m_backlog(BACKLOG_LEN),
        m_protocol(protocol),
        m_socket(DEFAULT_SOCKET_VAL)
    {}

    /**
     * @function: ~Socket
     * @class: Socket
     * @accessor: public
     */
    ~Socket();

    /**
     * Member function prototypes.
     */
    void init();
    void disconnect();
    void connect();

    bool is_ready(const uint32_t&);
    bool set_blocking(int, bool blocking = false);

    std::string receive();

    ssize_t send(const std::string&, bool OOB = false);

    /**
     * Setters
     */
    void set_hostname      (const std::string& hostname) { m_hostname     = hostname;           }
    void set_port          (const uint16_t   & port    ) { m_port         = port;               }
    void set_port          (const std::string& port    ) { m_port         = atoi(port.c_str()); }
    void set_address_family(const int        & addrf   ) { m_af           = addrf;              }
    void set_backlog       (const uint32_t   & backlog ) { m_backlog      = backlog;            }
    void set_protocol      (const int        & protocol) { m_protocol     = protocol;           }
    void set_recv_timeout  (double             timeout ) { m_recv_timeout = timeout;            }

    /**
     * @function: handle
     * @class: Socket
     * @return: pollfd*
     * @accessor: public
     */
    pollfd* handle() {
        return m_pfd;
    }
};

/**
 * @function: ~Socket
 * @class: Socket
 * @description:
 *  Explicitly disconnect during destruction.
 */
template<SOCKET_TYPE socket_t, PROC_TYPE proc_t>
Socket<socket_t, proc_t>::~Socket() {
    disconnect();
}

/**
 * @function: connect
 * @class: Socket
 * @description:
 *  Calls the correct connect function (server or client)
 *  based on the value of m_proc_type
 */
template<SOCKET_TYPE socket_t, PROC_TYPE proc_t>
void Socket<socket_t, proc_t>::connect() {
    m_buf_size = (socket_t == TCP ? TCP_RECV_BUF_LEN : UDP_RECV_BUF_LEN);
    if(proc_t == UNDEF) {
        throw SocketException("undefined service type");
    }

    if(proc_t == SERVER) {
        connect_server();
    }
    else if(proc_t == CLIENT) {
        connect_client();
    }
    else {
        throw SocketException("invalid service type");
    }

    m_pfd[0].fd     = m_socket;
    m_pfd[0].events = POLLIN;
}

/**
 * @function: disconnect
 * @class: Socket
 * @description: Closes the socket if it's open.
 */
template<SOCKET_TYPE socket_t, PROC_TYPE proc_t>
void Socket<socket_t, proc_t>::disconnect() {
   DEBUG_STDOUT("disconnecting socket");

#if defined(PREDEF_PLATFORM_LINUX)
    if(m_socket != -1) {
        close(m_socket);
    }
#elif defined(PREDEF_PLATFORM_WINDOWS)
    if (m_result != NULL) {
        freeaddrinfo(m_result);
    }

    WSACleanup();
#endif
}

/**
 * @function: connect_server
 * @class: Socket
 * @description:
 *  Makes a 3-way TCP handshake (Minimum number of packets: 3)
 *
 *  1. SERVER: socket(), bind(), listen()
 *
 *  2. CLIENT: sends a SYN segment by calling connect which tells the
 *             server the client's initial sequence number for the data
 *             which the client will send on the connection.
 *
 *  3. SERVER: ACKs the client's SYN and the server sends back it's own
 *             SYN segment containing the initial sequence number for the data
 *             it will send on the connection.
 *
 */
template<SOCKET_TYPE socket_t, PROC_TYPE proc_t>
void Socket<socket_t, proc_t>::connect_server() {
    DEBUG_STDOUT("starting server connection");
    // Throw an exception if the port hasn't be defined by the user.
    if(!m_port) {
        throw SocketException("port not defined");
    }

    DEBUG_STDOUT("creating socket");
#if defined(PREDEF_PLATFORM_LINUX)
    m_socket = socket(m_af, socket_t, m_protocol);
    if(m_socket == -1) {
        throw SocketException("socket failed: %s", std::strerror(errno));
    }

    DEBUG_STDOUT("successfully established a socket connection");
    /**
     * @TODO: Implement support and abstraction for setting
     *        and getting multiple options. This particular
     *        option (SO_REUSEADDR) allows forcing binding
     *        so we don't have to wait to bind if there's
     *        already a socket
     */
    const int toggle = 1;
    if(setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (const void*)&toggle, sizeof(toggle)) == -1) {
        throw SocketException("setsockopt SO_REUSEADDR failed: %s", std::strerror(errno));
    }

    /**
     * SO_REUSEPORT needs to be set if the current linux kernel version is >= 3.9
     */
#ifdef SO_REUSEPORT

    DEBUG_STDOUT("setting SO_REUSEPORT (requirement since kernel version >= 3.9)");

    if(setsockopt(m_socket, SOL_SOCKET, SO_REUSEPORT, (const void*)&toggle, sizeof(toggle)) == -1) {
        throw SocketException("setsockopt SO_REUSEPORT failed: %s", std::strerror(errno));
    }

#endif
    DEBUG_STDOUT("setting memory segment size of struct sockaddr_in to 0");

    /**
     * Set all bytes of m_host to zero. memset() is MT-Safe
     * See: http://man7.org/linux/man-pages/man3/memset.3.html
     */
    memset(m_host, (int)sizeof(struct sockaddr_in), (0));

    // Connection info.
    m_sockaddr.sin_family      = m_af;
    m_sockaddr.sin_addr.s_addr = INADDR_ANY;
    m_sockaddr.sin_port        = htons(m_port);

    // If binding fails, throw an exception.
    if(bind(m_socket, (struct sockaddr*)&m_sockaddr, sizeof(struct sockaddr_in)) == -1) {
        close(m_socket);
        throw SocketException("bind failed: %s", std::strerror(errno));
    }

    DEBUG_STDOUT("socket bind() was successful!");

    /**
     * If this is a TCP server then we need to
     * put the socket in listening mode.
     */
    if(socket_t == TCP) {
        DEBUG_STDOUT("listening for incoming tcp connections");
        listen(m_socket, m_backlog);
    }

#elif defined(PREDEF_PLATFORM_WINDOWS)
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
    m_hints.ai_protocol = m_protocol;
    m_hints.ai_flags	= AI_PASSIVE;

    err = getaddrinfo(NULL, std::to_string(m_port).c_str(), &m_hints, &m_result);

    if (err != 0) {
        disconnect();
        throw SocketException("getaddrinfo failed: %d", err);
    }

    m_socket = socket(
        m_result->ai_family,
        m_result->ai_socktype,
        m_result->ai_protocol
    );

    if (m_socket == INVALID_SOCKET) {
        disconnect();
        throw SocketException("socket failed with error: %ld", WSAGetLastError());
    }
#endif
}

/**
 * @function: connect_client
 * @class: Socket
 * @description: Establishes a client connection.
 */
template<SOCKET_TYPE socket_t, PROC_TYPE proc_t>
void Socket<socket_t, proc_t>::connect_client() {
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

#if defined(PREDEF_PLATFORM_LINUX)
    m_socket = socket(m_af, socket_t, m_protocol);

    if(m_socket == -1) {
        throw SocketException("socket failed: %s", std::strerror(errno));
    }

    /**
     * So that we can re-bind to it without TIME_WAIT problems
     */
    int ra = 1;
    int rc = setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &ra, sizeof(ra));

    if(rc == -1) {
        throw SocketException("setsockopt failed: %s", std::strerror(errno));
    }

    set_blocking(m_socket, false);

    m_host = gethostbyname(m_hostname.c_str());

    if(m_host == NULL) {
        disconnect();
        throw SocketException("gethostbyname failed: %s", std::strerror(h_errno));
    }

    /**
     *
     * @TODO:
     *
     *
     *
    if (socket_t == UDP) {
        struct addrinfo hints;
        struct addrinfo *result, *rp;
        int sfd, s, j;
        size_t len;
        ssize_t nread;
        char buf[BUF_SIZE];

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family   = AF_UNSPEC;
        hints.ai_socktype = socket_t; 
        hints.ai_flags    = 0;
        hints.ai_protocol = 0;

        s = getaddrinfo((char*)m_host->h_addr, htons(m_port), &hints, &result);

        if (s != 0) {
            throw SocketException("getaddrinfo call was not successful!");
        }

        for (rp = result; rp != NULL; rp = rp->ai_next) {
            sfd = socket(
                rp->ai_family,
                rp->ai_socktype,
                rp->ai_protocol
            );

            if (sfd == -1) {
                continue;
            }

            if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
                break;
            }

            close(sfd);
        }

        if (rp == NULL) {
            throw SocketException("Couldn't connect to any addresses returned by getaddrinfo()!");
        }

        freeaddrinfo(result);
    }
    */


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
    if(socket_t == TCP) {
        if(::connect(m_socket, (struct sockaddr*)&m_sockaddr, sizeof(struct sockaddr_in)) == -1) {
            disconnect();
            throw SocketException("connect failed: %s", std::strerror(errno));
        }
    }
    else if(socket_t == UDP) {
        socklen_t sock_size = sizeof(struct sockaddr*);
        if(bind(m_socket, (struct sockaddr*)&m_sockaddr, sock_size) == -1) {
            disconnect();
            throw SocketException("bind failed: %s", std::strerror(errno));
        }
    }

#elif defined(PREDEF_PLATFORM_WINDOWS)
    /**
     * @TODO: implement winsock
     */
#endif
}

/**
 * @function: ready
 * @class: Socket
 * @param: (const uint32_t&) events - poll events
 * @return: bool
 */
template<SOCKET_TYPE socket_t, PROC_TYPE proc_t>
bool Socket<socket_t, proc_t>::is_ready(const uint32_t& events) {
    return !(m_pfd[ 0 ].events & events);
}

/**
 * @function: set_blocking
 * @class: Socket
 * @param: (int) fd - file descriptor
 * @param: (bool) blocking - blocking flag
 * @return: bool
 */
template<SOCKET_TYPE socket_t, PROC_TYPE proc_t>
bool Socket<socket_t, proc_t>::set_blocking(int fd, bool blocking) {
    if(fd < 0) {
        DEBUG_STDOUT("fd (file descriptor) is < 0 - failed to update the sockets blocking state");
        return false;
    }

    DEBUG_STDOUT("setting socket blocking to: " + std::to_string(blocking));

    uint32_t rc = -1;

#if defined(PREDEF_PLATFORM_LINUX)
    int32_t flags = fcntl(fd, F_GETFL, 0);

    if(flags < 0) {
        return false;
    }

    flags = (blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK));
    rc    = fcntl(fd, F_SETFL, flags);

#elif defined(PREDEF_PLATFORM_WINDOWS)
    uint32_t mode = blocking ? 0 : 1;
             rc   = ioctlsocket(fd, FIONBIO, &mode)

#endif
    return (rc == 0 ? true : false);
}

/**
 * @function: receive
 * @class: Socket
 * @return: std::string
 */
template<SOCKET_TYPE socket_t, PROC_TYPE proc_t>
std::string Socket<socket_t, proc_t>::receive() {
    char buffer[ m_buf_size ];
    socklen_t sock_size = sizeof(struct sockaddr_in);
    memset(buffer, 0, m_buf_size);

#if defined(PREDEF_PLATFORM_LINUX)
    if(socket_t == TCP) {
        if(proc_t == SERVER) {
            DEBUG_STDOUT("accepting connection from client...");
            m_tcp_socket = accept(m_socket, (struct sockaddr*)&m_sockaddr, &sock_size);
            if(m_tcp_socket == -1) {
                close(m_tcp_socket);
                throw SocketException("accept failed: %s", std::strerror(errno));
            }
            if(read(m_tcp_socket, &buffer, m_buf_size) == -1) {
                close(m_tcp_socket);
                throw SocketException("read failed: %s", std::strerror(errno));
            }
        }
        else {
            DEBUG_STDOUT("listening for a response from the server...");
            if(read(m_socket, buffer, m_buf_size) == -1) {
                close(m_socket);
                throw SocketException("read failed: %s", std::strerror(errno));
            }
        }
    }
    else if(socket_t == UDP) {
        if(proc_t == SERVER) {
            DEBUG_STDOUT("calling recvfrom");
            if(recvfrom(m_socket, buffer, m_buf_size, 0, (struct sockaddr*)&m_client_sockaddr, &sock_size) == -1) {
                throw SocketException("recvfrom failed: %s", std::strerror(errno));
            }

            m_client_host = gethostbyaddr((const char*)&m_client_sockaddr.sin_addr.s_addr, sock_size, AF_INET);
            if (m_client_host == NULL) {
                throw SocketException("gethostbyaddr: failed to get client info");
            }

            m_client_addr = inet_ntoa(m_client_sockaddr.sin_addr);
            if (m_client_addr == NULL) {
                throw SocketException("inet_ntoa: returned null - couldn't get client addr");
            }
        }
        else {
           /**
            * @TODO: implement UDP client recv functionality
            */
        }
    }
    else if(socket_t == RAW) {

    }
#elif defined(PREDEF_PLATFORM_WINDOWS)
    /**
     * @TODO: implement send functionality for Windows
     */
#endif
    return std::string(buffer);
}

/**
 * @function: send
 * @param (const std::string&) message - message to send
 * @param (bool) (default: false) OOB - out of bounds
 * @return ssize_t
 */
template<SOCKET_TYPE socket_t, PROC_TYPE proc_t>
ssize_t Socket<socket_t, proc_t>::send(const std::string& message, bool OOB) {
    if(proc_t == SERVER) {
        if(m_socket == -1 || (socket_t == TCP && m_tcp_socket == -1)) {
            throw SocketException("socket not connected");
        }
    }

    ssize_t bytes_sent = 0;
    int socket = (socket_t == UDP ? m_socket : m_tcp_socket);

#if defined(PREDEF_PLATFORM_LINUX)
    DEBUG_STDOUT("sending " + std::to_string(message.size()) + " bytes of data");
    DEBUG_STDOUT("data: " + message);

    if(socket_t == TCP) {
        bytes_sent = write(socket, message.c_str(), strlen(message.c_str()));

        if(bytes_sent == -1) {
           throw SocketException("sendto failed: %s", std::strerror(errno));
        }

        /**
         * Close the TCP child socket otherwise the request will hang.
         */
        DEBUG_STDOUT("closing connection...");
        close(m_tcp_socket);
    }
    else if(socket_t == UDP) {
        bytes_sent = sendto(
            m_socket,
            message.c_str(),
            strlen(message.c_str()),
            0,
            (struct sockaddr*)&m_client_sockaddr,
            sizeof(m_client_sockaddr)
        );

        if(bytes_sent == -1) {
           throw SocketException("sendto failed: %s", std::strerror(errno));
        }
    }
#elif defined(PREDEF_PLATFORM_WINDOWS)
    /**
     * @TODO: implement send functionality for Windows
     */
#endif
    return bytes_sent;
}

/**
 * @function: get_receive_timeout
 */
template<SOCKET_TYPE socket_t, PROC_TYPE proc_t>
double Socket<socket_t, proc_t>::get_receive_timeout() {
    return m_recv_timeout;
}

typedef Socket<TCP, SERVER> TcpServer;

#endif
