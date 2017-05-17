#include <iostream>
#include <protocols/tcp.hpp>

void http_request(const std::string&, const std::string&);

int main(int argc, char ** argv) {
    int opt = 0;
    std::string host;
    std::string message;

    while((opt = getopt(argc, argv, "m:h:p:")) != -1) {
        std::string optarg_str;

        if(optarg != NULL) {
            optarg_str = optarg;
        }

        switch(opt) {
            case 'h': {
                host = optarg_str;
                break;
            }
            case 'm': {
                message = optarg_str;
                break;
            }
        }
    }

    if (host.empty()) {
        std::cerr << "Specify a host using the -h option." << std::endl;
        return EXIT_FAILURE;
    }

    if (message.empty()) {
        std::cerr << "Provide a message to send using -m <message>" << std::endl;
        return EXIT_FAILURE;
    }

    http_request(host, message);

    return EXIT_SUCCESS;
}

void http_request(const std::string& _host, const std::string& _message) {
    TCPClient client(_host, 80);

    try {
        client.connect();
        client.send(_message);
        std::string recvd = client.receive();
        std::cout << "----------------------------------------"
                  << "client received:                        "
                  << "                                        "
                  << recvd
                  << std::endl;
    }
    catch(const Libsock::SockException& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}
