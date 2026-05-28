// Eggs.Assert
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/assert.h>

#include <cstdio>
#include <cstdlib>
#include <format>
#include <utility>
#include <version>

#ifdef __cpp_lib_stacktrace
#    include <stacktrace>
#endif

namespace {

template <typename... Args>
void println(std::format_string<Args...> fmt, Args&&... args)
{
    auto s = std::format(fmt, std::forward<Args>(args)...);
    s += '\n';
    std::fputs(s.c_str(), stderr);
}

} // namespace

extern "C" [[noreturn]] void eggs_assert_failed(char const* message)
{
    println("ASSERTION FAILURE:");

#ifdef __cpp_lib_stacktrace
    auto const trace = std::stacktrace::current(1);
    if (!trace.empty()) {
        auto const& top = trace[0];
        println("- file   : {}", top.source_file());
        println("- line   : {}", top.source_line());
        println("- context: {}", top.description());
    }
    println("- message: {}\n", message);

    println("STACK TRACE:");
    for (auto const& entry : trace) {
        println("{}", std::to_string(entry));
    }
    println("");
#endif

    std::abort();
}
