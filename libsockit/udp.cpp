
#include "main.h"

void UdpClientSocket::connect() {
    SocketBase::connect_client();
}

void UdpServerSocket::connect() {
    SocketBase::connect_server();
}

void UdpServerSocket::respond(const std::string& response) {
    
}