# Eggs.Assert
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Force-includes suppress_abort_dialog.h on MSVC so test binaries don't hang
# waiting for the crash dialog when an assertion fires unexpectedly.
add_library(_eggs_assert_suppress_abort_dialog INTERFACE)
set_target_properties(
    _eggs_assert_suppress_abort_dialog
    PROPERTIES EXPORT_NAME Assert::SuppressAbortDialog
)

target_sources(
    _eggs_assert_suppress_abort_dialog
    INTERFACE
        FILE_SET HEADERS
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
            FILES
                "${CMAKE_CURRENT_SOURCE_DIR}/include/eggs/assert/support/suppress_abort_dialog.h"
)

if(MSVC)
    target_compile_options(
        _eggs_assert_suppress_abort_dialog
        INTERFACE "/FIeggs/assert/support/suppress_abort_dialog.h"
    )
endif()

add_library(
    Eggs::Assert::SuppressAbortDialog
    ALIAS _eggs_assert_suppress_abort_dialog
)

#
# Install rules
#
if(ENABLE_INSTALL)
    install(
        TARGETS _eggs_assert_suppress_abort_dialog
        EXPORT EggsAssertTargets
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()
