#include "libsock.hpp"

namespace Sock {
    Raw::Raw() {

    }

    Raw::~Raw() {

    }

    Sock::RetCode Raw::send() {
    	// TODO...
		return 0;
    }

    Sock::RetCode Raw::connect() {
    	// TODO...
    	_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

    	if (_fd < 0) {
    		std::string err("call to socket(AF_INET, SOCK_RAW, IPPROTO_UDP) failed");
    		DEBUG_STDERR(err.c_str());
    		throw err;
    	}

    	return 0;
    }

	Sock::RetCode Raw::connect(Sock::Addr* addr) {
		return 0;
	}

    Sock::Data Raw::receive() {
    	Sock::Data received;
    	// TODO...
    	return received;
    }
};
