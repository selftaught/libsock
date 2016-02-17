
#include "main.hpp"

/**
 * Send data through a TCP socket.
 *
 * @param
 *
 * @return void
 */
void TcpSocket::send(const std::string& message) {    
    int socket = (m_service_type == SERVER ? m_socket_tcp : m_socket);
    
    if(socket == -1) {
        throw SocketException("socket_not_established");
    }
    
    size_t n = write(socket, message.c_str(), message.size());
    
    if(n == -1) {
        throw SocketException("write_failed: %s", strerror(errno));
    }
}

/**
 * Receive data from a TCP socket and return that data as a string.
 *
 * @return std::string
 */
std::string TcpSocket::receive() {
    std::string received("", m_buf_size);
    char buffer[ m_buf_size ];
    
    /**
     * Fill the entire buffer with 0's.
     */
    memset(buffer, 0, sizeof(buffer));
    
    if(m_service_type == SERVER) {
        m_socket_tcp = accept(m_socket, (struct sockaddr*)&m_sockaddr, &m_sockaddr_in_size);
        
        if(m_socket_tcp == -1) {
            throw SocketException("accept_failed: %s", strerror(errno));
        }
        
        if(read(m_socket_tcp, buffer, sizeof(buffer)) == -1) {
            throw SocketException("read_failed: %s", strerror(errno));
        }
        
        received = buffer;
    }
    else {
        ssize_t n = read(m_socket, buffer, m_buf_size);
        received = buffer;
        
        if(n == -1) {
            throw SocketException("read_failed: %s", strerror(errno));
        }
    }
    
    return received;
}


