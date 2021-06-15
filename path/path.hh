#ifndef __RLX_PATH__
#define __RLX_PATH__

#include <string>
#include <fstream>
#include <libgen.h>

#include "../io.hh"

namespace rlx::path
{
    inline bool has_ext(std::string const &p, std::string const &ext)
    {
        if (p.length() == 0 || p.length() < ext.length())
            return false;

        rlx::io::debug(rlx::io::debug_level::trace, "CHECK EXT : ", p.substr(ext.length(), p.length() - ext.length()), " == ", ext);

        return p.substr(p.length() - ext.length(), ext.length()) == ext;
    }

    inline std::string basename(std::string path)
    {
        return ::basename((char *)path.c_str());
    }

    inline std::string dirname(std::string path)
    {
        return ::dirname((char *)path.c_str());
    }

    template <typename T>
    inline void writefile(std::string const &p, T const &content)
    {
        std::ofstream file(p);
        file << content;
        file.close();
    }
}

#endif