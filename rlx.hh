#ifndef __RLX__
#define __RLX__

#include "io.hh"
#include "utils/define.hh"

#include <string>
namespace rlx
{
    class obj
    {
    protected:
        std::string _error;

    public:
        class exception : std::exception
        {
            std::string _what;

        public:
            exception(std::string mesg)
                : _what(mesg)
            {
            }
            const char *what()
            {
                return _what.c_str();
            }
        };
        DEFINE_GET_METHOD(std::string, error);
    };
}

#endif