// Eggs.Assert
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

// Suppress the MSVC crash dialog via global initialization so CI tests don't
// hang on unexpected crashes.
#ifdef _MSC_VER

#    include <stdlib.h>

// Register via the MSVC CRT initializer table
// (.CRT$XCU is walked by _initterm before main).
static void eggs_assert_suppress_abort_dialog(void)
{
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
}

#    pragma section(".CRT$XCU", read)
#    ifdef __clang__
__attribute__((used)) // prevents clang-cl from dead-stripping the entry
#    endif
__declspec(allocate(
    ".CRT$XCU"
)) static void (*eggs_assert_suppress_abort_dialog_)(void) =
    eggs_assert_suppress_abort_dialog;

#endif
