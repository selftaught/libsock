
#ifndef ____tcp__
#define ____tcp__

/**
 * TCP (stream) client socket class.
 */
class TcpClientSocket : public SocketBase {
public:
    /**
     * Constructors
     */
    TcpClientSocket(const uint16_t& port):
        SocketBase(port, SOCK_STREAM)
    { }
    
    TcpClientSocket(const std::string& hostname, const uint16_t& port):
        SocketBase(hostname, port, SOCK_STREAM)
    { }
    
    TcpClientSocket(const std::string& port):
        SocketBase(port, SOCK_STREAM)
    { }
    
    TcpClientSocket(const std::string& hostname, const std::string& port):
        SocketBase(hostname, port, SOCK_STREAM)
    { }
    
    void connect();
};

/**
 * TCP (stream) server socket class.
 */
class TcpServerSocket : public SocketBase {
public:
    /**
     * Constructors
     */
    TcpServerSocket(const uint16_t& port):
        SocketBase(port, SOCK_STREAM)
    { }
    
    TcpServerSocket(const std::string& hostname, const uint16_t& port):
        SocketBase(hostname, port, SOCK_STREAM)
    { }
    
    TcpServerSocket(const std::string& port):
        SocketBase(port, SOCK_STREAM)
    { }
    
    TcpServerSocket(const std::string& hostname, const std::string& port):
        SocketBase(hostname, port, SOCK_STREAM)
    { }
    
    void connect();
    void respond(const std::string&);
    
    std::string receive();
};


#endif