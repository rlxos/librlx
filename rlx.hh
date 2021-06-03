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
        DEFINE_GET_METHOD(std::string, error);
    };
}

#endif