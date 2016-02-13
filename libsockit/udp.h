
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
    UdpClientSocket(const uint16_t& port):
        SocketBase(port)
    { }
    
    UdpClientSocket(const std::string& hostname, const uint16_t& port):
        SocketBase(hostname, port, SOCK_DGRAM)
    { }
    
    UdpClientSocket(const std::string& port):
        SocketBase(port, SOCK_DGRAM)
    { }
    
    UdpClientSocket(const std::string& hostname, const std::string& port):
        SocketBase(hostname, port, SOCK_DGRAM)
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
    UdpServerSocket(const uint16_t& port):
        SocketBase(port)
    { }
    
    UdpServerSocket(const std::string& hostname, const uint16_t& port):
        SocketBase(hostname, port)
    { }
    
    UdpServerSocket(const std::string& port):
        SocketBase(port, SOCK_DGRAM)
    { }
    
    UdpServerSocket(const std::string& hostname, const std::string& port):
        SocketBase(hostname, port, SOCK_DGRAM)
    { }
    
    void connect();
    
    virtual void respond(const std::string&);
};

#endif
