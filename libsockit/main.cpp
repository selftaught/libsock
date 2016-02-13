
#include "main.h"

int main(int argc, const char * argv[]) {
    
    TcpServerSocket* server = new TcpServerSocket("2000");
    
    try {
        server->connect();
        
        while(true) {
            std::cout << server->receive() << std::endl;
            sleep(1);
        }
    }
    catch(SocketException& e) {
        std::cout << e.what() << std::endl;
    }
    
    delete server;
    
    return 0;
}
