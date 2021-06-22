#ifndef __RLX_IO__
#define __RLX_IO__

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

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

    inline std::string readfile(std::string path)
    {
        std::ifstream file(path);
        std::string input(
            (std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>()));

        return input;
    }

    template <typename T>
    inline void writefile(std::string path, T content)
    {
        std::ofstream file(path);
        file << content;
        file.flush();
    }

    template <typename T>
    inline std::string input(T &in, std::string mesg = "")
    {
        std::string l;
        io::print(mesg, " >> ");
        std::getline(in, l);

        return l;
    }

    enum class debug_level : int
    {
        none,
        warn,
        error,
        debug,
        trace,
    };

    template <typename... Args>
    void debug(debug_level lev, Args &&...args)
    {
        debug_level current = debug_level::none;
        auto l = std::getenv("DEBUG");
        if (l != nullptr)
        {
            switch (l[0])
            {
            case 'N':
            case 'n':
                current = debug_level::none;
                break;

            case 'w':
            case 'W':
                current = debug_level::warn;
                break;

            case 'e':
            case 'E':
                current = debug_level::error;
                break;

            case 'd':
            case 'D':
                current = debug_level::debug;
                break;

            case 't':
            case 'T':
                current = debug_level::trace;
                break;
            }
        }

        if (current >= lev)
            log(args...);
    }

}

#endif