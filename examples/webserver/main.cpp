#include "webserver.hpp"

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

    WebServer server(host, port);
    server.run();

    return EXIT_SUCCESS;
}

void help() {

}
