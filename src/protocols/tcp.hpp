#ifndef __protocols_tcp_hpp
#define __protocols_tcp_hpp

#include "../base.hpp"

namespace Libsock {
    namespace Protocols {
        template<SERVICE_TYPE service_t>
        class TCP : public SockBase<SOCK_TYPE::TCP, service_t> {
            protected:
            private:
            public:
                TCP() {};

                TCP(const std::string& _host):
                    SockBase<SOCK_TYPE::TCP, service_t>(_host)
                {};

                TCP(const std::string& _host, const uint16_t& _port):
                    SockBase<SOCK_TYPE::TCP, service_t>(_host, _port)
                {};

                virtual void connect_server();
                virtual void connect_client();
        };

        template<SERVICE_TYPE service_t>
        void TCP<service_t>::connect_server() {
            DEBUG_STDOUT("starting server connection");

            /**
             * Throw an exception if the port hasn't be defined by the user.
             */
            if(!this->m_port) {
                throw SockException("port not defined");
            }

            DEBUG_STDOUT("creating socket");

#if defined(PREDEF_PLATFORM_LINUX)
            this->m_socket = socket(this->m_af, SOCK_TYPE::TCP, this->m_protocol);

            /**
             * Throw an exception if the socket connection failed.
             */
            if(this->m_socket == -1) {
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

            if(setsockopt(this->m_socket, SOL_SOCKET, SO_REUSEADDR, (const void*)&toggle, sizeof(toggle)) == -1) {
                throw SockException("setsockopt SO_REUSEADDR failed: %s", std::strerror(errno));
            }

            /**
             * SO_REUSEPORT needs to be set if the current linux kernel version is >= 3.9
             */
#ifdef SO_REUSEPORT

            DEBUG_STDOUT("setting SO_REUSEPORT (requirement since kernel version >= 3.9)");

            if(setsockopt(this->m_socket, SOL_SOCKET, SO_REUSEPORT, (const void*)&toggle, sizeof(toggle)) == -1) {
                throw SockException("setsockopt SO_REUSEPORT failed: %s", std::strerror(errno));
            }

#endif

            DEBUG_STDOUT("setting memory segment size of struct sockaddr_in to 0");

            /**
             * Set all bytes of this->m_host to zero. memset() is MT-Safe
             * See: http://man7.org/linux/man-pages/man3/memset.3.html
             */
            memset(this->m_host, sizeof(struct sockaddr_in), 0);

            /**
             * Connection info.
             */
            this->m_sockaddr.sin_family      = this->m_af;
            this->m_sockaddr.sin_addr.s_addr = INADDR_ANY;
            this->m_sockaddr.sin_port        = htons(this->m_port);

            /**
             * If binding fails, throw an exception.
             */
            if(bind(this->m_socket, (struct sockaddr*)&this->m_sockaddr, sizeof(struct sockaddr_in)) == -1) {
                close(this->m_socket);
                throw SockException("bind failed: %s", std::strerror(errno));
            }

            DEBUG_STDOUT("socket bind() was successful!");
            DEBUG_STDOUT("listening for incoming tcp connections");

            listen(this->m_socket, this->m_backlog);

#elif defined(PREDEF_PLATFORM_WINDOWS)
            /**
             * Initiate the use of Winsock DLL
             */
            int err = WSAStartup(MAKEWORD(2, 2), &this->m_wsa);

            if (err != 0) {
                throw SockException("WSAStartup failed: %d", err);
            }

            ZeroMemory(&this->m_hints, sizeof(this->m_hints));

            this->m_hints.ai_family	  = this->m_af;
            this->m_hints.ai_socktype = this->m_type;
            this->m_hints.ai_protocol = this->m_protocol;
            this->m_hints.ai_flags	  = AI_PASSIVE;

            err = getaddrinfo(NULL, std::to_string(this->m_port).c_str(), &this->m_hints, &this->m_result);

            if (err != 0) {
                this->disconnect();
                throw SockException("getaddrinfo failed: %d", err);
            }

            this->m_socket = socket(
                this->m_result->ai_family,
                this->m_result->ai_socktype,
                this->m_result->ai_protocol
            );

            if (this->m_socket == INVALID_SOCKET) {
                this->disconnect();
                throw SockException("socket failed with error: %ld", WSAGetLastError());
            }
#endif
        }

        template<SERVICE_TYPE service_t>
        void TCP<service_t>::connect_client() {
            /**
             * Throw an exception if the port hasn't be defined by the user.
             */
            if(!this->m_port) {
                throw SockException("port isn't defined");
            }

            /**
             * Throw an exception if the hostname hasn't be defined by the user.
             */
            if(this->m_hostname.empty()) {
                throw SockException("hostname isn't defined");
            }

#if defined(PREDEF_PLATFORM_LINUX)
            this->m_socket = socket(this->m_af, SOCK_TYPE::TCP, this->m_protocol);

            if(this->m_socket == -1) {
                throw SockException("socket failed: %s", std::strerror(errno));
            }

            /**
             * So that we can re-bind to it without TIME_WAIT problems
             */
            int ra = 1;
            int rc = setsockopt(this->m_socket, SOL_SOCKET, SO_REUSEADDR, &ra, sizeof(ra));

            if(rc == -1) {
                throw SockException("setsockopt failed: %s", std::strerror(errno));
            }

            this->set_blocking(this->m_socket, false);
            this->m_host = gethostbyname(this->m_hostname.c_str());

            if(this->m_host == NULL) {
                this->disconnect();
                throw SockException("gethostbyname failed: %s", std::strerror(h_errno));
            }

            /**
             * Zero out this->m_sockaddr struct and then copy the
             * host address to it's sin_addr member variable.
             */
            memset(&this->m_sockaddr, 0, sizeof(struct sockaddr_in));
            bcopy((char*)this->m_host->h_addr, (char*)&this->m_sockaddr.sin_addr, this->m_host->h_length);

            /**
             * Set address family and port.
             */
            this->m_sockaddr.sin_family = this->m_af;
            this->m_sockaddr.sin_port   = htons(this->m_port);

            /**
             * TCP
             */
            if(::connect(this->m_socket, (struct sockaddr*)&this->m_sockaddr, sizeof(struct sockaddr_in)) == -1) {
                this->disconnect();
                throw SockException("connect failed: %s", std::strerror(errno));
            }

#elif defined(PREDEF_PLATFORM_WINDOWS)
            /**
             * @TODO: implement winsock
             */
#endif

        }
    };
};

typedef Libsock::Protocols::TCP<Libsock::SERVICE_TYPE::SERVER> TCPServer;
typedef Libsock::Protocols::TCP<Libsock::SERVICE_TYPE::CLIENT> TCPClient;

#endif
