#ifndef __base_hpp
#define __base_hpp

namespace Sock {
    class Base {
        protected:
			// Linux dependent, protected member variables
			// and function prototypes.
#if defined(PREDEF_PLATFORM_LINUX)

#endif

			// Windows specific protected member variables
			// and member function prototypes.
#if defined(PREDEF_PLATFORM_WINDOWS)

#endif
        private:
            // Member variables.

            // Socket addr info.
            Sock::Host  _host;
            Sock::Port  _port;
            Sock::Level _level;

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
            ~Base();

            friend std::ostream& operator<< (std::ostream& out, const Base& sock) {
                return out << "[Sock::Base]:"
                           << std::endl
                           << std::endl
                           << "\t"
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
            Sock::Int  connect();
            Sock::Int  connect(Sock::Host host);
            Sock::Void disconnect();

			// Derived classes are required to implement
			// their own send and receive functions.
            virtual Sock::Void send() = 0;
            virtual Sock::Data receive() = 0;

			Sock::Addr* from_sockaddr (struct sockaddr_in* addr);
			struct sockaddr_in* to_sockaddr (Sock::Addr* addr);

            // Setters ...

            // Getters ...
            Sock::Uint fd();
            Sock::Bool bound();
    };
};

#endif
