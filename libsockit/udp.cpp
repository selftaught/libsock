
#include "sockit.hpp"

void UdpSocket::send(const std::string& response) {
    if(m_socket == -1) {
        throw SocketException("socket_not_established");
    }
    
#if defined(__NIX)
    ssize_t n = sendto(m_socket, response.c_str(), response.size(), 0, (struct sockaddr*)&m_sockaddr, sizeof(struct sockaddr_in));

	if (n == -1) {
		throw SocketException("sendto_failed: %s", strerror(errno));
	}
#else

#endif
}

std::string UdpSocket::receive() {	
	std::string received;
	std::unique_ptr<char[]> recv_buf(new char[m_buf_size]);

	memset(recv_buf.get(), 0, m_buf_size);
    
#if defined(__NIX)
    socklen_t sock_len = sizeof(struct sockaddr_in);
    ssize_t n = recvfrom(m_socket, recv_buf.get(), m_buf_size, 0, (struct sockaddr*)&m_sockaddr, &sock_len);

    if(n == -1) {
        throw SocketException("recvfrom_failed: %s", strerror(errno));
    }
#else

#endif

	return received;
}
