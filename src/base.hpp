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

#if PREDEF_PLATFORM_WINDOWS

#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

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
        virtual void connect_server() = 0;
        virtual void connect_client() = 0;
    
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

        /**
         * Member function prototypes.
         */
        void disconnect();
        void connect();

        bool ready(const uint32_t&);
        bool set_blocking(int, bool blocking = false);

        virtual std::string receive() = 0;

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
