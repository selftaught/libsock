#ifndef __raw_hpp
#define __raw_hpp

namespace Sock {
    class Raw : public Base {
		protected:
		private:
		public:
			Raw();
			~Raw();

        Sock::Void send();
        Sock::Data receive();
    };
}

#endif