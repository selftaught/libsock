#ifndef __protocols_udp_hpp
#define __protocols_udp_hpp

#include "../sock_base.hpp"

namespace Libsockit {
    namespace Protocols {
        template<SERVICE_TYPE service_t>
        class UDP : public SockBase<UDP, service_t> {
            protected:
            private:
            public:
        };
    };
};

typedef Libsock::Protocols::UDP<SERVER> UDPServer;
typedef Libsock::Protocols::UDP<CLIENT> UDPClient;

#endif
