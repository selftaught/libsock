#include "libsock.hpp"

namespace Sock {
    Base::Base() {
		_bound = false;
		_host  = nullptr;
		_port  = 0;
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
    	if (_fd != 0) {
    		::close(_fd);
    	}
 #elif defined(PREDEF_PLATFORM_WINDOWS)
 		// TODO...
 #endif
    }

    Sock::Addr* Base::from_sockaddr(struct sockaddr_in* sa) {
    	Sock::Addr* addr;

    	addr = (Sock::Addr*)calloc(1, sizeof(Sock::Addr));
    	addr->ip = inet_ntoa(sa->sin_addr);
    	addr->port = ntohs(sa->sin_port);

    	return addr;
    }

    struct sockaddr_in* Base::to_sockaddr(Sock::Addr* addr) {
    	struct sockaddr_in* sa;
    	return sa;
    }

    Sock::Uint Base::fd() {
    	return _fd;
    }

    Sock::Bool Base::bound() {
    	return _bound;
    }
};
