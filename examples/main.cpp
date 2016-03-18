
#include "main.hpp"

int main(int argc, char** argv) {

    int opt = 0;
    bool sflag, tflag, pflag = false;
    std::string service, type, host;
    uint16_t port = 0;
    
    while((opt = getopt(argc, argv, "h:s:t:p:")) != -1) {
        std::string optarg_str;

        if(optarg != NULL) {
            optarg_str = optarg;
        }

        switch(opt) {
            case 's': {
                sflag = true; 
                if(optarg_str == "server" || 
                   optarg_str == "client") { 
                    service = optarg_str;
                }
                break;
            }

            case 't': {
                tflag = true;
                if(optarg_str == "tcp" || 
                   optarg_str == "udp") { 
                    type = optarg_str;
                }
                break;
            }        

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

    if(service.empty()) {
        std::cerr << (sflag == true ? "Invalid service" : "Service undefined") 
                  << " (options are client or server)"                  
                  << std::endl; 
        return EXIT_FAILURE;
    }

    if(type.empty()) {
        std::cerr << (tflag == true ? "Invalid type" : "Type undefined") 
                  << " (options are udp or tcp)"                  
                  << std::endl; 
        return EXIT_FAILURE;
    }

    if(port == 0) {
        std::cerr << (pflag == true ? "Invalid port" : "Port undefined") 
                  << " (valid port range is 1 - 65535)"                  
                  << std::endl; 
        return EXIT_FAILURE;
    }

    if(service == "client" && host.empty()) {
        std::cerr << "You didn't provide a hostname" << std::endl;
        return EXIT_FAILURE;
    }

    if(service == "client") {
             if(type == "udp") { udp_client_example(host, port); }
        else if(type == "tcp") { tcp_client_example(host, port); }
    }
    else {
             if(type == "udp") { udp_server_example(port); }
        else if(type == "tcp") { tcp_server_example(port); }
    }

    return EXIT_SUCCESS;
}

void help() {

}
