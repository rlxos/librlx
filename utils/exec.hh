#ifndef __RLX_EXEC__
#define __RLX_EXEC__

#include <string>
#include <vector>
#include <unistd.h>

namespace rlx::utils::exec
{
    static inline int command(std::string cmd, std::string dir = ".", std::vector<std::string> env = std::vector<std::string>())
    {
        
        std::string cmd_ = "cd " + dir + "&& ";
        for (auto const &i : env)
            cmd_ += i + " ";

        return WEXITSTATUS(system((cmd_ + cmd).c_str()));
    }
}

#endif