
#include "main.h"

void UdpSocket::connect() {
    SocketBase::connect_server();
}

void UdpSocket::respond(const std::string& response) {
    if(m_socket == -1) {
        throw SocketException("socket_not_established");
    }
    
    ssize_t n = sendto(m_socket, response.c_str(), response.size(), 0, (struct sockaddr*)&m_cli_addr, m_sockaddr_in_size);
    
    if(n == -1) {
        throw SocketException("sendto_failed");
    }
}

std::string UdpSocket::receive() {
    char recv_buf[m_buf_size];
    bzero(recv_buf, m_buf_size);
    
    ssize_t n = recvfrom(m_socket, recv_buf, m_buf_size, 0, (struct sockaddr*)&m_cli_addr, &m_sockaddr_in_size);

    if(n == -1) {
        throw SocketException("recvfrom_failed");
    }

    return std::string(recv_buf);
}
