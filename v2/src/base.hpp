#ifndef __base_hpp
#define __base_hpp

namespace Sock {
    class Base {
        private:
			// Linux dependent, protected member variables
			// and function prototypes.
#if defined(PREDEF_PLATFORM_LINUX)

#endif

			// Windows specific protected member variables
			// and member function prototypes.
#if defined(PREDEF_PLATFORM_WINDOWS)

#endif
        protected:
            // Member variables.

            // Socket addr info.
            Sock::Host _host;
            Sock::Port _port;
            Sock::Type _type;

			// Note: "bound" is past tense for present "bind."
			// This bool member variable will be set to true
			// after making a call to ::bind. This is for
			// tracking state.
			Sock::Bool  _bound;
			Sock::Uint  _fd;
			Sock::Int   _backlog;

			// Client and server info structs.
			struct sockaddr_in _client;
			struct sockaddr_in _server;

			// Linux specific private member variables
			// and member function prototypes.
#if defined(PREDEF_PLATFORM_LINUX)

#endif
			// Window specific private member variables
			// and member function prototypes.
#if defined(PREDEF_PLATFORM_WINDOWS)

#endif
        public:
            Base();
            Base(Sock::Host host, Sock::Port port);
            virtual ~Base() { disconnect(); }

            friend std::ostream& operator<< (std::ostream& out, Base& sock) {
                return out << "[Sock::Base]:\n\n"
                           << "\t" << "hostname = " << (sock._host == nullptr ? "NOT SET" : sock._host) << "\n"
                           << "\t" << "port = " << (sock._port > 0 ? sock._port : 0x00) << "\n"
                           << std::endl;
            }

			// Linux specific public member variables
			// and member function prototypes.
#if defined(PREDEF_PLATFORM_LINUX)

#endif

			// Windows specific public member variables
			// and member function prototypes.
#if defined(PREDEF_PLATFORM_WINDOWS)

#endif
            // Function prototypes & definitions
            // Set and get socket option(s).
            Sock::Int  set_opt();
            Sock::Int  bind();
            Sock::Int  bind(Sock::Port port);
            Sock::Void disconnect();

			// Derived classes are required to implement
			// their own send and receive functions.
            virtual Sock::RetCode send() = 0;
            virtual Sock::RetCode connect() = 0;
            virtual Sock::Data receive() = 0;

			// helpers
			Sock::Addr* from_sockaddr (struct sockaddr_in* addr);
			struct sockaddr_in* to_sockaddr (Sock::Addr* addr);

            // Setters ...

            // Getters ...
            Sock::Uint fd();
            Sock::Bool bound();
            Sock::Host host();
            Sock::Port port();
    };
};

#endif
