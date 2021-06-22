#ifndef __RLX_EXEC__
#define __RLX_EXEC__

#include <string>
#include <vector>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>

#include <io.hh>

namespace rlx::utils::exec
{
    static inline int command(std::string cmd, std::string dir = ".", std::vector<std::string> env = std::vector<std::string>())
    {
        std::string cmd_ = "bash -euc '";
        for (auto const &i : env)
            cmd_ += " export " + i + "; ";

        if (dir != ".")
            cmd_ += " cd " + dir + "; ";

        cmd_ += cmd + "'";

        rlx::io::debug(rlx::io::debug_level::trace, "Executing '", cmd_);
        return WEXITSTATUS(system(cmd_.c_str()));
    }

    static inline std::string output(std::string cmd)
    {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe)
            throw std::runtime_error("failed to popen()");

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
            result += buffer.data();

        return result.substr(0, result.length() - 1);
    }
}

#endif