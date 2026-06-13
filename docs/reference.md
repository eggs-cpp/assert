<!--
Eggs.Assert

Copyright Agustin K-ballo Berge, Fusion Fenix 2026

Distributed under the Boost Software License, Version 1.0. (See
accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-->

# Reference

## Headers

### `<eggs/assert.h>`

A re-includable C/C++ header. Redefines the `assert` macro.
Compatible with C and C++.

**Behavior of `assert(expression)`:**

| Condition | Result |
|-----------|--------|
| `NDEBUG` defined | Expands to `(void)0` |
| Expression is true | No effect |
| Expression is false | Calls `eggs_assert_failed`, then `abort()` |

**Source location (selected at compile time):**

- **C++20** — `std::source_location::current()`
- **C (builtins)** — `__builtin_FILE()`, `__builtin_LINE()`,
  `__builtin_FUNCTION()`
- **C (fallback)** — `__FILE__`, `__LINE__`, `__func__`

No include guard. Safe to include multiple times with `NDEBUG`
changing between inclusions.

---

### `<eggs/assert.hpp>`

A C++-only header (requires C++20 `std::source_location`). Provides
`EGGS_ASSERT`. Does not touch the standard `assert` macro.

**Behavior of `EGGS_ASSERT(expression)`:**

| Condition | Result |
|-----------|--------|
| `NDEBUG` defined | Expands to `(void)0` |
| Expression is true | No effect |
| Expression is false | Calls `eggs_assert_failed`, then `abort()` |

Uses `std::source_location::current()` exclusively. No include guard;
safe to re-include with `NDEBUG` changing between inclusions.

---

## Runtime function (implementation detail)

```cpp
extern "C" [[noreturn]] void eggs_assert_failed(
    char const* message,
    char const* file,
    unsigned    line,
    char const* function
);
```

Called by the assertion macros when an expression evaluates to false.
This is an implementation detail — do not call it directly.

**Parameters:**

| Parameter | Description |
|-----------|-------------|
| `message` | Stringified assertion expression |
| `file` | Source file path |
| `line` | Source line number |
| `function` | Enclosing function name |

**Effect:** Writes a diagnostic to `stderr`, followed by a stacktrace
when `std::stacktrace` is available, then calls `std::abort()`. Never
returns.

**Stderr output format:**

```
ASSERTION FAILURE: <message>
- path    : <file>:<line>
- function: <function>

STACK TRACE:
<entries...>

```

The `STACK TRACE:` section is present only when
`__cpp_lib_stacktrace` is defined at compile time.

