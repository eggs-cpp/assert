// Eggs.Assert
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// No include guard: assert.h is intentionally designed to be re-includable so
// that the effect of NDEBUG can be changed between successive inclusions.

#ifdef assert
#    undef assert
#endif

#ifdef NDEBUG

#    define assert(...) ((void)0)

#else

#    ifdef __cplusplus
extern "C" [[noreturn]] void eggs_assert_failed(char const* message);
#    else
_Noreturn void eggs_assert_failed(char const* message);
#    endif

//! `assert` puts a diagnostic test into programs and expands to an expression
//! of type `void`. `__VA_ARGS__` is evaluated and _contextually converted to
//! `bool`_:
//! - If the evaluation yields `true`, there are no further effects.
//! - Otherwise, `assert` creates a diagnostic on the standard error stream
//!   and calls `std::abort()`.
#    define assert(...) \
        ((__VA_ARGS__) ? (void)0 : eggs_assert_failed(#__VA_ARGS__))

#endif // NDEBUG
