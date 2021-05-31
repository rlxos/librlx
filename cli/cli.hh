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

    class app;

    class context
    {
        YAML::Node _config;
        std::vector<std::string> _args;
        std::vector<std::string> _flags;

        using map_t = std::map<std::string, std::string>;
        map_t _values;

        app *_app;

    public:
        context(YAML::Node const &c,
                std::vector<std::string> const &a,
                std::vector<std::string> const &f,
                std::map<std::string, std::string> const &v,
                app *_app)
            : _config(c),
              _args(a),
              _flags(f),
              _values(v),
              _app(_app)
        {
        }

        DEFINE_GET_METHOD(YAML::Node, config);
        DEFINE_GET_METHOD(std::vector<std::string>, args);
        DEFINE_GET_METHOD(std::vector<std::string>, flags);
        DEFINE_GET_METHOD(map_t, values);

        cli::app const *getapp() const
        {
            return _app;
        }

        bool const checkflag(std::string f) const
        {
            return std::find(_flags.begin(), _flags.end(), f) == _flags.end();
        }

        std::string const value(std::string v, std::string default_ = "") const
        {
            auto val = _values.find(v);
            if (val == _values.end())
                return default_;

            return val->second;
        }
    };

    using fn_t = std::function<int(context const &)>;

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
        fn_t _fn;
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

        DEFINE_SELF_RETURNING_GET_SET_METHOD(arg, fn, fn_t);

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

        fn_t _fn;

        std::vector<cli::author> _authors;
        std::vector<cli::arg> _args;
        std::vector<cli::app> _subs;

        std::vector<std::string> _cmd_args;
        std::vector<std::string> _flags;

        std::map<std::string, std::string> _values;

        YAML::Node _config;

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

        DEFINE_SELF_RETURNING_GET_SET_METHOD(app, fn, fn_t);

        DEFINE_SELF_RETURNING_GET_SET_METHOD_PUSH(app, author, cli::author);

        DEFINE_SELF_RETURNING_GET_SET_METHOD_PUSH(app, arg, cli::arg);

        DEFINE_SELF_RETURNING_GET_SET_METHOD_PUSH(app, sub, cli::app);

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
                                if (i + 1 > ac)
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

        int exec()
        {
            std::string task = "main";
            auto args_ = _cmd_args;

  
                // check for first argument;
                if (_cmd_args.size())
                {
                    for (auto const &i : _subs)
                    {
                        if (i.id() == _cmd_args[0] && i.fn() != nullptr)
                        {
                            utils::container::pop_front(args_);
                            auto context_ = context(_config, args_, _flags, _values, this);
                            return i.fn()(context_);
                        }
                    }
                }
                else if (_flags.size())
                {
                    for (auto const &i : _args)
                    {
                        if (i.id() == _flags[0] && i.fn() != nullptr)
                        {
                            utils::container::pop_front(args_);
                            auto context_ = context(_config, args_, _flags, _values, this);
                            return i.fn()(context_);
                        }
                    }
                }
                {

                    auto context_ = context(_config, args_, _flags, _values, this);
                    if (fn() != nullptr)
                        return fn()(context_);
                }
            

            std::cout << "invalid usage" << std::endl;
            std::cout << *this << std::endl;
            return 1;
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