#ifndef __protocols_tcp_hpp
#define __protocols_tcp_hpp

#include "../sock_base.hpp"

namespace Libsock {
    namespace Protocols {
        template<SERVICE_TYPE service_t>
        class TCP : public SockBase<TCP, service_t> {
            protected:
            private:
            public:
        };
    };
};

typedef Libsock::Protocols::TCP<SERVER> TCPServer;
typedef Libsock::Protocols::TCP<CLIENT> TCPClient;

#endif
