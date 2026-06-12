# Eggs.Assert
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.25)
project(
    Eggs.Assert
    VERSION 1.0.0
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

    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
        "$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>"
    )

    option(BUILD_EXAMPLE "Build the example" ON)
    option(BUILD_SHARED_LIBS "Build as a shared library" OFF)
    include(CTest)
    option(ENABLE_INSTALL "Generate install rules" ON)
else()
    option(EGGS_ASSERT_BUILD_EXAMPLE "Build the example" OFF)
    option(
        EGGS_ASSERT_BUILD_SHARED_LIBS
        "Build as a shared library"
        ${BUILD_SHARED_LIBS}
    )
    option(EGGS_ASSERT_BUILD_TESTING "Build the test suite" OFF)
    option(EGGS_ASSERT_ENABLE_INSTALL "Generate install rules" OFF)

    # Map prefixed options back to the canonical names used below
    set(BUILD_EXAMPLE ${EGGS_ASSERT_BUILD_EXAMPLE})
    set(BUILD_SHARED_LIBS ${EGGS_ASSERT_BUILD_SHARED_LIBS})
    set(BUILD_TESTING ${EGGS_ASSERT_BUILD_TESTING})
    set(ENABLE_INSTALL ${EGGS_ASSERT_ENABLE_INSTALL})
endif()

#
# Library target — static or shared depending on BUILD_SHARED_LIBS.
# Internal name _eggs_assert; downstream CMake code uses the alias Eggs::Assert.
#
add_library(_eggs_assert src/assert.cpp)
set_target_properties(
    _eggs_assert
    PROPERTIES
        EXPORT_NAME Assert
        OUTPUT_NAME eggs_assert
        VERIFY_INTERFACE_HEADER_SETS ${BUILD_TESTING}
)
if(MSVC)
    set_target_properties(
        _eggs_assert
        PROPERTIES
            COMPILE_PDB_NAME "eggs_assert"
            COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>"
    )
endif()
target_compile_features(_eggs_assert PUBLIC cxx_std_23)

include(GenerateExportHeader)
generate_export_header(
    _eggs_assert
    BASE_NAME EGGS_ASSERT
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/eggs/assert-export.h"
)
if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(_eggs_assert PUBLIC EGGS_ASSERT_STATIC_DEFINE)
endif()

target_sources(
    _eggs_assert
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS include "${CMAKE_CURRENT_BINARY_DIR}/include"
            FILES
                include/eggs/assert.h
                include/eggs/assert.hpp
                "${CMAKE_CURRENT_BINARY_DIR}/include/eggs/assert-export.h"
)

include(Cxx23Stacktrace)
target_link_libraries(_eggs_assert PUBLIC ${EGGS_CXX23_STACKTRACE_LIB})

add_library(Eggs::Assert ALIAS _eggs_assert)

add_subdirectory(support)

#
# Example
#
if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

#
# Tests
#
if(BUILD_TESTING)
    add_subdirectory(test)
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
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    if(MSVC)
        if(BUILD_SHARED_LIBS)
            install(
                FILES "$<TARGET_PDB_FILE:_eggs_assert>"
                DESTINATION ${CMAKE_INSTALL_BINDIR}
                CONFIGURATIONS Debug RelWithDebInfo
                OPTIONAL
            )
        else()
            install(
                FILES "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/eggs_assert.pdb"
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
                CONFIGURATIONS Debug RelWithDebInfo
                OPTIONAL
            )
        endif()
    endif()

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
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/eggs.assert-config.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/eggs.assert-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/eggs.assert
    )

    set(CPACK_PACKAGE_NAME "Eggs.Assert")
    set(CPACK_PACKAGE_VENDOR "Agustin K-ballo Berge, Fusion Fenix")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
    if(WIN32)
        set(CPACK_GENERATOR ZIP)
    else()
        set(CPACK_GENERATOR TGZ)
    endif()
    include(CPack)
endif()
