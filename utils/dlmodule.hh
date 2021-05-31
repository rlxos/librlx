#ifndef __DLMODULE__
#define __DLMODULE__

#include <dlfcn.h>
#include <string>
#include <stdexcept>
#include <sstream>
#include <filesystem>

namespace rlx::utils::dlmodule
{
    template <typename Type>
    inline Type load(std::string library, std::string fn)
    {
        void *handler = dlopen(library.c_str(), RTLD_LAZY);
        if (handler == nullptr)
            throw std::runtime_error(dlerror());

        Type fn_ptr = (Type)dlsym(handler, fn.c_str());
        if (fn_ptr == nullptr)
            throw std::runtime_error(dlerror());

        return fn_ptr;
    }

    inline std::string search(std::string modid, std::string paths, std::string env)
    {
        char const *env_path = getenv(env.c_str());
        if (env_path != nullptr)
            paths += ":" + std::string(env_path);

        std::stringstream pathstream(paths);
        std::string path;

        std::string module_path;
        bool found = false;

        while (std::getline(pathstream, path, ':'))
        {
            module_path = path + "/" + modid + "/lib" + modid + ".so";
            if (std::filesystem::exists(module_path))
            {
                found = true;
                break;
            }
        }

        if (!found)
            return "";

        return module_path;
    }
}

#endif