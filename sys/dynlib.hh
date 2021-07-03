#ifndef __SYS_DYNLIB__
#define __SYS_DYNLIB__

#include <dlfcn.h>
#include <stdexcept>
#include "../io.hh"
#include "../rlx.hh"

namespace rlx::sys
{
    using std::string;

    class dylink : rlx::obj
    {
    private:
        void *_handler;

    public:
        dylink(char const *lib, int f = RTLD_NOW)
        {
            _handler = dlopen(lib, f);
            if (_handler == nullptr)
                _error = dlerror();

            dlerror();
        }

        template <typename T>
        T lookup(char const *fn)
        {
            void *_f = dlsym(_handler, fn);
            if (_f == nullptr)
                _error = dlerror();

            return (T)(_f);
        }

        dylink(dylink const &) = delete;

        ~dylink()
        {
            if (_handler != nullptr)
                dlclose(_handler);
        }
    };
}

#endif