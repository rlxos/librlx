#ifndef __RLX_STRING__
#define __RLX_STRING__

#include <string>
#include <unistd.h>

namespace rlx::utils::string
{
    static const char alnum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    inline bool ends_with(std::string const &value, std::string const &ending)
    {
        if (ending.size() > value.size())
            return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    inline std::string random(int const len)
    {
        std::string str;

        srand((unsigned)time(nullptr) * getpid());
        str.reserve(len);
        for (int i = 0; i < len; i++)
            str += alnum[rand() % (sizeof(alnum) - 1)];

        return str;
    }
}

#endif