
#include "main.h"

void TcpClientSocket::connect() {
    SocketBase::connect_client();
}

void TcpServerSocket::connect() {
    SocketBase::connect_server();
}

void TcpServerSocket::respond(const std::string& message) {
    size_t n = write(m_socket_tcp, message.c_str(), message.size());
    
    if(n == -1) {
        throw SocketWriteException();
    }
}

std::string TcpServerSocket::receive() {
    std::string received(m_recv_buf_size, 0);
    char buffer[ m_recv_buf_size ];
    
    m_socket_tcp = accept(m_socket, (struct sockaddr*)&m_cli_addr, &m_sockaddr_in_size);
    
    if(m_socket_tcp == -1) {
        throw SocketAcceptFailedException();
    }
    
    memset(buffer, 0, sizeof(buffer));
    
    if(read(m_socket_tcp, buffer, sizeof(buffer)) == -1) {
        throw SocketReadException();
    }
    
    received = std::string(buffer);
    
    return received;
}