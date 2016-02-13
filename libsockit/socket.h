
#ifndef socket_h
#define socket_h

/**
 * Exception classes
 */
class SocketException : public std::exception { };

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

class SocketBindingFailedException : public std::exception {
public:
    const char* what() throw () {
        return "socket_binding_failed";
    }
};

class SocketAcceptFailedException : public std::exception {
public:
    const char* what() throw () {
        return "socket_accept_failed";
    }
};

class SocketReadException : public std::exception {
public:
    const char* what() throw () {
        return "socket_read_failed";
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
     * If the current system is *nix or apple
     */
#if defined(__NIX)
    
    /**
     * Socket file descriptor.
     */
    int m_socket;
    int m_socket_new;

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
    SocketBase(const int& type):
        m_port(0),
        m_socket(-1),
        m_af(AF_INET),
        m_type(type),
        m_backlog(5)
    { }
    
    SocketBase(const uint16_t& port, const int& type):
        m_port(port),
        m_socket(-1),
        m_af(AF_INET),
        m_type(type),
        m_backlog(5)
    { }
    
    SocketBase(const std::string& hostname, const uint16_t& port, const int& type):
        m_hostname(hostname),
        m_port(port),
        m_socket(-1),
        m_af(AF_INET),
        m_type(type),
        m_backlog(5)
    { }
    
    SocketBase(const std::string&, const int&);
    SocketBase(const std::string&, const std::string&, const int&);
    
    /**
     * Destructors.
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
