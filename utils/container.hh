#pragma once

#include <vector>
#include <algorithm>

namespace rlx::utils::container
{
    template <typename Type>
    static inline void pop_front(std::vector<Type> &cont_)
    {
        if (cont_.size() == 0)
            return;

        std::reverse(cont_.begin(), cont_.end());
        cont_.pop_back();

        std::reverse(cont_.begin(), cont_.end());
    }
}