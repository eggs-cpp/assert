<!--
Eggs.Assert

Copyright Agustin K-ballo Berge, Fusion Fenix 2026

Distributed under the Boost Software License, Version 1.0. (See
accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-->

# Introduction

**Eggs.Assert** is a C++23 assertion library that replaces the
standard `assert` macro with one that prints a full stacktrace on
failure.

## Why

The standard C `assert` prints a short message and calls `abort()`.
That is often enough to know *that* something went wrong, but not
*how* you got there. Eggs.Assert adds a stacktrace to the failure
output so the call path is immediately visible without a debugger.

## What it provides

Three things:

- **`<eggs/assert.h>`** — A drop-in replacement for `<assert.h>`.
  Including it redefines the standard `assert` macro to call the
  Eggs.Assert failure handler instead of the C runtime one.
- **`<eggs/assert.hpp>`** — Provides `EGGS_ASSERT` under its own
  name. The standard `assert` is left untouched. Use this when you
  want an explicit macro that cannot be confused with the standard
  one.
- **`Eggs::Assert::Intercept`** — A CMake target that transparently
  intercepts `#include <assert.h>` (and `<cassert>`) in third-party
  code you cannot modify, redirecting all assertions to Eggs.Assert.

## What failure looks like

```
ASSERTION FAILURE: value > 0 && "value must be positive"
- path    : src/validation.cpp:12
- function: void third_party::validate(int)

STACK TRACE:
   0# third_party::validate(int) at src/validation.cpp:12
   1# third_party::process(int) at src/validation.cpp:18
   2# main at src/main.cpp:9
```

## Requirements

- C++23 compiler (GCC 14+, Clang 18+, MSVC 2022+)
- CMake 3.25+
