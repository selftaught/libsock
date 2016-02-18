
#include "main.hpp"

/**
 * Send data through a TCP socket.
 *
 * @param
 *
 * @return void
 */
void TcpSocket::send(const std::string& message) {
#if defined(__NIX)
    int socket = (m_service_type == SERVER ? m_socket_tcp : m_socket);
    
    if(socket == -1) {
        throw SocketException("socket_not_established");
    }
    
    size_t n = write(socket, message.c_str(), message.size());
    
    if(n == -1) {
        throw SocketException("write_failed: %s", strerror(errno));
    }
#else

#endif
}

/**
 * Receive data from a TCP socket and return that data as a string.
 *
 * @return std::string
 */
std::string TcpSocket::receive() {
    std::string received("", m_buf_size);
	std::unique_ptr<char[ ]> buffer(new char[ m_buf_size ]);

#if defined(__NIX)
    /**
     * Fill the entire buffer with 0's.
     */
    memset(buffer.get(), 0, m_buf_size);
    
    /**
     * If the software using this lib has been declared as a SERVER.
     */
    if(m_service_type == SERVER) {
        socklen_t sock_size = sizeof(struct sockaddr_in);
        
        m_socket_tcp = accept(m_socket, (struct sockaddr*)&m_sockaddr, &sock_size);
        
        if(m_socket_tcp == -1) {
            throw SocketException("accept_failed: %s", strerror(errno));
        }
        
        if(read(m_socket_tcp, &buffer, sizeof(buffer)) == -1) {
            throw SocketException("read_failed: %s", strerror(errno));
        }
        
        received = buffer.get();
    }
    
    /**
     * If the software using this lib has been declared as a CLIENT.
     */
    else {
        ssize_t n = read(m_socket, buffer.get(), m_buf_size);
        received = buffer.get();
        
        if(n == -1) {
            throw SocketException("read_failed: %s", strerror(errno));
        }
    }
#else

#endif
    
    return received;
}


