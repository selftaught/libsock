//
//  main.cpp
//  libsockit
//
//  Created by Dillan Hildebrand on 3/10/16.
//
//

#include "sockit.hpp"

int main(int argc, char** argv) {
    
    TcpSocket socket(9999);
    socket.set_service_type(SERVER);
    socket.connect();
    
    while(true) {
        try {
            std::string received = socket.receive();
            
            if(!received.empty()) {
                std::cout << received << std::endl;
            }
        }
        catch(SocketException e) {
            std::cout << e.what() << std::endl;
            break;
        }
    }
    
    return EXIT_SUCCESS;
}
