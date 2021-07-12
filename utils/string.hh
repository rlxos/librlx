#ifndef __RLX_STRING__
#define __RLX_STRING__

#include <string>
#include <unistd.h>

namespace rlx::utils::string
{
    static const char alnum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    inline bool starts_with(std::string const &val, std::string const &prefix)
    {
        return val.rfind(prefix, 0) == 0;
    }

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

    inline std::string humanize(size_t bytes)
    {
        std::string r;
        if (bytes <= 0)
            r = "0 Bytes";
        else if (bytes >= 1073741824)
            r = std::to_string(bytes / 1073741824) + " GiB";
        else if (bytes >= 1048576)
            r = std::to_string(bytes / 1048576) + " MiB";
        else if (bytes >= 1024)
            r = std::to_string(bytes / 1024) + " KiB";
        return r;
    }

    inline std::string replace_all(std::string str, const std::string &from, const std::string &to)
    {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }
}

#endif