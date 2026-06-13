<!--
Eggs.Assert

Copyright Agustin K-ballo Berge, Fusion Fenix 2026

Distributed under the Boost Software License, Version 1.0. (See
accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-->

# Usage

## Replacing `assert`

Include `<eggs/assert.h>` instead of `<assert.h>`. Existing
`assert(...)` call sites work without modification.

```cpp
#include <eggs/assert.h>

void validate(int value) {
    assert(value > 0 && "value must be positive");
}
```

When the assertion fails in a debug build, output like this appears
on `stderr` before `abort()` is called:

```
ASSERTION FAILURE: value > 0 && "value must be positive"
- path    : src/validation.cpp:5
- function: void validate(int)

STACK TRACE:
   0# validate(int) at src/validation.cpp:5
   1# process(int) at src/main.cpp:12
   2# main at src/main.cpp:20
```

In a release build (`NDEBUG` defined), `assert(...)` expands to
`(void)0` as usual.

---

## Using `EGGS_ASSERT`

When you want a distinct macro name that leaves the standard `assert`
untouched:

```cpp
#include <eggs/assert.hpp>

void validate(int value) {
    EGGS_ASSERT(value > 0 && "value must be positive");
}
```

`EGGS_ASSERT` behaves identically to the enhanced `assert` but is
always spelled differently, making it easy to distinguish deliberate
Eggs.Assert checks from legacy standard assertions in the same
codebase.

---

## Intercepting third-party assertions

To redirect assertions from code you cannot modify, link against
`Eggs::Assert::Intercept` instead of `Eggs::Assert`:

```cmake
target_link_libraries(my_target PRIVATE Eggs::Assert::Intercept)
```

The intercept target places a shadow `assert.h` ahead of the system
headers. Any translation unit that includes `<assert.h>` or
`<cassert>` — whether your code or a library you depend on — will use
Eggs.Assert automatically.

```cpp
// third_party/widget.cpp — unmodified
#include <assert.h>

void Widget::validate(int value) {
    assert(value >= 0);  // now routes through Eggs.Assert
}
```

---

## Handling `NDEBUG`

Eggs.Assert respects `NDEBUG` the same way the standard does. Both
macros expand to `(void)0` when `NDEBUG` is defined.

Because `<eggs/assert.h>` has no include guard, you can toggle
assertions within a translation unit by redefining `NDEBUG` and
re-including the header — the same technique the standard permits.

```c
#undef NDEBUG
#include <eggs/assert.h>
// assertions active here

#define NDEBUG
#include <eggs/assert.h>
// assertions inactive here
```
