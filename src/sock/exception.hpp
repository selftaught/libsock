#ifndef __sock_exception_hpp
#define __sock_exception_hpp

#include <exception>
#include <string>

namespace Libsock {
    /**
     * @class: SockException
     * @parent: (public) std::exception
     */
    class SockException : public std::exception {
    public:
        /**
         * @function: SockException
         * @class: SockException
         * @param: (const char*) format - message C-style string error message.
         *                                The string contents are copied upon construction.
         *                                Hence, responsibility for deleting the char* lies
         *                                with the caller.
         */
        explicit SockException(const char* format, ...) {
            va_list ap;
            va_start(ap, format);
            this->init(format, ap);
            va_end(ap);
        }

        /**
         * @function: SockException
         * @class: SockException
         * @param: (const std::string&) message - exception message.
         */
        explicit SockException(const std::string& message):
            msg_(message)
        {}

        /**
         * @function: ~SockException
         * @class: SockException
         */
        virtual ~SockException() throw() {}

        /**
         * Need to use std::exception::what otherwise
         * g++ will generate these warnings: 'hides overloaded virtual function'
         */
        using std::exception::what;

        /**
         * @function: what
         * @class: SockException
         * @return: const char*
         */
        virtual const char* what() throw() {
            return msg_.c_str();
        }

    protected:
        /**
         * Error message.
         */
        std::string msg_;

        /**
         * @function: init
         * @class: SockException
         * @param: (const char*) format
         * @param: (va_list) ap
         */
        void init(const char* fmt, va_list ap) {
            char errbuf[ 256 ];
            vsnprintf(errbuf, sizeof(errbuf), fmt, ap);
            msg_ = errbuf;
        }
    };
};

#endif

