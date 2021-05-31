#ifndef __RLX_PATH__
#define __RLX_PATH__

#include <string>

namespace rlx::path
{
    inline bool has_ext(std::string const& p, std::string const& ext)
    {
        if (p.length() == 0)
            return false;
        return p.rfind(ext, 0) == 0;
    }
}

#endif