
#ifndef socket_h
#define socket_h

#define UDP_RECV_BUF_LEN 576
#define TCP_RECV_BUF_LEN 1500

#define IPPROTO(TYPE) ((TYPE) == (SOCK_STREAM) ? (IPPROTO_TCP) : (IPPROTO_UDP))

/**
 * Exception classes
 */
class SocketException : public std::exception {
public:
    virtual const char* what() throw() = 0;
};

class SocketPortUndefinedException : public SocketException {
public:
    const char* what() throw () {
        return "port_not_defined";
    }
};

class SocketConnectException : public SocketException {
public:
    const char* what() throw () {
        return "socket_conncetion_failed";
    }
};

class SocketBindingFailedException : public SocketException {
public:
    const char* what() throw () {
        return "socket_binding_failed";
    }
};

class SocketAcceptFailedException : public SocketException {
public:
    const char* what() throw () {
        return "socket_accept_failed";
    }
};

class SocketReadException : public SocketException {
public:
    const char* what() throw () {
        return "socket_read_failed";
    }
};

class SocketWriteException : public SocketException {
public:
    const char* what() throw () {
        return "socket_read_failed";
    }
};

class SocketNotEstablishedException : public SocketException {
public:
    const char* what() throw() {
        return "socket_not_established";
    }
};

class SocketRecvException : public SocketException {
public:
    const char* what() throw() {
        return "socket_recvfrom_failed";
    }
};

/**
 * Base socket class.
 *
 * This is the class which Socket::UDP and Socket::TCP are derived from.
 *
 */
class SocketBase {
protected:
    
    /**
     * Hostname to connect to.
     */
    std::string m_hostname;
    
    /**
     * Port number to connect on.
     */
    uint16_t m_port;
    
    /**
     *
     */
    uint16_t m_recv_buf_size;
    
    /**
     * Structure where connection info will be stored.
     */
    struct sockaddr_in m_server_addr;
    struct sockaddr_in m_cli_addr;
    
    /**
     * Address family.
     */
    int m_af;
    
    /**
     * Type (SOCK_STREAM or SOCK_DGRAM).
     */
    int m_type;

    /**
     * Maximum backlog size.
     */
    int m_backlog;
    
    /**
     * Protocol (IPPROTO_TCP or IPPROTO_UDP).
     */
    int m_protocol;
    
    /**
     *
     */
    socklen_t m_sockaddr_in_size;
    
    /**
     * If the current system is *nix or apple
     */
#if defined(__NIX)
    
    /**
     * Socket file descriptors. 
     * m_socket_tcp is only for the TCP server socket.
     */
    int m_socket;
    int m_socket_tcp;

    struct hostent* m_server;
    
    /**
     * Else the system is windows.
     */
#else
    SOCKET  m_socket;
    WSADATA m_wsa;
#endif
    
    void connect_server();
    void connect_client();
    
public:
    
    /**
     * Constructors.
     */
    SocketBase(const int& type, uint16_t recv_buf_size):
        m_port(0),
        m_socket(-1),
        m_af(AF_INET),
        m_type(type),
        m_backlog(5),
        m_recv_buf_size(recv_buf_size),
        m_sockaddr_in_size(sizeof(struct sockaddr_in))
    { }
    
    SocketBase(const uint16_t& port, const int& type, uint16_t recv_buf_size):
        m_port(port),
        m_socket(-1),
        m_af(AF_INET),
        m_type(type),
        m_backlog(5),
        m_recv_buf_size(recv_buf_size),
        m_sockaddr_in_size(sizeof(struct sockaddr_in))
    { }
    
    SocketBase(const std::string& hostname, const uint16_t& port, const int& type, uint16_t recv_buf_size):
        m_hostname(hostname),
        m_port(port),
        m_socket(-1),
        m_af(AF_INET),
        m_type(type),
        m_backlog(5),
        m_recv_buf_size(recv_buf_size),
        m_sockaddr_in_size(sizeof(struct sockaddr_in))
    { }
    
    SocketBase(const std::string&, const int&, uint16_t);
    SocketBase(const std::string&, const std::string&, const int&, uint16_t);
    
    /**
     * Destructor.
     */
    ~SocketBase();
    
    /**
     * Prototypes.
     */
    void disconnect();
    virtual void connect() = 0;
    virtual void respond(const std::string&) = 0;
    virtual std::string receive() = 0;
    
    /**
     * Setters
     */
    void set_hostname(const std::string& hostname) {
        m_hostname = hostname;
    }
    
    void set_port(const uint16_t& port) {
        m_port = port;
    }
    
    void set_address_family(const int& address_family) {
        m_af = address_family;
    }
    
    void set_type(const int& type) {
        m_type = type;
    }
    
    void set_backlog(const uint32_t& backlog) {
        m_backlog = backlog;
    }
};

#endif
