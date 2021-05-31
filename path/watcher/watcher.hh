#pragma once

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>

#include "../../utils/utils.hh"

namespace rlx::path
{
    class watcher
    {
    private:
        std::unordered_map<std::string, std::filesystem::file_time_type> _store;
        std::vector<std::string> _paths;
        using duration = std::chrono::duration<int, std::milli>;

        duration _delay = std::chrono::milliseconds(5000);

    public:
        enum status
        {
            CREATED,
            MODIFIED,
            REMOVED,
        };

        watcher()
        {
        }

        static watcher create()
        {
            return watcher();
        }

        DEFINE_SELF_RETURNING_GET_SET_METHOD_PUSH(watcher, path, std::string);

        //DEFINE_SELF_RETURNING_GET_SET_METHOD(watcher, delay, duration);

        int start(std::function<void(std::string, status)> action)
        {

            // Store data here
            for (auto const &p : _paths)
                for (auto const &f : std::filesystem::recursive_directory_iterator(p))
                    _store[f.path().string()] = std::filesystem::last_write_time(f);
                
                    

            // start loop
            while (true)
            {
                // check for deleted
                std::this_thread::sleep_for(_delay);

                auto iter = _store.begin();
                while (iter != _store.end())
                {
                    if (!std::filesystem::exists(iter->first))
                    {
                        action(iter->first, status::REMOVED);
                        iter = _store.erase(iter);
                    }
                    else
                    {
                        iter++;
                    }
                }

                for (auto const &p : _paths)
                {
                    for (auto &f : std::filesystem::recursive_directory_iterator(p))
                    {
                        auto cur_status = std::filesystem::last_write_time(f);
                        if (_store.find(f.path().string()) == _store.end())
                        {
                            _store[f.path().string()] = cur_status;
                            action(f.path().string(), status::CREATED);
                        }
                        else
                        {
                            if (_store[f.path().string()] != cur_status)
                            {
                                _store[f.path().string()] = cur_status;
                                action(f.path().string(), status::MODIFIED);
                            }
                        }
                    }
                }
            }

            return 1;
        }
    };
}