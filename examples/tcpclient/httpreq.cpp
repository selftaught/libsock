#include <iostream>
#include <protocols/tcp.hpp>

void http_request(const std::string&);

int main(int argc, char ** argv) {
    int opt = 0;
    std::string host;

    while((opt = getopt(argc, argv, "h:p:")) != -1) {
        std::string optarg_str;

        if(optarg != NULL) {
            optarg_str = optarg;
        }

        switch(opt) {
            case 'h': {
                host = optarg_str;
                break;
            }
        }
    }

    if (host.empty()) {
        std::cerr << "Specify a host using the -h option." << std::endl;
        return EXIT_FAILURE;
    }

    http_request(host);

    return EXIT_SUCCESS;
}

void http_request(const std::string& _host) {
    TCPClient client(_host, 80);

    try {
        client.connect();
    }
    catch(const Libsock::SockException& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    while(true) {
        int e = poll(client.handle(), 1, 500);

        switch(e) {
            case POLL_EXPIRE: break;
            default: {
                try {
                    std::string received = client.receive();

                    if(!received.empty()) {
                        std::cout << "received " << received.length() << " bytes\n";
                        std::cout << "message: \n\n" << received << std::endl;
                    }
                }
                catch(const Libsock::SockException& e) {
                    std::cerr << e.what() << std::endl;
                    break;
                }
            }
        }
    }
}
