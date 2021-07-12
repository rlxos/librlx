#ifndef __RLX_SYS_EXEC__
#define __RLX_SYS_EXEC__

#include "../rlx.hh"
#include <tuple>
#include <array>

namespace rlx::sys
{
    /**
     * @brief Execute system command
     * 
     */
    std::tuple<int, std::string> exec(std::string const &cmd)
    {
        int exitcode = 255;
        std::array<char, 1024> buffer;
        std::string output;

        io::debug(io::debug_level::trace, "Executing '" + cmd + "'");

        FILE *pipe = popen(cmd.c_str(), "r");
        if (pipe == nullptr)
            std::runtime_error("popen() failed");

        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
            output += buffer.data();

        exitcode = pclose(pipe);

        return {exitcode, output};
    }
}

#endif