#ifndef __PROGRESS_BAR__
#define __PROGRESS_BAR__

#include "../io.hh"
#include "define.hh"
#include <string>

namespace rlx::utils
{
    class progressbar
    {
    private:
        float _value;
        unsigned int _bar_width = 50;
        std::string _fill = "■", _empty = " ";
        std::string _mesg;

    public:
        DEFINE_SET_METHOD(float, value);
        DEFINE_SET_METHOD(std::string, fill);
        DEFINE_SET_METHOD(std::string, empty);

        DEFINE_SET_METHOD(std::string, mesg);

        template <typename T>
        void update(T &os, float val)
        {
            value(val);
            print(os);
        }

        template <typename T>
        void print(T &os)
        {
            using namespace rlx;
            using color = rlx::io::color;

            if (_value > 100.0f)
                return;

            io::fprint(os, "                                 \r");
            os << std::flush;

            io::fprint(os, color::BOLD, "[", color::RESET);

            auto const done = static_cast<size_t>(_value * static_cast<float>(_bar_width) / 100.0);

            for (size_t i = 0; i < _bar_width; ++i)
                if (i <= done)
                    io::fprint(os, color::BOLD, color::GREEN, _fill, color::RESET);
                else
                    io::fprint(os, _empty, color::RESET);

            io::fprint(os, color::BOLD, "]", color::RESET);

            os << " " << std::min(static_cast<size_t>(_value), size_t(100)) << "%";
            os << " " << _mesg;
        }
    };
} // namespace rlx::utils

#endif