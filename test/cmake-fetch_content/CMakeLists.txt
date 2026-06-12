# Eggs.Assert
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.25)
project(Eggs.Assert-FetchContent LANGUAGES CXX)

set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
    "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>"
)

# -------------------------------------------------------------------------
# EGGS_ASSERT_SOURCE_DIR is injected by the CTest fixture so that this
# consumer always tests the local working tree, not a remote tag.
# -------------------------------------------------------------------------
if(NOT DEFINED EGGS_ASSERT_SOURCE_DIR)
    message(
        FATAL_ERROR
        "EGGS_ASSERT_SOURCE_DIR must be set. "
        "Pass -DEGGS_ASSERT_SOURCE_DIR=<path> to cmake."
    )
endif()
file(TO_CMAKE_PATH "${EGGS_ASSERT_SOURCE_DIR}" EGGS_ASSERT_SOURCE_DIR)

include(FetchContent)
FetchContent_Declare(Eggs.Assert SOURCE_DIR "${EGGS_ASSERT_SOURCE_DIR}")
FetchContent_MakeAvailable(Eggs.Assert)

# -------------------------------------------------------------------------
# Verification target — links against the FetchContent target and runs.
# -------------------------------------------------------------------------
add_executable(consumer_verify main.cpp)
target_link_libraries(
    consumer_verify
    PRIVATE Eggs::Assert Eggs::Assert::SuppressAbortDialog
)

get_target_property(_assert_type Eggs::Assert TYPE)
if(_assert_type STREQUAL "SHARED_LIBRARY")
    if(WIN32)
        add_custom_command(
            TARGET consumer_verify
            POST_BUILD
            COMMAND
                ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:Eggs::Assert> $<TARGET_FILE_DIR:consumer_verify>
        )
    else()
        set_target_properties(
            consumer_verify
            PROPERTIES BUILD_RPATH "$<TARGET_FILE_DIR:Eggs::Assert>"
        )
    endif()
endif()

# -------------------------------------------------------------------------
# Confirm that the FetchContent target exposes the same properties as the
# installed one (no accidental in-tree-only paths).
# -------------------------------------------------------------------------
foreach(
    _prop
    IN
    ITEMS
        INTERFACE_INCLUDE_DIRECTORIES
        INTERFACE_COMPILE_DEFINITIONS
        INTERFACE_COMPILE_FEATURES
        INTERFACE_LINK_LIBRARIES
)
    get_target_property(_val Eggs::Assert ${_prop})
    message(STATUS "Eggs::Assert (FetchContent) ${_prop}: ${_val}")
endforeach()

enable_testing()
add_test(
    NAME verify
    COMMAND
        ${CMAKE_COMMAND} -DCMD=$<TARGET_FILE:consumer_verify>
        -DWILL_CRASH=$<CONFIG:Debug> -P
        "${CMAKE_CURRENT_SOURCE_DIR}/../../support/run_verify.cmake"
)
