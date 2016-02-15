
#include "main.hpp"

/**
 *
 *
 * @return void
 */
void TcpSocket::connect() {
    if(m_service_type == UNDEF) {
        throw SocketException("service_type_not_specified");
    }
    
    if(m_service_type == SERVER) {
        SocketBase::connect_server();
    }
    else if(m_service_type == CLIENT) {
        SocketBase::connect_client();
    }
    else {
        throw SocketException("invalid_service_type");
    }
}

/**
 *
 * @param
 *
 * @return void
 */
void TcpSocket::send(const std::string& message) {
    if(m_socket_tcp == -1) {
        throw SocketException("socket_not_established");
    }
    
    size_t n = write(m_socket_tcp, message.c_str(), message.size());
    
    if(n == -1) {
        throw SocketException("write_failed: %s", strerror(errno));
    }
}

/**
 *
 *
 * @return std::string
 */
std::string TcpSocket::receive() {
    char buffer[ m_buf_size ];
    
    m_socket_tcp = accept(m_socket, (struct sockaddr*)&m_sockaddr, &m_sockaddr_in_size);
    
    if(m_socket_tcp == -1) {
        throw SocketException("accept_failed: %s", strerror(errno));
    }
    
    memset(buffer, 0, sizeof(buffer));
    
    if(read(m_socket_tcp, buffer, sizeof(buffer)) == -1) {
        throw SocketException("read_failed: %s", strerror(errno));
    }
    
    return std::string(buffer, sizeof(buffer));
}


