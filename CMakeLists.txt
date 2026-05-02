# Eggs.Assert
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.23)

project(Eggs.Assert C CXX)

if(PROJECT_IS_TOP_LEVEL)
    option(BUILD_EXAMPLE "Build the example" ON)
    option(ENABLE_INSTALL "Enable installing the library" ON)

    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
else()
    option(EGGS_ASSERT_BUILD_EXAMPLE "Build the example" OFF)
    option(EGGS_ASSERT_ENABLE_INSTALL "Enable installing the library" OFF)

    set(BUILD_EXAMPLE ${EGGS_ASSERT_BUILD_EXAMPLE})
    set(ENABLE_INSTALL ${EGGS_ASSERT_ENABLE_INSTALL})
endif()

# Build

add_library(_eggs_assert STATIC
    src/assert.cpp)

# <stacktrace> requires linking against stdc++exp on GCC
target_link_libraries(_eggs_assert PUBLIC
    $<$<CXX_COMPILER_ID:GNU>:stdc++exp>)

target_sources(_eggs_assert PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/eggs/assert.h)

set_target_properties(_eggs_assert
    PROPERTIES
        CXX_SCAN_FOR_MODULES OFF
        EXPORT_NAME Eggs::Assert)

add_library(Eggs::Assert ALIAS _eggs_assert)

# Example

if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

# Install

include(GNUInstallDirs)

if(ENABLE_INSTALL)
    install(TARGETS _eggs_assert EXPORT _targets
        FILE_SET HEADERS)

    install(EXPORT _targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/eggs.assert
        FILE eggs.assert-config.cmake)
endif()
