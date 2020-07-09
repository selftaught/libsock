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

	int Base::set_opt() {
		return 0; // TODO
	}

    int Base::bind() {
		return 0; // TODO
    }

    int Base::bind(Sock::Port port) {
    	return 0; // TODO
    }

    int Base::connect() {
    	return 0; // TODO
    }

    int Base::connect(Sock::Host host) {
    	return 0; // TODO
    }

    void Base::disconnect() {
    	// TODO
    }

    Sock::Addr* Base::from_sockaddr(struct sockaddr_in* sa) {
    	Sock::Addr* addr = nullptr;
    	return addr;
    }

    Sock::Uint Base::descriptor() {
    	return fd;
    }
};
