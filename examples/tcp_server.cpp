#include <iostream>
#include <sockit.hpp>

void help();
void tcp_server(const uint16_t&, std::string host = "localhost");

int main(int argc, char ** argv) {
    int opt = 0;
    bool pflag = false;
    std::string host;
    uint16_t port = 0;

    while((opt = getopt(argc, argv, "h:p:")) != -1) {
        std::string optarg_str;

        if(optarg != NULL) {
            optarg_str = optarg;
        }

        switch(opt) {
            case 'p': {
                pflag = true;
                uint16_t p = (uint16_t)std::stoi(optarg);
                if(p > 0 && p <= 65535) {
                    port = p;
                }
                break;
            }

            case 'h': {
                host = optarg_str;
                break;
            }

            default: help(); break;
        }
    }

    if(port == 0) {
        std::cerr << (pflag == true ? "Invalid port" : "Port undefined")
                  << " (valid port range is 1 - 65535)"
                  << std::endl;
        return EXIT_FAILURE;
    }

    tcp_server(port);

    return EXIT_SUCCESS;
}

void help() {

}

void tcp_server(const uint16_t& port, std::string host) {
    Socket<TCP, SERVER> socket(host, port);

    try {
        socket.connect();
    }
    catch(SocketException e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    while(true) {
        int e = poll(socket.handle(), 1, 500);

        switch(e) {
            case POLL_EXPIRE: break;
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
