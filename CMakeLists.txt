# Eggs.Assert
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.24)
project(
    Eggs.Assert
    DESCRIPTION "A C++23 assert library with stacktrace"
    LANGUAGES C CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)

# Options (two spellings: short when top-level, prefixed when a subdirectory)
if(PROJECT_IS_TOP_LEVEL)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
    set(CMAKE_C_VISIBILITY_PRESET hidden)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

    option(BUILD_EXAMPLE "Build the example" ON)
    option(ENABLE_INSTALL "Generate install rules" ON)
else()
    option(EGGS_ASSERT_BUILD_EXAMPLE "Build the example" OFF)
    option(EGGS_ASSERT_ENABLE_INSTALL "Generate install rules" OFF)

    # Map prefixed options back to the canonical names used below
    set(BUILD_EXAMPLE ${EGGS_ASSERT_BUILD_EXAMPLE})
    set(ENABLE_INSTALL ${EGGS_ASSERT_ENABLE_INSTALL})
endif()

#
# Library target — static library.
# Internal name _eggs_assert; downstream CMake code uses the alias Eggs::Assert.
#
add_library(_eggs_assert STATIC src/assert.cpp)
target_sources(
    _eggs_assert
    PUBLIC FILE_SET HEADERS BASE_DIRS include FILES include/eggs/assert.h
)
set_target_properties(_eggs_assert PROPERTIES EXPORT_NAME Assert)
target_compile_features(_eggs_assert PUBLIC cxx_std_23)

include(Cxx23Stacktrace)
target_link_libraries(_eggs_assert PUBLIC ${EGGS_CXX23_STACKTRACE_LIB})

add_library(Eggs::Assert ALIAS _eggs_assert)

#
# Example
#
if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

#
# Install rules
#
if(ENABLE_INSTALL)
    include(CMakePackageConfigHelpers)

    install(
        TARGETS _eggs_assert
        EXPORT EggsAssertTargets
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    install(
        EXPORT EggsAssertTargets
        NAMESPACE Eggs::
        FILE eggs.assert-targets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/eggs.assert
    )

    configure_package_config_file(
        cmake/eggs.assert-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/eggs.assert-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/eggs.assert
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/eggs.assert-config-version.cmake
        VERSION 1.0.0
        COMPATIBILITY SameMajorVersion
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/eggs.assert-config.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/eggs.assert-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/eggs.assert
    )
endif()
