// Eggs.Assert
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Verify that the condition expression is evaluated exactly once.

#ifdef NDEBUG
#    error This test only makes sense in debug mode (NDEBUG must not be defined).
#endif

#include <eggs/assert.h>
#include <eggs/assert.hpp>

int main()
{
    int n = 0;
    assert(++n);
    if (n != 1) return 1;

    n = 0;
    EGGS_ASSERT(++n);
    if (n != 1) return 1;
}
