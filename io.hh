#ifndef __RLX_IO__
#define __RLX_IO__

#include <iostream>
#include <sstream>
#include <string>

namespace rlx::io
{

    enum class color
    {
        RESET = 0,
        BOLD = 1,
        UNDERLINE = 4,

        BLACK = 30,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE
    };

    inline std::ostream &operator<<(std::ostream &os, color color_)
    {
        os << "\033[1;" << std::to_string((int)color_) << "m";
        return os;
    }

    template <typename... Args>
    void fprint(std::ostream &os, Args &&...args)
    {
        ((os << std::forward<Args>(args)), ...);
    }

    template <typename... Args>
    void print(Args &&...args)
    {
        return fprint(std::cout, args...);
    }

    template <typename... Args>
    void println(Args &&...args)
    {
        return fprint(std::cout, args..., "\n");
    }

    template <typename... Args>
    std::string format(Args &&...args)
    {
        std::stringstream ss;
        fprint(ss, args...);
        return ss.str();
    }

    template <typename... Args>
    void message(color c, std::string title, Args &&...args)
    {
        return fprint(std::cerr, c, title, color::RESET, " ", color::BOLD, args..., color::RESET, "\n");
    }

    template <typename... Args>
    void error(Args &&...args)
    {
        return message(color::RED, "Error!", args...);
    }

    template <typename... Args>
    void info(Args &&...args)
    {
        return message(color::CYAN, "[INFO]", args...);
    }

    template <typename... Args>
    void warn(Args &&...args)
    {
        return message(color::YELLOW, "[WARN]", args...);
    }

    template <typename... Args>
    void process(Args &&...args)
    {
        return message(color::MAGENTA, "=>", args...);
    }

    template <typename... Args>
    void success(Args &&...args)
    {
        return message(color::GREEN, "Success!", args...);
    }

    template <typename... Args>
    void log(Args &&...args)
    {
        time_t now = time(0);
        std::string dt(ctime(&now));
        return message(color::BLUE, dt.substr(0, dt.length() - 1), args...);
    }

}

#endif