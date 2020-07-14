#ifndef __raw_hpp
#define __raw_hpp

namespace Sock {
    class Raw : public Base {
		protected:
		private:
		public:
			Raw();
			~Raw();

        Sock::RetCode send();
        Sock::RetCode connect();
        Sock::RetCode connect(Sock::Addr* addr = nullptr);

        Sock::Data receive();
    };
}

#endif