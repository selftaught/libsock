
#ifndef ____udp__
#define ____udp__

/**
 * UDP (datagram) server socket class.
 */
class UdpClientSocket : public SocketBase {
public:
    /**
     * Constructors
     */
    UdpClientSocket(const uint16_t& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
        SocketBase(port, SOCK_DGRAM, buf_size)
    { }
    
    UdpClientSocket(const std::string& hostname, const uint16_t& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
        SocketBase(hostname, port, SOCK_DGRAM, buf_size)
    { }
    
    UdpClientSocket(const std::string& port, uint16_t buf_size = 576):
        SocketBase(port, SOCK_DGRAM, buf_size)
    { }
    
    UdpClientSocket(const std::string& hostname, const std::string& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
        SocketBase(hostname, port, SOCK_DGRAM, buf_size)
    { }
    
    void connect();
};

/**
 * UDP (datagram) server socket class.
 */
class UdpServerSocket : public SocketBase {
public:
    /**
     * Constructors
     */
    UdpServerSocket(const uint16_t& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
        SocketBase(port, SOCK_DGRAM, buf_size)
    { }
    
    UdpServerSocket(const std::string& hostname, const uint16_t& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
        SocketBase(hostname, port, SOCK_DGRAM, buf_size)
    { }
    
    UdpServerSocket(const std::string& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
        SocketBase(port, SOCK_DGRAM, buf_size)
    { }
    
    UdpServerSocket(const std::string& hostname, const std::string& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
        SocketBase(hostname, port, SOCK_DGRAM, buf_size)
    { }
    
    void connect();
    void respond(const std::string&);
    
    std::string receive();
};

#endif
