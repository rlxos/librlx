#ifndef __RLX_UNITTEST__
#define __RLX_UNITTEST__

#include <functional>
#include <vector>

#include "../io.hh"

#define assert(expr, mesg) \
    rlx::unittest::_assert(#expr, expr, __FILE__, __LINE__, mesg)

namespace rlx::unittest
{
    typedef std::function<void()> case_;
    static std::vector<case_> cases_list;

    inline void add(case_ const &f)
    {
        cases_list.push_back(f);
    }

    inline void _assert(char const *expr_str, bool expr, const char *file, int line, char const *mesg)
    {
        io::message(io::color::MAGENTA, "TESTING", mesg, "......");
        if (!(expr))
        {
            io::println("\t",
                        io::color::RED, "ASSERTION FAILED: ", io::color::RED,
                        io::color::BOLD, mesg, "\n\t",
                        io::color::CYAN, "Expression: ", io::color::RESET,
                        io::color::BOLD, expr_str, "\n\t",

                        io::color::CYAN, "File: ", io::color::RESET,
                        io::color::BOLD, file, io::color::RESET,

                        io::color::CYAN, " Line: ", io::color::RESET,
                        io::color::BOLD, line, io::color::RESET);

            abort();
        }
    }

    inline void run()
    {
        for (auto const &i : cases_list)
            i();
    }
}

#endif