#ifndef __tcp_hpp
#define __tcp_hpp

namespace Sock {
    class TCP : public Base {
        protected:
        private:
        public:
            TCP();
            TCP(Sock::Host host, Sock::Port port);
            ~TCP();

        Sock::RetCode send();
        Sock::RetCode connect();
        Sock::Data receive();
    };
}

#endif
