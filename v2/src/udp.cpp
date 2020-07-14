#include "libsock.hpp"

namespace Sock {
    UDP::UDP() {
    }

    UDP::~UDP() {

    }

    Sock::RetCode UDP::send() {
		return 0;
    }

    Sock::Data UDP::receive() {
    	Sock::Data received;
    	return received;
    }
};
