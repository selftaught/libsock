#include "libsock.hpp"

namespace Sock {
    TCP::TCP() {

    }

	TCP::TCP(Sock::Host host, Sock::Port port) {
		_host = host;
		_port = port;
	}

    TCP::~TCP() {

    }

    Sock::RetCode TCP::send() {
    	// TODO...
		return 0;
    }

    Sock::RetCode TCP::connect() {
    	// TODO...
    	return 0;
    }

    Sock::Data TCP::receive() {
    	Sock::Data received;
    	// TODO...
    	return received;
    }
};
