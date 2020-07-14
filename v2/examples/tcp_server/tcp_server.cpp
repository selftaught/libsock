#include <libsock.hpp>
#include <stdio.h>

int main (int argc, char** argv) {

	Sock::TCP tcp("localhost", 12358);
	Sock::UDP udp("/var/run/lib.sock");

	std::cout << tcp << std::endl;
	return 0;
}