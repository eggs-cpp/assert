// Eggs.Assert
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Verify that expressions containing commas (e.g. template arguments) compile
// and evaluate correctly through the variadic macro.

#include <eggs/assert.h>
#include <eggs/assert.hpp>

#include <utility>

int main()
{
    assert(std::pair<int, int>(1, 2) == std::pair<int, int>(1, 2));
    EGGS_ASSERT(std::pair<int, int>(1, 2) == std::pair<int, int>(1, 2));
}
