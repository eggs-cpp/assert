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
    std::println(stderr, "ASSERTION FAILURE:");

    auto const trace = std::stacktrace::current(1);
    if (!trace.empty()) {
        auto const& top = trace[0];
        std::println(stderr, "- file   : {}", top.source_file());
        std::println(stderr, "- line   : {}", top.source_line());
        std::println(stderr, "- context: {}", top.description());
    }
    std::println(stderr, "- message: {}\n", message);

    std::println(stderr, "STACK TRACE:\n{}\n", trace);

    std::abort();
}
