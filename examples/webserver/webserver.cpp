#include "webserver.hpp"

void WebServer::run() {
    try {
        connect();
    }
    catch(const Libsock::SockException& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    while(true) {
        int e = poll(handle(), 1, 500);

        switch(e) {
            case POLL_EXPIRE: break;
            default: {
                try {
                    std::string received = receive();

                    if(!received.empty()) {
                        //std::cout << "received " << received.length() << " bytes\n";
                        //std::cout << "message: \n\n" << received << std::endl;
                        send("HTTP/1.1 200 OK\r\n\r\n<html><body><h1>foobar</h1></body></html>");
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



