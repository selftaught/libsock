#ifndef __sock_enums_hpp
#define __sock_enums_hpp

#include <sys/socket.h>

namespace Libsock {
    enum SERVICE_TYPE {
        UNDEF,
        CLIENT,
        SERVER
    };

    enum SOCK_TYPE {
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM,
        RAW = SOCK_RAW,
        SEQ = SOCK_SEQPACKET,
        RDM = SOCK_RDM
    };
};

#endif
