#ifndef __typedefs_hpp
#define __typedefs_hpp

namespace Sock {
    typedef char*       Host;
    typedef std::string IP;
    typedef uint16_t    Port;
    typedef int         Option;
    typedef int         Level;
    typedef bool        Bool;
    typedef intmax_t    Int;
    typedef uintmax_t   Uint;
    typedef std::string Data;
    typedef void        Void;
    typedef struct {
    	Sock::IP   ip;
    	Sock::Port port;
    } Addr;
}

#endif