#pragma once

#include <string>
#include <filesystem>
#include "../io.hh"
#include "string.hh"

namespace rlx::utils::sys
{
    inline std::string tempdir(std::string prefix, std::string dir)
    {
        dir = prefix + "/" + dir + "-" + string::random(5);
        std::error_code ec;
        std::filesystem::create_directories(dir, ec);
        if (ec)
            throw std::runtime_error(ec.message() + " " + dir);

        return dir;
    }

    inline std::string tempfile(std::string prefix, std::string name)
    {
        name = prefix + "/" + name + "-" + string::random(5);
        return name;
    }
}