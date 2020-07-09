#ifndef __tcp_hpp
#define __tcp_hpp

namespace Sock {
    class TCP : public Base {
        protected:
        private:
        public:
            TCP();
            ~TCP();

        Sock::Void send();
        Sock::Data receive();
    };
}

#endif
