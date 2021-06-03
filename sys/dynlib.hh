#ifndef __SYS_DYNLIB__
#define __SYS_DYNLIB__

#include <dlfcn.h>
#include <stdexcept>
#include "../io.hh"

namespace rlx::sys
{

    class dynlib_exception : std::exception
    {
        char const *_lib;

    public:
        dynlib_exception(char const *w)
            : _lib(w)
        {
        }
        char const *what() const throw()
        {
            return io::format(_lib).c_str();
        }
    };

    class dynlib
    {
    private:
        void *_handler;

    public:
        dynlib(char const *lib, int flags = RTLD_LAZY)
        {
            _handler = dlopen(lib, flags);
            if (!_handler)
                throw dynlib_exception(dlerror());
        }

        dynlib(dynlib const &src) = delete;

        virtual ~dynlib()
        {
            if (_handler)
                dlclose(_handler);
        }

        void *lookup(char const *symbol)
        {
            auto fn = dlsym(_handler, symbol);
            if (!fn)
                throw dynlib_exception(dlerror());

            return fn;
        }
    };
}

#endif