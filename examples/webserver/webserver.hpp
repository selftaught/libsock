#ifndef __webserver_h
#define __webserver_h

#include <iostream>
#include <protocols/tcp.hpp>

void help();

class WebServer : public TCPServer {
    protected:
        uint16_t m_port;
        std::string m_host;
        std::vector<std::string> m_headers;

    public:
        WebServer(const std::string& _host, const uint16_t& _port):
            TCPServer(_host, _port)
        {};

        void run();
};

#endif
