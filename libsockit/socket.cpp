
#include "main.h"

SocketBase::SocketBase(const std::string& port, const int& type) {
    m_port    = atoi(port.c_str());
    m_socket  = -1;
    m_af      = AF_INET;
    m_type    = type;
    m_backlog = 5;
}

SocketBase::SocketBase(const std::string& hostname, const std::string& port, const int& type) {
    m_hostname = hostname;
    m_port     = atoi(port.c_str());
    m_socket   = -1;
    m_af       = AF_INET;
    m_type     = type;
    m_backlog  = 5;
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
        throw SocketPortUndefinedException();
    }
    
#if defined(__NIX)
    m_socket = socket(m_af, m_type, 0);
    
    /**
     * Throw an exception if the socket connection failed.
     */
    if(m_socket == -1) {
        throw SocketConnectException();
    }
    
    int sockaddr_in_size = sizeof(m_server_addr);
    
    /**
     * Set all bytes of m_server_addr to zero.
     * memset() is MT-Safe
     * See: http://man7.org/linux/man-pages/man3/memset.3.html
     */
    memset(m_server, sockaddr_in_size, 0);
    
    /**
     * Connection info.
     */
    m_server_addr.sin_family      = m_af;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port        = htons(m_port);
    
    /**
     * If binding fails, throw an exception.
     */
    if(bind(m_socket, (struct sockaddr*)&m_server_addr, sockaddr_in_size) == -1) {
        throw SocketConnectException();
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
     * Throw an exception if the port hasn't be defined
     * by the user.
     */
    if(!m_port) {
        throw SocketPortUndefinedException();
    }
    
#if defined(__NIX)
    m_socket = socket(m_af, m_type, 0);
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