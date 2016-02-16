
#include "main.hpp"

enum TYPE {
    UDP,
    TCP
};

int main(int argc, const char * argv[]) {
    
    TYPE type = TCP;
    
    if(type == TCP) {
        TcpSocket* server = new TcpSocket("d2jsp.org", "80");
        
        try {
            server->connect();
            server->send("GET / HTTP/1.1\r\nHost: d2jsp.org\n\r\n");
            
            std::string received = server->receive();
                
            if(!received.empty()) {
                std::cout << received << std::endl;
            }
        }
        catch(SocketException e) {
            std::cout << e.what() << std::endl;
        }
        
        delete server;
    }
    else {
        UdpSocket* server = new UdpSocket(2001);
        
        try {
            server->connect();
            
            while(true) {
                std::string received = server->receive();
                
                if(!received.empty()) {
                    std::cout << received << std::endl;
                    std::cout << received.size() << std::endl << std::endl;
                    server->send("got your message");
                }
            }
        }
        catch(SocketException e) {
            std::cout << e.what() << std::endl;
        }
        
        delete server;
    }
    
    return 0;
}
