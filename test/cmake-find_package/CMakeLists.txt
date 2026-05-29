# Eggs.Assert
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.25)
project(Eggs.Assert-FindPackage LANGUAGES CXX)

set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
    "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>"
)

# -------------------------------------------------------------------------
# Find the installed package exactly as an end-user would.
# -------------------------------------------------------------------------
find_package(Eggs.Assert REQUIRED)

# -------------------------------------------------------------------------
# Verification target — links against the imported target and runs.
# -------------------------------------------------------------------------
add_executable(consumer_verify main.cpp)
target_link_libraries(consumer_verify PRIVATE Eggs::Assert)

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
# Print target properties at configure time so ctest -V shows them.
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
    message(STATUS "Eggs::Assert ${_prop}: ${_val}")
endforeach()

enable_testing()
add_test(
    NAME verify
    COMMAND
        ${CMAKE_COMMAND} -DCMD=$<TARGET_FILE:consumer_verify>
        -DEXPECT_CRASH=$<CONFIG:Debug> -P
        "${CMAKE_CURRENT_SOURCE_DIR}/../run_verify.cmake"
)
