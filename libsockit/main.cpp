//
//  main.cpp
//  libsockit
//
//  Created by Dillan Hildebrand on 3/10/16.
//
//

#include "sockit.hpp"

/**
 * Signal capturing function
 */
void signal_callback_handler(int signum) {
    LOG(FATAL) << "Caught signal: " << signum;
}

int main(int argc, char** argv) {
    
    signal(SIGABRT, signal_callback_handler);
    
    Socket<UDP, SERVER>* socket = new Socket<UDP, SERVER>(10001);
    
    try {
        socket->connect();
    }
    catch(SocketException e) {
        LOG(FATAL) << e.what();
    }

    while(true) {
        
        int e = poll(socket->pfd(), 1, 0);
        
        switch(e) {
            case POLL_EXPIRE: {
                //std::cout << "Poll timed out\n";
                break;
            }
                
            case POLL_ERR: {
                //std::cout << std::strerror(errno) << std::endl;
            }
                
            default: {
                try {
                    std::string received = socket->receive();
                    
                    if(!received.empty()) {
                        LOG(INFO) << "received " << received.length() << " bytes";
                    }
                }
                catch(SocketException e) {
                    std::cout << e.what() << std::endl;
                }
            }
        }
        
        sleep(1);   
    }
    
    return EXIT_SUCCESS;
}
