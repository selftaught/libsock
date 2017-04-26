#ifndef __sock_raw_hpp
#define __sock_raw_hpp

#include "sock_base.hpp"

namespace Libsock {
    class SockRaw : public SockBase<RAW, UNDEF> {
        protected:
        private:
        public:
    };
};

typedef Libsock::SockRaw RawSock;

#endif
