
#include "main.hpp"

SocketBase::SocketBase(const std::string& port, const int& type, uint16_t buf_size) {
	m_socket	   = DEFAULT_SOCKET_VAL;
    m_port         = atoi(port.c_str());
    m_af           = AF_INET;
    m_type         = type;
    m_backlog      = 5;
    m_buf_size     = buf_size;
    m_service_type = SERVER;
}

SocketBase::SocketBase(const std::string& hostname, const std::string& port, const int& type, uint16_t buf_size) {
	m_socket	   = DEFAULT_SOCKET_VAL;
	m_hostname     = hostname;
    m_port         = atoi(port.c_str());
    m_af           = AF_INET;
    m_type         = type;
    m_backlog      = 5;
    m_buf_size     = buf_size;
    m_service_type = CLIENT;
}

SocketBase::~SocketBase() {
#if defined(__NIX)
    
#else

#endif
    disconnect();
}

/**
 * Calls the correct connect function (server or client) 
 * based on the value of m_service_type;
 */
void SocketBase::connect() {
    if(m_service_type == UNDEF) {
        throw SocketException("service_type_not_specified");
    }
    
         if(m_service_type == SERVER) { connect_server(); }
    else if(m_service_type == CLIENT) { connect_client(); }
    else {
        throw SocketException("invalid_service_type");
    }
}

/**
 * Does the needful to setup a server socket.
 */
void SocketBase::connect_server() {
    /**
     * Throw an exception if the port hasn't be defined by the user.
     */
    if(!m_port) {
        throw SocketException("port_not_defined");
    }
   
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
 * Does the needful to setup a client socket.
 */
void SocketBase::connect_client() {
    /**
     * Throw an exception if the port hasn't be defined by the user.
     */
    if(!m_port) {
        throw SocketException("port_not_defined");
    }
    
    /**
     * Throw an exception if the hostname hasn't be defined by the user.
     */
    if(m_hostname.empty()) {
        throw SocketException("hostname_not_defined");
    }
    
#if defined(__NIX)
    m_socket = socket(m_af, m_type, IPPROTO(m_type));
    
    if(m_socket == -1) {
        throw SocketException("socket_failed: %s", strerror(errno));
    }
    
    m_host = gethostbyname(m_hostname.c_str());
    
    if(m_host == NULL) {
        throw SocketException("gethostbyname_failed: %s", hstrerror(h_errno));
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
    if(m_type == SOCK_STREAM) {
        if(::connect(m_socket, (struct sockaddr*)&m_sockaddr, sizeof(struct sockaddr_in)) == -1) {
            throw SocketException("connect_failed: %s", strerror(errno));
        }
    }

#else
    /**
     * @TODO: implement winsock
     */
#endif
}

/**
 * Closes the socket if it's open.
 */
void SocketBase::disconnect() {
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


