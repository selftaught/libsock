#ifndef __debug_hpp
#define __debug_hpp

#ifdef DEBUG
    #define DEBUG_STDERR(x) \
        (std::cout << "[ " << __FILE__ << " ] " \
                   << "(func " << __FUNCTION__ << "): " \
                   << "(line " << __LINE__ << "): " \
                   << (x) \
                   << std::endl \
        )
    #define DEBUG_STDOUT(x) \
        (std::cout << "[ " << __FILE__ << " ] " \
                   << "(func " << __FUNCTION__ << "): " \
                   << "(line " << __LINE__ << "): " \
                   << (x) \
                   << std::endl \
        )
#else
    #define DEBUG_STDERR(x)
    #define DEBUG_STDOUT(x)
#endif

#endif