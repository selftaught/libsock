
#ifndef ____tcp__
#define ____tcp__

/**
 * TCP (stream) socket class.
 */
class TcpSocket : public SocketBase {
public:
    /**
     * Constructors
     *
     * Default m_recv_buf_size is because the MTU (Max Transmission Unit)
     * for ethernet is TCP_RECV_BUF_LEN bytes.
     */
    TcpSocket(const uint16_t& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
        SocketBase(port, SOCK_STREAM, buf_size)
    { }
    
    TcpSocket(const std::string& hostname, const uint16_t& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
        SocketBase(hostname, port, SOCK_STREAM, buf_size)
    { }
    
    TcpSocket(const std::string& port):
        SocketBase(port, SOCK_STREAM, TCP_RECV_BUF_LEN)
    { }
    
    TcpSocket(const std::string& hostname, const std::string& port, uint16_t buf_size = TCP_RECV_BUF_LEN):
        SocketBase(hostname, port, SOCK_STREAM, buf_size)
    { }
    
    void set_buf_size(const uint16_t& size) {
        m_buf_size = size;
    }
    
    void connect();
    void send(const std::string&);
    
    std::string receive();
};


#endif

