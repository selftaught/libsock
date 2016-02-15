
#ifndef ____udp__
#define ____udp__

/**
 * UDP (datagram) socket class.
 */
class UdpSocket : public SocketBase {
public:
    /**
     * Constructors
     */
    UdpSocket(const uint16_t& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
        SocketBase(port, SOCK_DGRAM, buf_size)
    { }
    
    UdpSocket(const std::string& hostname, const uint16_t& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
        SocketBase(hostname, port, SOCK_DGRAM, buf_size)
    { }
    
    UdpSocket(const std::string& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
        SocketBase(port, SOCK_DGRAM, buf_size)
    { }
    
    UdpSocket(const std::string& hostname, const std::string& port, uint16_t buf_size = UDP_RECV_BUF_LEN):
        SocketBase(hostname, port, SOCK_DGRAM, buf_size)
    { }
    
    void connect();
    void respond(const std::string&);
    
    std::string receive();
};

#endif
