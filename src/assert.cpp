// Eggs.Assert
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/assert-export.h>
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

extern "C" [[noreturn]] EGGS_ASSERT_EXPORT void eggs_assert_failed(
    char const* message, char const* file, unsigned line, char const* function
)
{
    println("ASSERTION FAILURE:");
    println("- file   : {}", file);
    println("- line   : {}", line);
    println("- context: {}", function);
    println("- message: {}\n", message);

#ifdef __cpp_lib_stacktrace
    println("STACK TRACE:");
    auto const trace = std::stacktrace::current(1);
    for (auto const& entry : trace) {
        println("{}", std::to_string(entry));
    }
    println("");
#endif

    std::abort();
}
