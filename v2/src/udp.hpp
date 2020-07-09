#ifndef __udp_hpp
#define __udp_hpp

namespace Sock {
    class UDP : public Base {
        protected:
        private:
        public:
            UDP();
            ~UDP();

        void send();
        Sock::Data receive();
    };
}

#endif
