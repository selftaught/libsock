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

#define BACKLOG_LEN 5

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
     *
	 *
     */
    template<SOCK_TYPE socket_t, SERVICE_TYPE servicec_t>
    class SockBase {
    protected:

		// Member variables
        std::string m_hostname;

        uint16_t m_port;
        uint16_t m_buf_size;

        struct sockaddr_in m_sockaddr;
        struct sockaddr_in m_client_sockaddr;

        int m_af;
        int m_backlog;
        int m_protocol;

        struct pollfd m_pfd[ 1 ];

        char* m_client_addr;

#if defined(PREDEF_PLATFORM_LINUX)

        int m_socket;
        int m_tcp_socket;

        struct hostent* m_host;
        struct hostent* m_client_host;

#elif defined(PREDEF_PLATFORM_WINDOWS)

        SOCKET m_socket;
        SOCKET m_socket_client;

        WSADATA m_wsa;

        struct addrinfo* m_result;
        struct addrinfo* m_ptr;
        struct addrinfo  m_hints;

#endif

    public:

		// Constructors
        SockBase(const uint16_t& port, int protocol = 0):
            m_port(port),
            m_af(AF_INET),
            m_backlog(BACKLOG_LEN),
            m_protocol(protocol),
            m_socket(DEFAULT_SOCKET_VAL)
        {}

        SockBase(const std::string& hostname, const uint16_t& port, int protocol = 0):
            m_hostname(hostname),
            m_port(port),
            m_af(AF_INET),
            m_backlog(BACKLOG_LEN),
            m_protocol(protocol),
            m_socket(DEFAULT_SOCKET_VAL)
        {}

        SockBase(const std::string& hostname, const std::string& port, int protocol = 0):
            m_hostname(hostname),
            m_port(std::stoi(port)),
            m_af(AF_INET),
            m_backlog(BACKLOG_LEN),
            m_protocol(protocol),
            m_socket(DEFAULT_SOCKET_VAL)
        {}

        ~SockBase();

        // Member function prototypes.
        void disconnect();

        bool ready(const uint32_t&);
        bool set_blocking(int, bool blocking = false);

        virtual void connect() = 0;
        virtual std::string receive() = 0;
        virtual ssize_t send(const std::string&, bool OOB = false) = 0;

        // Setters
        void set_hostname      (const std::string& hostname) { m_hostname   = hostname;           }
        void set_port          (const uint16_t   & port    ) { m_port       = port;               }
        void set_port          (const std::string& port    ) { m_port       = atoi(port.c_str()); }
        void set_address_family(const int        & addrf   ) { m_af         = addrf;              }
        void set_backlog       (const uint32_t   & backlog ) { m_backlog    = backlog;            }
        void set_protocol      (const int        & protocol) { m_protocol   = protocol;           }

        pollfd* handle();
    };

    /**
     *
     *
     */
	template<SOCK_TYPE socket_t, SERVICE_TYPE service_t>
	pollfd* SockBase<socket_t, service_t>::handle() {
		return m_pfd;
	}

    /**
     *
     *
     */
    template<SOCK_TYPE socket_t, SERVICE_TYPE service_t>
    SockBase<socket_t, service_t>::~SockBase() {
        disconnect();
    }

    /**
     *
     *
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
     *
     *
     */
    template<SOCK_TYPE socket_t, SERVICE_TYPE servicec_t>
    bool SockBase<socket_t, servicec_t>::ready(const uint32_t& events) {
        return !(m_pfd[0].events & events);
    }

    /**
     *
     *
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
}

#endif
