//
//  main.cpp
//  libsockit
//
//  Created by Dillan Hildebrand on 3/10/16.
//
//

#include "sockit.hpp"

int main(int argc, char** argv) {
    
    Socket<TCP, SERVER>* socket = new Socket<TCP, SERVER>(9999);
    socket->connect();

    while(true) {
        
        int e = poll(socket->pfd(), 1, 10000);
        
        switch(e) {
            case POLL_EXPIRE: {
                //std::cout << "Poll timed out\n";
                break;
            }
                
            case POLL_ERR: {
                //std::cout << std::strerror(errno) << std::endl;
                break;
            }
                
            default: {
                try {
                    std::string received = socket->receive();
                    
                    if(!received.empty()) {
                        std::cout << received << std::endl;
                    }
                }
                catch(SocketException e) {
                    std::cout << e.what() << std::endl;
                    break;
                }
            }
        }
        
        sleep(1);
    }
    
    return EXIT_SUCCESS;
}
