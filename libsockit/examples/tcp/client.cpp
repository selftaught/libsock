
#include "../main.hpp"

void tcp_client_example() {
    Socket<TCP, CLIENT> socket(10001);
    
    try {
        socket.connect();
    }
    catch(SocketException e) {
        LOG(FATAL) << e.what();
        return;
    }
    
    while(true) {
        
        int e = poll(socket.pfd(), 1, 0);
        
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
                    std::string received = socket.receive();
                    
                    if(!received.empty()) {
                        LOG(INFO) << "received " << received.length() << " bytes";
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
}
