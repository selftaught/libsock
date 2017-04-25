#ifndef __webserver_h
#define __webserver_h

#include <iostream>
#include <sockit.hpp>

void help();

class WebServer : public TcpServer {
    protected:
        uint16_t m_port;
        std::string m_host;
        std::vector<std::string> m_headers;

    public:
        WebServer(const std::string& host, const uint16_t& port) : TcpServer(host, port) {};

        void run();
};

#endif
