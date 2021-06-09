#ifndef __RLX_TAR__
#define __RLX_TAG__

#include <string>
#include <vector>
#include "../utils/exec.hh"
#include "../algo/algo.hh"

namespace rlx::tar
{
    inline bool exists(std::string const &tarfile, std::string filepath)
    {
        return WEXITSTATUS(system(("tar -taf " + tarfile + " " + filepath).c_str())) == 0;
    }

    inline std::vector<std::string> files(std::string const &tarfile)
    {
        return algo::str::split(utils::exec::output("tar -taf " + tarfile), '\n');
    }

    inline std::string get(std::string const &tarfile, std::string filepath)
    {
        return utils::exec::output("tar -xaf " + tarfile + " " + filepath + " -O");
    }
}

#endif