
#include "../main.hpp"

void udp_server_example(const uint16_t& port, std::string host) {
    Socket<UDP, SERVER> socket(host, port);
    
    try {
        socket.connect();
    }
    catch(SocketException e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    
    while(true) {
        int e = poll(socket.pfd(), 1, 500);

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
                        std::cout << "message: \n\n" << received << std::endl;

                        socket.send("<html><body><h1>Got it!</h1></body></html>");
                    }
                }
                catch(SocketException e) {
                    std::cerr << e.what() << std::endl;
                    break;
                }
            }
        }
    }    
}
