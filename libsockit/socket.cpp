
#include "main.hpp"

SocketBase::SocketBase(const std::string& port, const int& type, uint16_t buf_size) {
    m_port         = atoi(port.c_str());
    m_socket       = -1;
    m_af           = AF_INET;
    m_type         = type;
    m_backlog      = 5;
    m_buf_size     = buf_size;
    m_service_type = SERVER;
#if defined(__NIX)
    m_sockaddr_in_size = sizeof(struct sockaddr_in);
#else
    
#endif
}

SocketBase::SocketBase(const std::string& hostname, const std::string& port, const int& type, uint16_t buf_size) {
    m_hostname     = hostname;
    m_port         = atoi(port.c_str());
    m_socket       = -1;
    m_af           = AF_INET;
    m_type         = type;
    m_backlog      = 5;
    m_buf_size     = buf_size;
    m_service_type = CLIENT;
#if defined(__NIX)
    m_sockaddr_in_size = sizeof(struct sockaddr_in);
#else
    
#endif
}

SocketBase::~SocketBase() {
#if defined(__NIX)
    
#else

#endif
    disconnect();
}

/**
 *
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
    if(bind(m_socket, (struct sockaddr*)&m_sockaddr, m_sockaddr_in_size) == -1) {
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
     * @TODO: implement winsock
     */
#endif
}

/**
 *
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
    
    bzero((char*)&m_sockaddr, sizeof(m_sockaddr));
    
    m_sockaddr.sin_family = m_af;
    
    bcopy((char*)m_host->h_addr, (char*)&m_sockaddr.sin_addr, m_host->h_length);
    
    /**
     * TCP
     */
    if(m_type == SOCK_STREAM) {
        
    }
    /**
     * UDP
     */
    else {
        
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
    
#endif
}


