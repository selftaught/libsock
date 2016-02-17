
#include "main.hpp"

enum TYPE {
    UDP,
    TCP
};

int main(int argc, const char * argv[]) {
    
    TYPE type = TCP;
    
    if(type == TCP) {
        TcpSocket* server = new TcpSocket("www.4chan.org", 80);
        int counter = 0;
        
        try {
            server->connect();
            server->send("GET / HTTP/1.1\r\nHost: www.4chan.org\n\r\n");
            
            while(true) {
                std::string received = server->receive();
                    
                if(received.empty()) {
                    std::cout << "empty" << std::endl;
                    counter++;
                    
                    if(counter == 10) {
                        break;
                    }
                }
                else {
                    std::cout << received << std::endl /*<< "\n------------------------------------\n" << std::endl*/;
                }
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
