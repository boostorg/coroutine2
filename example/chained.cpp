
//          Copyright Oliver Kowalke 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>

#include <boost/coroutine2/all.hpp>

template<typename coroutine>
struct test
{
    using pull_type = typename coroutine::pull_type;
    using push_type = typename coroutine::push_type;

    pull_type * child = nullptr;

    void start_child_coroutine()
    {
        child = new pull_type
        (
            [](push_type & yield)
            {
                std::cout << "2";
                yield();
                std::cout << "2";
                yield();
                std::cout << "2";
                yield();
                std::cout << "2";
                yield();
                std::cout << "2";
                yield();
                std::cout << "2";
            }
        );
    }

    pull_type start_parent_coroutine()
    {
        return pull_type
        (
            [this](push_type & yield)
            {
                std::cout << "1";
                start_child_coroutine();
                yield();
                std::cout << "1";
            }
        );
    }

    test()
    {
        auto parent = start_parent_coroutine();
        while (*child)
        {
            (*child)();
        }
        std::cout << std::endl;
    }
};


int main() {
    test<boost::coroutines2::coroutine<void>> t2;
    std::cout << "Done" << std::endl;
}
