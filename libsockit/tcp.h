
#ifndef ____tcp__
#define ____tcp__

/**
 * TCP (stream) client socket class.
 */
class TcpClientSocket : public SocketBase {
public:
    /**
     * Constructors
     *
     * Default m_recv_buf_size is because the MTU (Max Transmission Unit)
     * for ethernet is TCP_RECV_BUF_LEN bytes.
     */
    TcpClientSocket(const uint16_t& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
        SocketBase(port, SOCK_STREAM, buf_size)
    { }
    
    TcpClientSocket(const std::string& hostname, const uint16_t& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
        SocketBase(hostname, port, SOCK_STREAM, buf_size)
    { }
    
    TcpClientSocket(const std::string& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
        SocketBase(port, SOCK_STREAM, buf_size)
    { }
    
    TcpClientSocket(const std::string& hostname, const std::string& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
        SocketBase(hostname, port, SOCK_STREAM, buf_size)
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
     *
     * Default m_recv_buf_size is because the MTU (Max Transmission Unit)
     * for ethernet is TCP_RECV_BUF_LEN bytes.
     */
    TcpServerSocket(const uint16_t& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
        SocketBase(port, SOCK_STREAM, buf_size)
    { }
    
    TcpServerSocket(const std::string& hostname, const uint16_t& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
        SocketBase(hostname, port, SOCK_STREAM, buf_size)
    { }
    
    TcpServerSocket(const std::string& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
        SocketBase(port, SOCK_STREAM, buf_size)
    { }
    
    TcpServerSocket(const std::string& hostname, const std::string& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
        SocketBase(hostname, port, SOCK_STREAM, buf_size)
    { }
    
    void set_recv_buf_size(const uint16_t& size) {
        m_recv_buf_size = size;
    }
    
    void connect();
    void respond(const std::string&);
    
    std::string receive();
};


#endif