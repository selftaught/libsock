#ifndef __sock_base_hpp
#define __sock_base_hpp

#include <iostream>
#include <exception>
#include <cstdint>
#include <cstdarg>
#include <string>
#include <cstring>
#include <errno.h>
#include <string>
#include <vector>

#include "sock/defs.hpp"
#include "sock/enums.hpp"
#include "sock/exception.hpp"

/**
 * WINDOWS
 */
#if PREDEF_PLATFORM_WINDOWS

#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

/**
 * DARWIN / LINUX
 */
#elif PREDEF_PLATFORM_LINUX

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <net/if.h>
#include <ifaddrs.h>

#endif

namespace Libsock {
    /**
     * @class: SockBase
     * @description:
     *  Base socket class.
     */
    template<SOCK_TYPE socket_t, SERVICE_TYPE servicec_t>
    class SockBase {
    protected:
        /**
         * Member function prototypes.
         */
        void disconnect();
        void connect();

        /**
         * @membervar: (std::string) m_hostname
         * @class: SockBase
         * @accessor: protected
         */
        std::string m_hostname;

        /**
         * @membervar: (uint16_t) m_port
         * @class: SockBase
         * @accessor: protected
         */
        uint16_t m_port;

        /**
         * @membervar: (uint16_t) m_buf_size - Receiving buffer size.
         * @class: SockBase
         * @accessor: protected
         */
        uint16_t m_buf_size;

        /**
         * @membervar: (sockadd_in) m_sockaddr
         * @class: SockBase
         * @accessor: protected
         */
        struct sockaddr_in m_sockaddr;

        /**
         * @membervar: (sockadd_in) m_client_sockaddr
         * @class: SockBase
         * @accessor: protected
         */
        struct sockaddr_in m_client_sockaddr;

        /**
         * @membervar: (int) m_af - Address family.
         * @class: SockBase
         * @accessor: protected
         */
        int m_af;

        /**
         * @membervar: (int) m_backlog
         * @class: SockBase
         * @accessor: protected
         */
        int m_backlog;

        /**
         * @membervar: (int) m_protocol
         * @class: SockBase
         * @accessor: protected
         */
        int m_protocol;

        /**
         * @membervar: (struct pollfd) m_pfd[ 1 ]
         * @class: SockBase
         * @accessor: protected
         */
        struct pollfd m_pfd[ 1 ];

        /**
         * @membervar: (char*) m_client_addr
         * @class: SockBase
         * @accessor: protected
         */
        char* m_client_addr;

        /**
         * If the current platform is *nix or darwin
         */
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

        virtual void connect_server();
        virtual void connect_client();

    public:

        /**
         * @function: SockBase
         * @class: SockBase
         * @param: (const uint16_t&) port
         * @param: (int) (default: 0) protocol
         * @accessor: public
         * @description:
         *  Explicitly disconnect during destruction.
         */
        SockBase(const uint16_t& port, int protocol = 0):
            m_port(port),
            m_af(AF_INET),
            m_backlog(5),
            m_protocol(protocol),
            m_socket(DEFAULT_SOCKET_VAL)
        {}

        /**
         * @function: SockBase
         * @class: SockBase
         * @param: (const std::string&) hostname
         * @param: (const uint16_t&) port
         * @param: (int) (default: 0) protocol
         * @accessor: public
         * @description:
         *  Explicitly disconnect during destruction.
         */
        SockBase(const std::string& hostname, const uint16_t& port, int protocol = 0):
            m_hostname(hostname),
            m_port(port),
            m_af(AF_INET),
            m_backlog(5),
            m_protocol(protocol),
            m_socket(DEFAULT_SOCKET_VAL)
        {}

        /**
         * @function: SockBase
         * @class: SockBase
         * @param: (const std::string&) hostname
         * @param: (const std::string&) port
         * @param: (int) (default: 0) protocol
         * @accessor: public
         * @description:
         *  Explicitly disconnect during destruction.
         */
        SockBase(const std::string& hostname, const std::string& port, int protocol = 0):
            m_hostname(hostname),
            m_port(std::stoi(port)),
            m_af(AF_INET),
            m_backlog(5),
            m_protocol(protocol),
            m_socket(DEFAULT_SOCKET_VAL)
        {}

        /**
         * @function: ~SockBase
         * @class: SockBase
         * @accessor: public
         */
        ~SockBase();


        bool ready(const uint32_t&);
        bool set_blocking(int, bool blocking = false);

        std::string receive();

        ssize_t send(const std::string&, bool OOB = false);

        /**
         * Setters
         */
        void set_hostname      (const std::string& hostname) { m_hostname   = hostname;           }
        void set_port          (const uint16_t   & port    ) { m_port       = port;               }
        void set_port          (const std::string& port    ) { m_port       = atoi(port.c_str()); }
        void set_address_family(const int        & addrf   ) { m_af         = addrf;              }
        void set_backlog       (const uint32_t   & backlog ) { m_backlog    = backlog;            }
        void set_protocol      (const int        & protocol) { m_protocol   = protocol;           }

        /**
         * @function: handle
         * @class: SockBase
         * @return: pollfd*
         * @accessor: public
         */
        pollfd* handle() {
            return m_pfd;
        }
    };

    /**
     * @function: ~SockBase
     * @class: SockBase
     * @description:
     *  Explicitly disconnect during destruction.
     */
    template<SOCK_TYPE socket_t, SERVICE_TYPE servicec_t>
    SockBase<socket_t, servicec_t>::~SockBase() {
        disconnect();
    }

    /**
     * @function: connect
     * @class: SockBase
     * @descrition:
     *  Calls the correct connect function (server or client)
     *  based on the value of m_servicec_type
     */
    template<SOCK_TYPE socket_t, SERVICE_TYPE servicec_t>
    void SockBase<socket_t, servicec_t>::connect() {
        /**
         * TCP specific declarations
         */
        if(socket_t == TCP) {
           m_buf_size = TCP_RECV_BUF_LEN;
        }
        /**
         * UDP specific declarations
         */
        else if(socket_t == UDP) {
            m_buf_size = UDP_RECV_BUF_LEN;
        }

        if(servicec_t == UNDEF) {
            throw SockException("undefined service type");
        }

             if(servicec_t == SERVER) { connect_server(); }
        else if(servicec_t == CLIENT) { connect_client(); }
        else {
            throw SockException("invalid service type");
        }

        m_pfd[0].fd     = m_socket;
        m_pfd[0].events = POLLIN;
    }

    /**
     * @function: disconnect
     * @class: SockBase
     * @description: Closes the socket if it's open.
     */
    template<SOCK_TYPE socket_t, SERVICE_TYPE servicec_t>
    void SockBase<socket_t, servicec_t>::disconnect() {
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
     * @functon: connect_server
     * @class: SockBase
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
    template<SOCK_TYPE socket_t, SERVICE_TYPE servicec_t>
    void SockBase<socket_t, servicec_t>::connect_server() {

        DEBUG_STDOUT("starting server connection");

        /**
         * Throw an exception if the port hasn't be defined by the user.
         */
        if(!m_port) {
            throw SockException("port not defined");
        }

        DEBUG_STDOUT("creating socket");

#if defined(PREDEF_PLATFORM_LINUX)
        m_socket = socket(m_af, socket_t, m_protocol);

        /**
         * Throw an exception if the socket connection failed.
         */
        if(m_socket == -1) {
            throw SockException("socket failed: %s", std::strerror(errno));
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
            throw SockException("setsockopt SO_REUSEADDR failed: %s", std::strerror(errno));
        }

        /**
         * SO_REUSEPORT needs to be set if the current linux kernel version is >= 3.9
         */
#ifdef SO_REUSEPORT

        DEBUG_STDOUT("setting SO_REUSEPORT (requirement since kernel version >= 3.9)");

        if(setsockopt(m_socket, SOL_SOCKET, SO_REUSEPORT, (const void*)&toggle, sizeof(toggle)) == -1) {
            throw SockException("setsockopt SO_REUSEPORT failed: %s", std::strerror(errno));
        }

#endif

        DEBUG_STDOUT("setting memory segment size of struct sockaddr_in to 0");

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
            throw SockException("bind failed: %s", std::strerror(errno));
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
            throw SockException("WSAStartup failed: %d", err);
        }

        ZeroMemory(&m_hints, sizeof(m_hints));

        m_hints.ai_family	= m_af;
        m_hints.ai_socktype = m_type;
        m_hints.ai_protocol = m_protocol;
        m_hints.ai_flags	= AI_PASSIVE;

        err = getaddrinfo(NULL, std::to_string(m_port).c_str(), &m_hints, &m_result);

        if (err != 0) {
            disconnect();
            throw SockException("getaddrinfo failed: %d", err);
        }

        m_socket = socket(
            m_result->ai_family,
            m_result->ai_socktype,
            m_result->ai_protocol
        );

        if (m_socket == INVALID_SOCKET) {
            disconnect();
            throw SockException("socket failed with error: %ld", WSAGetLastError());
        }
#endif
    }

    /**
     * @function: connect_client
     * @class: SockBase
     * @description: Establishes a client connection.
     */
    template<SOCK_TYPE socket_t, SERVICE_TYPE servicec_t>
    void SockBase<socket_t, servicec_t>::connect_client() {
        /**
         * Throw an exception if the port hasn't be defined by the user.
         */
        if(!m_port) {
            throw SockException("port isn't defined");
        }

        /**
         * Throw an exception if the hostname hasn't be defined by the user.
         */
        if(m_hostname.empty()) {
            throw SockException("hostname isn't defined");
        }

#if defined(PREDEF_PLATFORM_LINUX)
        m_socket = socket(m_af, socket_t, m_protocol);

        if(m_socket == -1) {
            throw SockException("socket failed: %s", std::strerror(errno));
        }

        /**
         * So that we can re-bind to it without TIME_WAIT problems
         */
        int ra = 1;
        int rc = setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &ra, sizeof(ra));

        if(rc == -1) {
            throw SockException("setsockopt failed: %s", std::strerror(errno));
        }

        set_blocking(m_socket, false);

        m_host = gethostbyname(m_hostname.c_str());

        if(m_host == NULL) {
            disconnect();
            throw SockException("gethostbyname failed: %s", std::strerror(h_errno));
        }

        /**
         *
         * @TODO:
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
                throw SockException("getaddrinfo call was not successful!");
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
                throw SockException("Couldn't connect to any addresses returned by getaddrinfo()!");
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
                throw SockException("connect failed: %s", std::strerror(errno));
            }
        }
        else if(socket_t == UDP) {
            socklen_t sock_size = sizeof(struct sockaddr*);
            if(bind(m_socket, (struct sockaddr*)&m_sockaddr, sock_size) == -1) {
                disconnect();
                throw SockException("bind failed: %s", std::strerror(errno));
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
     * @class: SockBase
     * @param: (const uint32_t&) events - poll events
     * @return: bool
     */
    template<SOCK_TYPE socket_t, SERVICE_TYPE servicec_t>
    bool SockBase<socket_t, servicec_t>::ready(const uint32_t& events) {
        return !(m_pfd[ 0 ].events & events);
    }

    /**
     * @function: set_blocking
     * @class: SockBase
     * @param: (int) fd - file descriptor
     * @param: (bool) blocking - blocking flag
     * @return: bool
     */
    template<SOCK_TYPE socket_t, SERVICE_TYPE servicec_t>
    bool SockBase<socket_t, servicec_t>::set_blocking(int fd, bool blocking) {
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
     * @class: SockBase
     * @return: std::string
     */
    template<SOCK_TYPE socket_t, SERVICE_TYPE servicec_t>
    std::string SockBase<socket_t, servicec_t>::receive() {
        char buffer[ m_buf_size ];
        socklen_t sock_size = sizeof(struct sockaddr_in);
        memset(buffer, 0, m_buf_size);

#if defined(PREDEF_PLATFORM_LINUX)
        /**
         * TCP
         */
        if(socket_t == TCP) {
            /**
             * Server
             */
            if(servicec_t == SERVER) {
                DEBUG_STDOUT("accepting connection from client...");
                m_tcp_socket = accept(m_socket, (struct sockaddr*)&m_sockaddr, &sock_size);
                if(m_tcp_socket == -1) {
                    close(m_tcp_socket);
                    throw SockException("accept failed: %s", std::strerror(errno));
                }
                if(read(m_tcp_socket, &buffer, m_buf_size) == -1) {
                    close(m_tcp_socket);
                    throw SockException("read failed: %s", std::strerror(errno));
                }
            }
            /**
             * Client
             */
            else {
                DEBUG_STDOUT("listening for a response from the server...");

                if(read(m_socket, buffer, m_buf_size) == -1) {
                    close(m_socket);
                    throw SockException("read failed: %s", std::strerror(errno));
                }
            }
        }
        /**
         * UDP
         */
        else if(socket_t == UDP) {
            /**
             * Server
             */
            if(servicec_t == SERVER) {
                DEBUG_STDOUT("calling recvfrom");

                if(recvfrom(m_socket, buffer, m_buf_size, 0, (struct sockaddr*)&m_client_sockaddr, &sock_size) == -1) {
                    throw SockException("recvfrom failed: %s", std::strerror(errno));
                }

                /**
                 * Get addr info about who sent the udp packet.
                 */
                m_client_host = gethostbyaddr((const char*)&m_client_sockaddr.sin_addr.s_addr, sock_size, AF_INET);

                if (m_client_host == NULL) {
                    throw SockException("gethostbyaddr: failed to get client info");
                }

                m_client_addr = inet_ntoa(m_client_sockaddr.sin_addr);

                if (m_client_addr == NULL) {
                    throw SockException("inet_ntoa: returned null - couldn't get client addr");
                }
            }
            /**
             * Client
             */
            else {
               /**
                * @TODO: implement UDP client recv functionality
                */
            }
        }
        /**
         * RAW
         */
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
    template<SOCK_TYPE socket_t, SERVICE_TYPE servicec_t>
    ssize_t SockBase<socket_t, servicec_t>::send(const std::string& message, bool OOB) {
        if(servicec_t == SERVER) {
            if(m_socket == -1 || (socket_t == TCP && m_tcp_socket == -1)) {
                throw SockException("socket not connected");
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
               throw SockException("sendto failed: %s", std::strerror(errno));
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
               throw SockException("sendto failed: %s", std::strerror(errno));
            }
        }
#elif defined(PREDEF_PLATFORM_WINDOWS)
        /**
         * @TODO: implement send functionality for Windows
         */
#endif
        return bytes_sent;
    }
}

#endif