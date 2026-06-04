// Eggs.Assert
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef NDEBUG

#    define EGGS_ASSERT(...) ((void)0)

#else

#    include <eggs/assert-export.h>

#    include <source_location>

extern "C" [[noreturn]] EGGS_ASSERT_EXPORT void eggs_assert_failed(
    char const* message, char const* file, unsigned line, char const* function
);

#    define EGGS_ASSERT(...)                                                  \
        ((__VA_ARGS__)                                                        \
             ? (void)0                                                        \
             : ::eggs_assert_failed(                                          \
                   #__VA_ARGS__, std::source_location::current().file_name(), \
                   (unsigned)std::source_location::current().line(),          \
                   std::source_location::current().function_name()            \
               ))

#endif // NDEBUG
