
#include "../main.hpp"

void tcp_server_example(const uint16_t& port) {
    Socket<TCP, SERVER> socket(port);
    
    try {
        socket.connect();
    }
    catch(SocketException e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    
    while(true) {
        int e = poll(socket.pfd(), 1, 0);
        
        switch(e) {
            case POLL_EXPIRE: {
                break;
            }
                
            case POLLERR: {
                
            }
                
            default: {
                try {
                    std::string received = socket.receive();
                    
                    if(!received.empty()) {
                        std::cout << "received " << received.length() << " bytes\n";
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
