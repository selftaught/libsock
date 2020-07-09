#include "libsock.hpp"

namespace Sock {
    Base::Base() {
		bound = false;
		host  = nullptr;
		port  = 0;
    }

    Base::~Base() {
		disconnect();
    }

	Sock::Int Base::set_opt() {
		return 0;
	}

    Sock::Int Base::bind() {
		return 0;
    }

    Sock::Int Base::bind(Sock::Port port) {
    	return 0;
    }

    Sock::Int Base::connect() {

    	return 0;
    }

    Sock::Int Base::connect(Sock::Host host) {
    	return 0;
    }

    void Base::disconnect() {
 #ifdef PREDEF_PLATFORM_LINUX
    	if (fd != 0) {
    		::close(fd);
    	}
 #elif defined(PREDEF_PLATFORM_WINDOWS)
 		// TODO...
 #endif
    }

    Sock::Addr* Base::from_sockaddr(struct sockaddr_in* sa) {
    	Sock::Addr* addr;

    	addr = (Sock::Addr*)calloc(1, sizeof(Sock::Addr*));

    	return addr;
    }

    struct sockaddr_in* Base::to_sockaddr(Sock::Addr* addr) {
    	struct sockaddr_in* sa;
    	return sa;
    }

    Sock::Uint Base::descriptor() {
    	return fd;
    }
};
