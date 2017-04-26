#ifndef __protocols_udp_hpp
#define __protocols_udp_hpp

#include "../base.hpp"

namespace Libsock {
    namespace Protocols {
        template<SERVICE_TYPE service_t>
        class UDP : public SockBase<SOCK_TYPE::UDP, service_t> {
            private:
            public:
                UDP() {};
                UDP(const std::string& _host, const uint16_t& _port):
                    SockBase<SOCK_TYPE::UDP, service_t>(_host, _port)
                {};

                void connect_server();
                void connect_client();

                std::string receive();
        };

        /**
         *
         *
         */
        template<SERVICE_TYPE service_t>
        void UDP<service_t>::connect_server() {

        }

        /**
         *
         *
         */
        template<SERVICE_TYPE service_t>
        void UDP<service_t>::connect_client() {
            // Throw an exception if the port hasn't be defined by the user.
            if(!m_port) {
                throw SockException("port isn't defined");
            }

            // Throw an exception if the hostname hasn't be defined by the user.
            if(m_hostname.empty()) {
                throw SockException("hostname isn't defined");
            }

#if defined(PREDEF_PLATFORM_LINUX)
            m_socket = socket(m_af, socket_t, m_protocol);

            if(m_socket == -1) {
                throw SockException("socket failed: %s", std::strerror(errno));
            }

            // So that we can re-bind to it without TIME_WAIT problems
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

            // Zero out m_sockaddr struct and then copy the
            // host address to it's sin_addr member variable.
            memset(&m_sockaddr, 0, sizeof(struct sockaddr_in));
            bcopy((char*)m_host->h_addr, (char*)&m_sockaddr.sin_addr, m_host->h_length);

            // Set address family and port.
            m_sockaddr.sin_family = m_af;
            m_sockaddr.sin_port   = htons(m_port);

            socklen_t sock_size = sizeof(struct sockaddr*);
            if(bind(m_socket, (struct sockaddr*)&m_sockaddr, sock_size) == -1) {
                disconnect();
                throw SockException("bind failed: %s", std::strerror(errno));
            }

#elif defined(PREDEF_PLATFORM_WINDOWS)
            // @TODO: implement winsock
#endif
        }
        
        /**
         *
         *
         */
        std::string UDP<service_t>::receive() {
            char buffer[ m_buf_size ];
            socklen_t sock_size = sizeof(struct sockaddr_in);
            memset(buffer, 0, m_buf_size);

#if defined(PREDEF_PLATFORM_LINUX)
            if(servicec_t == SERVER) {
                DEBUG_STDOUT("calling recvfrom");

                if(recvfrom(m_socket, buffer, m_buf_size, 0, (struct sockaddr*)&m_client_sockaddr, &sock_size) == -1) {
                    throw SockException("recvfrom failed: %s", std::strerror(errno));
                }

                // Get addr info about who sent the udp packet.
                m_client_host = gethostbyaddr((const char*)&m_client_sockaddr.sin_addr.s_addr, sock_size, AF_INET);

                if (m_client_host == NULL) {
                    throw SockException("gethostbyaddr: failed to get client info");
                }

                m_client_addr = inet_ntoa(m_client_sockaddr.sin_addr);

                if (m_client_addr == NULL) {
                    throw SockException("inet_ntoa: returned null - couldn't get client addr");
                }
            }
            else {
                // @TODO: implement UDP client recv functionality
            }
#elif defined(PREDEF_PLATFORM_WINDOWS)
            // @TODO: implement send functionality for Windows
#endif
            return std::string(buffer);
        }
    };
};

typedef Libsock::Protocols::UDP<Libsock::SERVICE_TYPE::SERVER> UDPServer;
typedef Libsock::Protocols::UDP<Libsock::SERVICE_TYPE::CLIENT> UDPClient;

#endif
