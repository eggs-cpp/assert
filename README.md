# **Eggs.Assert**

## Introduction

**Eggs.Assert** is a **C++23** assertion library with stacktrace.

Link against `Eggs::Assert` and include one of:

- `<eggs/assert.h>` — replaces the standard `assert` macro with one that prints a full stacktrace on failure.
- `<eggs/assert.hpp>` — provides `EGGS_ASSERT` under its own name, leaving the standard `assert` untouched.

To intercept `#include <assert.h>` in third-party or library code without modifying it, link against `Eggs::Assert::Intercept` instead.
This places `eggs/assert.h` ahead of the system headers so any `assert` from `<assert.h>` or `<cassert>` is transparently redirected.

---

> Copyright *Agustín Bergé*, *Fusion Fenix* 2026
>
> Distributed under the Boost Software License, Version 1.0. (See accompanying
> file LICENSE.txt or copy at <http://www.boost.org/LICENSE_1_0.txt>)
