// Eggs.Assert
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// No include guard: assert.h is intentionally designed to be re-includable so
// that the effect of NDEBUG can be changed between successive inclusions.

#ifdef EGGS_ASSERT_INTERCEPT_ASSERT_H
#    ifdef _MSC_VER
#        include <../ucrt/assert.h>
#    else
#        include_next <assert.h>
#    endif
#endif

#ifdef assert
#    undef assert
#endif

#ifdef NDEBUG

#    define assert(...) ((void)0)

#else

#    include <eggs/assert-export.h>

#    ifdef __cplusplus
extern "C" [[noreturn]] EGGS_ASSERT_EXPORT void eggs_assert_failed(
    char const* message, char const* file, unsigned line, char const* function
);
#    else
EGGS_ASSERT_EXPORT _Noreturn void eggs_assert_failed(
    char const* message, char const* file, unsigned line, char const* function
);
#    endif

//! `assert` puts a diagnostic test into programs and expands to an expression
//! of type `void`. `__VA_ARGS__` is evaluated and _contextually converted to
//! `bool`_:
//! - If the evaluation yields `true`, there are no further effects.
//! - Otherwise, `assert` creates a diagnostic on the standard error stream
//!   and calls `std::abort()`.

#    ifdef __cplusplus
#        include <source_location>

#        define assert(...)                                              \
            ((__VA_ARGS__)                                               \
                 ? (void)0                                               \
                 : ::eggs_assert_failed(                                 \
                       #__VA_ARGS__,                                     \
                       std::source_location::current().file_name(),      \
                       (unsigned)std::source_location::current().line(), \
                       std::source_location::current().function_name()   \
                   ))
#    elif defined(__GNUC__) || defined(__clang__)
#        define assert(...)                                             \
            ((__VA_ARGS__)                                              \
                 ? (void)0                                              \
                 : eggs_assert_failed(                                  \
                       #__VA_ARGS__, __builtin_FILE(),                  \
                       (unsigned)__builtin_LINE(), __builtin_FUNCTION() \
                   ))
#    else
#        define assert(...)                                                 \
            ((__VA_ARGS__) ? (void)0                                        \
                           : eggs_assert_failed(                            \
                                 #__VA_ARGS__, __FILE__, __LINE__, __func__ \
                             ))
#    endif

#endif // NDEBUG
