<!--
Eggs.Assert

Copyright Agustin K-ballo Berge, Fusion Fenix 2026

Distributed under the Boost Software License, Version 1.0. (See
accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-->

# CMake Integration

## Building

```sh
cmake -S . -B build
cmake --build build
cmake --install build --prefix /usr/local
```

The project ships `CMakePresets.json` with configurations for GCC,
Clang, and MSVC. List available presets with:

```sh
cmake --list-presets
cmake --build --preset gcc-debug
```

### Options

Pass options at configure time with `-D<option>=<value>`.

| Option | Subdirectory variant | Description |
|--------|----------------------|-------------|
| **`BUILD_DOCS`** (default: `ON`) | **`EGGS_ASSERT_BUILD_DOCS`** (default: `OFF`) | Build the documentation |
| **`BUILD_SHARED_LIBS`** (default: `OFF`) | **`EGGS_ASSERT_BUILD_SHARED_LIBS`** (default: `OFF`) | Build as a shared library instead of static |
| **`BUILD_EXAMPLES`** (default: `ON`) | **`EGGS_ASSERT_BUILD_EXAMPLES`** (default: `OFF`) | Build example programs |
| **`ENABLE_INSTALL`** (default: `ON`) | **`EGGS_ASSERT_ENABLE_INSTALL`** (default: `OFF`) | Generate install rules |

The prefixed variants take precedence over the short names when the
project is included via `add_subdirectory`, avoiding collisions with
the parent project's own settings.

## Consuming the library

### FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    eggs.assert
    GIT_REPOSITORY https://github.com/eggs-cpp/assert.git
    GIT_TAG        v@VERSION@
)
FetchContent_MakeAvailable(eggs.assert)

target_link_libraries(my_target PRIVATE Eggs::Assert)
```

### find_package (after installation)

```cmake
find_package(Eggs.Assert @VERSION@ REQUIRED)
target_link_libraries(my_target PRIVATE Eggs::Assert)
```

---

## Targets

### `Eggs::Assert`

The main library target. Provides `<eggs/assert.h>` and
`<eggs/assert.hpp>` as public headers and links the compiled runtime.

```cmake
target_link_libraries(my_target PRIVATE Eggs::Assert)
```

### `Eggs::Assert::Intercept`

An interface target that links `Eggs::Assert` and additionally
intercepts system `<assert.h>` / `<cassert>` includes by placing a
shadow header at the front of the include path.

```cmake
target_link_libraries(my_target PRIVATE Eggs::Assert::Intercept)
```

Adds the compile definition `EGGS_ASSERT_INTERCEPT_ASSERT_H` and a
prefixed include directory containing a shadow `assert.h`.

