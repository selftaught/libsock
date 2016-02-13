
#include "main.h"

void TcpClientSocket::connect() {
    SocketBase::connect_client();
}

void TcpServerSocket::connect() {
    SocketBase::connect_server();
}

void TcpServerSocket::respond(const std::string& message) {
    
}

std::string TcpServerSocket::receive() {
    std::string received;
    char buffer[1024];
    socklen_t sockaddr_in_size = sizeof(struct sockaddr);
    
    m_socket_new = accept(m_socket, (struct sockaddr*)&m_cli_addr, &sockaddr_in_size);
    
    if(m_socket_new == -1) {
        throw SocketAcceptFailedException();
    }
    
    memset(buffer, 0, sizeof(buffer));
    
    if(read(m_socket_new, buffer, sizeof(buffer)) == -1) {
        throw SocketReadException();
    }
    
    received = std::string(buffer);
    
    return received;
}