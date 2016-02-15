
#include "main.h"

enum TYPE {
    UDP,
    TCP
};

int main(int argc, const char * argv[]) {
    
    TYPE type = UDP;
    
    if(type == TCP) {
        TcpServerSocket* server = new TcpServerSocket(2000);
        
        try {
            server->connect();
            
            while(true) {
                std::string received = server->receive();
                
                if(!received.empty()) {
                    std::cout << received << std::endl;
                    server->respond("<html><body>got it</body></html>");
                }
                
                sleep(1);
            }
        }
        catch(SocketException& e) {
            std::cout << e.what() << std::endl;
        }
        
        delete server;
    }
    else {
        UdpServerSocket* server = new UdpServerSocket(2001);
        
        try {
            server->connect();
            
            while(true) {
                std::string received = server->receive();
                
                if(!received.empty()) {
                    std::cout << received << std::endl;
                    std::cout << received.size() << std::endl << std::endl;
                    //server->respond("<html><body>got it</body></html>");
                }
                
                //sleep(1);
            }
        }
        catch(SocketException& e) {
            std::cout << e.what() << std::endl;
        }
        
        delete server;
    }
    
    return 0;
}
