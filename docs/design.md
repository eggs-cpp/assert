<!--
Eggs.Assert

Copyright Agustin K-ballo Berge, Fusion Fenix 2026

Distributed under the Boost Software License, Version 1.0. (See
accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-->

# Design

## Two headers, two use cases

Eggs.Assert ships two public headers that address different situations.

**`<eggs/assert.h>`** is designed to be a drop-in replacement for the
standard `<assert.h>`. It follows the same contract: no include guard,
re-includable, `NDEBUG`-aware. You can swap it in without changing call
sites. It also supports C code, not just C++.

**`<eggs/assert.hpp>`** introduces `EGGS_ASSERT` as a distinct name.
It is C++-only, requires `std::source_location`, and never touches the
standard `assert`. Use it when you want assertions that are visibly
different from the standard macro, or when you want to keep both in
the same translation unit.

## Source location

Source location is captured at the call site, not inside the failure
handler, so the information points at the assertion expression rather
than at a fixed location inside the library.

In C++ mode, `std::source_location::current()` is used. It is a
C++23 feature and the most reliable mechanism. In C mode (only
available through `assert.h`), the compiler builtins `__builtin_FILE()`,
`__builtin_LINE()`, and `__builtin_FUNCTION()` are used when available;
otherwise the preprocessor macros `__FILE__`, `__LINE__`, and `__func__`
are the fallback.

## Re-includability

`<eggs/assert.h>` has no include guard intentionally. The standard
permits `<assert.h>` to be included multiple times with `NDEBUG`
changing between inclusions to toggle assertions on or off in different
sections of a translation unit. Eggs.Assert preserves this behavior.

`<eggs/assert.hpp>` follows the same pattern: no include guard, so
`NDEBUG` changes between inclusions also affect `EGGS_ASSERT`.

## Interception

The `Eggs::Assert::Intercept` CMake target adds a directory
containing `eggs/assert.h` to the front of the system include path
under the name `assert.h`. When the compiler searches for
`<assert.h>`, it finds this copy first. The copy includes the real
system `assert.h` via `#include_next` (or a direct path on MSVC),
then redefines `assert` with the enhanced version.

This means third-party code that includes `<assert.h>` or `<cassert>`
automatically gets Eggs.Assert behavior without any source changes.

## Stacktrace

The failure handler checks for `__cpp_lib_stacktrace` at compile
time. When the C++23 stacktrace library is available it captures
`std::stacktrace::current()` and prints each entry to `stderr`. When it is not available the output
still includes the assertion expression, file, line, and function —
just without the call chain.

The CMake build detects which link library (if any) is needed for
stacktrace support on the current toolchain and links it automatically.

## Abort behavior

Assertion failure always ends with `std::abort()`. There is no
recovery path and no exception. This matches the contract of the
standard `assert`.

## Static and shared library

The runtime (`eggs_assert_failed`) is compiled into a library rather
than being header-only. This keeps the failure-path code out of every
translation unit and allows the stacktrace dependency to be isolated
in one place. The library can be built as either static (default) or
shared via `BUILD_SHARED_LIBS`.
