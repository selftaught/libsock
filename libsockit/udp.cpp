
#include "main.h"

void UdpClientSocket::connect() {
    SocketBase::connect_client();
}

void UdpServerSocket::connect() {
    SocketBase::connect_server();
}

void UdpServerSocket::respond(const std::string& response) {
    if(m_socket == -1) {
        throw SocketNotEstablishedException();
    }
}

std::string UdpServerSocket::receive() {
    char recv_buf[ m_recv_buf_size ];
    bzero(recv_buf, m_recv_buf_size);
    
    ssize_t n = recvfrom(m_socket, recv_buf, m_recv_buf_size, 0, (struct sockaddr*)&m_cli_addr, &m_sockaddr_in_size);

    if(n == -1) {
        throw SocketRecvException();
    }

    return std::string(recv_buf);
}

