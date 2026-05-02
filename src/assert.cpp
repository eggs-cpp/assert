// Eggs.Assert
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/assert.h>

#include <cstdlib>
#include <print>
#include <stacktrace>

extern "C" [[noreturn]] void eggs_assert_failed(char const* message)
{
    auto const trace = std::stacktrace::current();
    std::println(stderr, "Assertion `{}' failed.\n{}", message, trace);
    std::abort();
}
