#ifndef __ALGO__
#define __ALGO__

#include <vector>
#include <string>
#include <sstream>

namespace rlx::algo
{
    template <typename T>
    bool contains(std::vector<T> const &c, T v)
    {
        for (auto const &i : c)
            if (i == v)
                return true;
        return false;
    }

    namespace str
    {
        inline std::vector<std::string> split(std::string const &s, char del = ' ')
        {
            std::vector<std::string> ret;
            std::stringstream ss(s);
            std::string i;
            while(std::getline(ss, i, del))
                ret.push_back(i);

            return ret;
        }
    }
}

#endif