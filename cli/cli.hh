#ifndef __RLX_APP__
#define __RLX_APP__

#include <yaml-cpp/yaml.h>

#include <string>
#include <ostream>
#include <vector>
#include <numeric>
#include <iostream>
#include <map>
#include <cstring>
#include <algorithm>
#include <filesystem>

#include "../utils/utils.hh"

namespace rlx::cli
{

    using std::string;
    class app;

    class context
    {
    public:
        using fn_t = std::function<int(context const &)>;

    private:
        YAML::Node _config;
        std::vector<std::string> _args;
        std::vector<std::string> _flags;

        using fns = std::map<std::string, fn_t>;

        using map_t = std::map<std::string, std::string>;
        map_t _values;
        fns _fn_s;

        app *_app;

    public:
        context(YAML::Node const &c,
                std::vector<std::string> const &a,
                std::vector<std::string> const &f,
                std::map<std::string, std::string> const &v,
                fns const &_f,
                app *_app)
            : _config(c),
              _args(a),
              _flags(f),
              _values(v),
              _fn_s(_f),
              _app(_app)
        {
        }

        DEFINE_GET_METHOD(YAML::Node, config);
        DEFINE_GET_METHOD(std::vector<std::string>, args);
        DEFINE_GET_METHOD(std::vector<std::string>, flags);
        DEFINE_GET_METHOD(map_t, values);
        DEFINE_GET_METHOD(fns, fn_s);

        cli::app const *getapp() const
        {
            return _app;
        }

        bool const checkflag(std::string f) const
        {
            return std::find(_flags.begin(), _flags.end(), f) != _flags.end();
        }

        std::string const value(std::string v, std::string default_ = "") const
        {
            auto val = _values.find(v);
            if (val == _values.end())
                return default_;

            return val->second;
        }

        int exec(string const &i, context const & cc)
        {
            return _fn_s[i](cc);
        }
    };

    class author
    {
    private:
        std::string _email;
        std::string _name;

    public:
        author(char const *author_name)
            : _name(author_name)
        {
        }

        static author create(char const *name)
        {
            return author(name);
        }

        DEFINE_SELF_RETURNING_GET_SET_METHOD(author, email, std::string);

        DEFINE_GET_METHOD(std::string, name);

        friend std::ostream &operator<<(std::ostream &os, author const &a)
        {
            os << a._name << " <" << a._email << ">";
            return os;
        }
    };

    class arg
    {
    private:
        char _short_id;
        std::string _long_id, _about;
        bool _required = false;
        context::fn_t _fn;
        std::string _id;

    public:
        arg(std::string i)
            : _id(i)
        {
        }

        static arg create(std::string i)
        {
            return arg(i);
        }

        DEFINE_GET_METHOD(std::string, id);

        DEFINE_SELF_RETURNING_GET_SET_METHOD(arg, short_id, char);

        DEFINE_SELF_RETURNING_GET_SET_METHOD(arg, long_id, std::string);

        DEFINE_SELF_RETURNING_GET_SET_METHOD(arg, about, std::string);

        DEFINE_SELF_RETURNING_GET_SET_METHOD(arg, required, bool);

        DEFINE_SELF_RETURNING_GET_SET_METHOD(arg, fn, context::fn_t);

        bool match(std::string const &l) const
        {
            return l == _long_id ||
                   l == std::string(1, _short_id);
        }
    };

    /** @brief 
     * 
     */
    class app
    {
    private:
        std::string _id,
            _version,
            _about,
            _usage;

        context::fn_t _fn;

        std::vector<cli::author> _authors;
        std::vector<cli::arg> _args;
        std::vector<cli::app> _subs;

        std::vector<std::string> _cmd_args;
        std::vector<std::string> _flags;

        std::map<std::string, std::string> _values;

        std::map<std::string, context::fn_t> _fns;

        YAML::Node _config;

        bool const is_sub(string id) const
        {
            for (auto const &i : _subs)
                if (i.id() == id)
                    return true;

            return false;
        }

        bool const is_flag(string id) const
        {
            for (auto const &i : _args)
                if (i.id() == id)
                    return true;

            return false;
        }

    public:
        app(std::string appid)
            : _id(appid)
        {
        }

        static app create(std::string appid)
        {
            return app(appid);
        }

        DEFINE_GET_METHOD(std::string, id);

        DEFINE_SELF_RETURNING_GET_SET_METHOD(app, version, std::string);

        DEFINE_SELF_RETURNING_GET_SET_METHOD(app, about, std::string);

        DEFINE_SELF_RETURNING_GET_SET_METHOD(app, usage, std::string);

        DEFINE_SELF_RETURNING_GET_SET_METHOD(app, fn, context::fn_t);

        DEFINE_SELF_RETURNING_GET_SET_METHOD_PUSH(app, author, cli::author);

        app &arg(cli::arg a)
        {
            _args.push_back(a);
            _fns[a.id()] = a.fn();
            return *this;
        }

        app &sub(cli::app a)
        {
            _subs.push_back(a);
            _fns[a.id()] = a.fn();
            return *this;
        }

        std::vector<cli::arg> const &arg() const
        {
            return _args;
        }

        std::vector<cli::app> const &sub() const
        {
            return _subs;
        }

        app &config(std::vector<std::string> config_paths)
        {
            for (auto const &i : config_paths)
            {
                if (std::filesystem::exists(i))
                {
                    _config = YAML::LoadFile(i);
                    return *this;
                }
            }
            return *this;
        }

        app &args(int ac, char **av)
        {
            auto i = 1;
            while (i < ac)
            {
                std::string arg_(av[i]);
                if (arg_.length() > 1 && arg_[0] == '-')
                {
                    arg_ = arg_.substr(1, arg_.length() - 1);
                    if (arg_.length() > 1 && arg_[0] == '-')
                        arg_ = arg_.substr(1, arg_.length() - 1);

                    bool matched = false;
                    for (auto const &flg : _args)
                    {
                        if (flg.match(arg_))
                        {
                            matched = true;
                            if (flg.required() == true)
                                if (i + 1 < ac)
                                    _values.insert(std::make_pair(flg.id(), std::string(av[++i])));
                                else
                                {
                                    std::cout << "Error: require " + flg.id() + " value";
                                    exit(EXIT_FAILURE);
                                }
                            else
                                _flags.push_back(flg.id());
                        }
                    }
                }
                else
                    _cmd_args.push_back(arg_);

                i++;
            }

            return *this;
        }

        int exec(std::string task = "main")
        {
            _fns["main"] = _fn;
            auto args_ = _cmd_args;
            context::fn_t fnptr = nullptr;

            if (_cmd_args.size() == 0 && _fns.find("main") != _fns.end())
            {
                fnptr = _fns["main"];
            }

            else if (_cmd_args.size() != 0 && is_sub(_cmd_args[0]))
            {
                utils::container::pop_front(args_);
                fnptr = _fns[_cmd_args[0]];
            }

            else if (is_flag(_flags[0]))
            {
                utils::container::pop_front(args_);
                fnptr = _fns[_flags[0]];
            }

            auto cc = context(_config, args_, _flags, _values, _fns, this);
            if (fnptr == nullptr)
            {
                io::error("invalid task ", task);
                io::println(*this);
                return 1;
            }

            return fnptr(cc);
        }

        friend std::ostream &operator<<(std::ostream &os, app const &app)
        {
            os << app.id() << "-" << app.version() << " : " << app.about() << "\n";
            os << "\nUsage: " << app.id() << " " << app.usage() << "\n";

            auto spacing = 20;

            if (app.sub().size())
            {
                os << "\nSub:\n";
                for (auto const &a : app.sub())
                    os << "  " << a._id << std::string(spacing - (a.id().size()), ' ') << a.about() << "\n";
            }

            if (app._args.size())
            {
                os << "\nArgs:\n";
                for (auto const &a : app._args)
                    os << "  --" << a.id() << std::string(spacing - (a.id().size() + 2), ' ') << a.about() << "\n";
            }

            return os;
        }
    };
}

#endif