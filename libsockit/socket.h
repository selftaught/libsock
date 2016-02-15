
#ifndef socket_h
#define socket_h

/**
 * Default recv char array lengths
 * for the UDP and TCP protocols.
 */
#define UDP_RECV_BUF_LEN 576
#define TCP_RECV_BUF_LEN 1500

#define IPPROTO(TYPE) ((TYPE) == (SOCK_STREAM) ? (IPPROTO_TCP) : (IPPROTO_UDP))

enum SERVICE_TYPE {
    UNDEF,
    CLIENT,
    SERVER
};

/**
 * SocketException class
 *
 * @credits: http://stackoverflow.com/questions/8152720/correct-way-to-inherit-from-stdexception
 */
class SocketException : public std::exception {
public:
    /** 
     * Constructor (C strings).
     * @param message C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the \c char* lies
     *                 with the caller.
     */
    explicit SocketException(const char* message):
        msg_(message)
    {}
    
    /** 
     * Constructor (C++ STL strings).
     * @param message The error message.
     */
    explicit SocketException(const std::string& message):
        msg_(message)
    {}
    
    /** 
     * Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~SocketException() throw() {}
    
    /** 
     * Returns a pointer to the (constant) error description.
     * @return A pointer to a \c const \c char*. The underlying memory
     *          is in posession of the \c Exception object. Callers \a must
     *          not attempt to free the memory.
     */
    virtual const char* what() throw() {
        return msg_.c_str();
    }
    
protected:
    /**
     * Error message.
     */
    std::string msg_;
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
    uint16_t m_buf_size;
    
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
     * Service type (SYSTEM_TYPE::CLIENT or SYSTEM_TYPE::SERVER)
     */
    SERVICE_TYPE m_service_type;
    
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
    SocketBase(const int& type, uint16_t buf_size):
        m_port(0),
        m_socket(-1),
        m_af(AF_INET),
        m_type(type),
        m_backlog(5),
        m_buf_size(buf_size),
        m_sockaddr_in_size(sizeof(struct sockaddr_in)),
        m_service_type(SERVER)
    { }
    
    SocketBase(const uint16_t& port, const int& type, uint16_t buf_size):
        m_port(port),
        m_socket(-1),
        m_af(AF_INET),
        m_type(type),
        m_backlog(5),
        m_buf_size(buf_size),
        m_sockaddr_in_size(sizeof(struct sockaddr_in)),
        m_service_type(SERVER)
    { }
    
    SocketBase(const std::string& hostname, const uint16_t& port, const int& type, uint16_t buf_size):
        m_hostname(hostname),
        m_port(port),
        m_socket(-1),
        m_af(AF_INET),
        m_type(type),
        m_backlog(5),
        m_buf_size(buf_size),
        m_sockaddr_in_size(sizeof(struct sockaddr_in)),
        m_service_type(CLIENT)
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
    
    /**
     * Explicitly declare the service type (CLIENT or SERVER)
     * despite the fact the service type get's intelligently
     * set in the constructor.
     */
    void set_service_type(const SERVICE_TYPE& type) {
        m_service_type = type;
    }
};

#endif
