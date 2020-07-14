#include "libsock.hpp"

namespace Sock {
    Base::Base() {
		_bound = false;
		_host  = nullptr;
		_port  = 0;
    }

    Base::Base(Sock::Host host, Sock::Port port) {
    	_host  = host;
    	_port  = port;
    	_bound = false;
    }

    Sock::Int Base::bind() {
		// TODO...
		return 0;
    }

    Sock::Int Base::bind(Sock::Port port) {
		// TODO...
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
    	struct sockaddr_in* sa = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
        sa->sin_family = AF_INET;
        sa->sin_port = htons(addr->port);
        inet_aton(addr->ip.c_str(), &(sa->sin_addr));
    	return sa;
    }

	// Getters
    Sock::Uint Base::fd() {
    	return _fd;
    }

    Sock::Bool Base::bound() {
    	return _bound;
    }

    Sock::Host Base::host() {
    	return _host;
    }

    Sock::Port Base::port() {
    	return _port;
    }

    // Setters
	Sock::Int Base::set_opt() {
		// TODO...
		return 0;
	}
};
