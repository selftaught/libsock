#ifndef __protocols_udp_hpp
#define __protocols_udp_hpp

#include "../base.hpp"

namespace Libsock { namespace Protocols {
    template<SERVICE_TYPE service_t>
    class UDP : public SockBase<SOCK_TYPE::UDP, service_t> {
        public:

            UDP() {}

            UDP(const std::string& _host, const uint16_t& _port):
                SockBase<SOCK_TYPE::UDP, service_t>(_host, _port)
            {}

            void connect();
            std::string receive();
    		ssize_t send(const std::string&, bool oob = 0);
    };

    /**
     *
     *
     */
    template<SERVICE_TYPE service_t>
    void UDP<service_t>::connect() {
        if (service_t == SERVER) {
			DEBUG_STDOUT("starting server connection");

			// Throw an exception if the port hasn't be defined by the user.
			if(!this->m_port) {
				throw SocketException("port is not defined");
			}

			DEBUG_STDOUT("creating socket");

#if defined(PREDEF_PLATFORM_LINUX)
			this->m_socket = socket(m_af, socket_t, m_protocol);

			// Throw an exception if the socket connection failed.
			if(this->m_socket == -1) {
				throw SocketException("socket failed: %s", std::strerror(errno));
			}

			DEBUG_STDOUT("successfully established a socket connection");

            // @TODO: Implement support and abstraction for setting
            //        and getting multiple options. This particular
            //        option (SO_REUSEADDR) allows forcing binding
            //        so we don't have to wait to bind if there's
            //        already a socket
			const int toggle = 1;

			if(setsockopt(this->m_socket, SOL_SOCKET, SO_REUSEADDR, (const void*)&toggle, sizeof(toggle)) == -1) {
				throw SocketException("setsockopt SO_REUSEADDR failed: %s", std::strerror(errno));
			}

			// SO_REUSEPORT needs to be set if the current linux kernel version is >= 3.9
#ifdef SO_REUSEPORT

			DEBUG_STDOUT("setting SO_REUSEPORT (requirement since kernel version >= 3.9)");

			if(setsockopt(this->m_socket, SOL_SOCKET, SO_REUSEPORT, (const void*)&toggle, sizeof(toggle)) == -1) {
				throw SocketException("setsockopt SO_REUSEPORT failed: %s", std::strerror(errno));
			}

#endif

			DEBUG_STDOUT("setting memory segment size of struct sockaddr_in to 0");

			// Set all bytes of m_host to zero. memset() is MT-Safe
			// See: http://man7.org/linux/man-pages/man3/memset.3.html
			memset(this->m_host, sizeof(struct sockaddr_in), 0);

			// Connection info
			this->m_sockaddr.sin_family      = this->m_af;
			this->m_sockaddr.sin_addr.s_addr = INADDR_ANY;
			this->m_sockaddr.sin_port        = htons(this->m_port);

			// If binding fails, throw an exception
			if(bind(this->m_socket, (struct sockaddr*)&this->m_sockaddr, sizeof(struct sockaddr_in)) == -1) {
				close(this->m_socket);
				throw SocketException("bind failed: %s", std::strerror(errno));
			}

			DEBUG_STDOUT("socket bind() was successful!");

#elif defined(PREDEF_PLATFORM_WINDOWS)
			// Initiate the use of Winsock DLL
			int err = WSAStartup(MAKEWORD(2, 2), &this->m_wsa);

			if (err != 0) {
				throw SocketException("WSAStartup failed: %d", err);
			}

			ZeroMemory(&this->m_hints, sizeof(this->m_hints));

			this->m_hints.ai_family	  = this->m_af;
			this->m_hints.ai_socktype = this->m_type;
			this->m_hints.ai_protocol = this->m_protocol;
			this->m_hints.ai_flags	  = AI_PASSIVE;

			err = getaddrinfo(NULL, std::to_string(this->m_port).c_str(), &this->m_hints, &this->m_result);

			if (err != 0) {
				this->disconnect();
				throw SocketException("getaddrinfo failed: %d", err);
			}

			this->m_socket = socket(
				this->m_result->ai_family,
				this->m_result->ai_socktype,
				this->m_result->ai_protocol
			);

			if (this->m_socket == INVALID_SOCKET) {
				disconnect();
				throw SocketException("socket failed with error: %ld", WSAGetLastError());
			}
#endif
        }
        else if (service_t == CLIENT) {
            // Throw an exception if the port hasn't be defined by the user
            if(!this->m_port) {
                throw SockException("port isn't defined");
            }

            // Throw an exception if the hostname hasn't be defined by the user
            if(this->m_hostname.empty()) {
                throw SockException("hostname isn't defined");
            }

#if defined(PREDEF_PLATFORM_LINUX)
            this->m_socket = socket(this->m_af, socket_t, this->m_protocol);

            if(this->m_socket == -1) {
                throw SockException("socket failed: %s", std::strerror(errno));
            }

            // So that we can re-bind to it without TIME_WAIT problems
            int ra = 1;
            int rc = setsockopt(this->m_socket, SOL_SOCKET, SO_REUSEADDR, &ra, sizeof(ra));

            if(rc == -1) {
                throw SockException("setsockopt failed: %s", std::strerror(errno));
            }

            set_blocking(this->m_socket, false);

            this->m_host = gethostbyname(this->m_hostname.c_str());

            if(m_host == NULL) {
                this->disconnect();
                throw SockException("gethostbyname failed: %s", std::strerror(h_errno));
            }

            // Zero out m_sockaddr struct and then copy the
            // host address to it's sin_addr member variable.
            memset(&this->m_sockaddr, 0, sizeof(struct sockaddr_in));
            bcopy((char*)this->m_host->h_addr, (char*)&this->m_sockaddr.sin_addr, this->m_host->h_length);

            // Set address family and port
            this->m_sockaddr.sin_family = m_af;
            this->m_sockaddr.sin_port   = htons(this->m_port);

            socklen_t sock_size = sizeof(struct sockaddr*);
            if(bind(this->m_socket, (struct sockaddr*)&this->m_sockaddr, sock_size) == -1) {
                this->disconnect();
                throw SockException("bind failed: %s", std::strerror(errno));
            }

#elif defined(PREDEF_PLATFORM_WINDOWS)
            // @TODO: implement winsock
#endif
        }
        else if (service_t == UNDEF) {
            throw SockException("invalid service type");
        }

        this->m_pfd[0].fd     = this->m_socket;
        this->m_pfd[0].events = POLLIN;
    } 

    /**
     *
     *
     */
    std::string UDP<service_t>::receive() {
        char buffer[UDP_RECV_BUF_LEN];
        socklen_t sock_size = sizeof(struct sockaddr_in);
        memset(buffer, 0, UDP_RECV_BUF_LEN);

#if defined(PREDEF_PLATFORM_LINUX)
        if(servicec_t == SERVER) {
            DEBUG_STDOUT("calling recvfrom");

            if(recvfrom(this->m_socket, buffer, UDP_RECV_BUF_LEN, 0, (struct sockaddr*)&this->m_client_sockaddr, &sock_size) == -1) {
                throw SockException("recvfrom failed: %s", std::strerror(errno));
            }

            // Get addr info about who sent the udp packet.
            this->m_client_host = gethostbyaddr((const char*)&this->m_client_sockaddr.sin_addr.s_addr, sock_size, AF_INET);

            if (this->m_client_host == NULL) {
                throw SockException("gethostbyaddr: failed to get client info");
            }

            this->m_client_addr = inet_ntoa(m_client_sockaddr.sin_addr);

            if (this->m_client_addr == NULL) {
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

	/**
	 *
	 *
     */
	template<SERVICE_TYPE service_t>
	ssize_t UDP<service_t>::send(const std::string& msg, bool oob) {
		if(service_t == SERVER) {
			if(this->m_socket == -1) {
				throw SockException("socket not connected");
			}
		}

		ssize_t bytes_sent = 0;

#if defined(PREDEF_PLATFORM_LINUX)
        size_t msg_len = strlen(msg);
		DEBUG_STDOUT("sending " + std::to_string(msg_len) + " bytes of data");
		DEBUG_STDOUT("data: " + msg);

		bytes_sent = sendto(
			this->m_socket,
			msg.c_str(),
			msg_len,
			0,
			(struct sockaddr*)&this->m_client_sockaddr,
			sizeof(this->m_client_sockaddr)
		);

		if(bytes_sent == -1) {
			throw SockException("sendto failed: %s", std::strerror(errno));
		}
#elif defined(PREDEF_PLATFORM_WINDOWS)
		// @TODO: implement send functionality for Windows
#endif
		return bytes_sent;

	}
};};

typedef Libsock::Protocols::UDP<Libsock::SERVICE_TYPE::SERVER> UDPServer;
typedef Libsock::Protocols::UDP<Libsock::SERVICE_TYPE::CLIENT> UDPClient;

#endif
