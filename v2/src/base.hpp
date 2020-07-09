#ifndef __base_hpp
#define __base_hpp

namespace Sock {
    class Base {
        protected:
			// Linux dependent, protected member variables
			// and function prototypes.
#if defined(PREDEF_PLATFORM_LINUX)

#endif

			// Windows dependent, protected member variables
			// and function prototypes.
#if defined(PREDEF_PLATFORM_WINDOWS)

#endif
        private:
            // Member variables
            // Connection info
            Sock::Host  host;
            Sock::Port  port;
            Sock::Level level;

			// Note: "bound" is past tense for present "bind."
			// This bool member variable will be set to true
			// after internal scoped calls to ::bind.
			Sock::Bool  bound;

			// Socket file descriptor
			Sock::Uint  fd;

			Sock::Int   backlog;

			// client / server info structs
			struct sockaddr_in client;
			struct sockaddr_in server;

			// Linux dependent, private member variables
			// and function prototypes.
#if defined(PREDEF_PLATFORM_LINUX)

#endif

			// Windows dependent, private member variables
			// and function prototypes.
#if defined(PREDEF_PLATFORM_WINDOWS)

#endif
        public:
            Base();
            ~Base();

            // << overload for printing base socket classes
            friend std::ostream& operator<< (std::ostream& out, const Base& sock) {
                return out << "[Sock::Base]:"
                           << std::endl
                           << std::endl
                           << "\t"
                           << std::endl;
            }

			// Linux dependent, public member variables
			// and function prototypes.
#if defined(PREDEF_PLATFORM_LINUX)

#endif

			// Windows dependent, public member variables
			// and function prototypes.
#if defined(PREDEF_PLATFORM_WINDOWS)

#endif
            // Function prototypes & definitions
            // Set and get socket option(s)
            int set_opt();

            int bind();
            int bind(Sock::Port port);

            int connect();
            int connect(Sock::Host host);

            void disconnect();

			Sock::Addr* from_sockaddr (struct sockaddr_in* addr);
			struct sockaddr_in to_sockaddr (Sock::Addr* addr);
            // Public function prototypes
            // setters ...
            // getters ...
            Sock::Uint descriptor();
    };
};

#endif
