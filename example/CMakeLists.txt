# Eggs.Assert
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

add_executable(eggs_assert_example main.cpp)

target_link_libraries(
    eggs_assert_example
    PRIVATE Eggs::Assert Eggs::Assert::SuppressAbortDialog
)

if(BUILD_TESTING)
    get_target_property(_type _eggs_assert TYPE)
    if(_type STREQUAL "SHARED_LIBRARY")
        if(WIN32)
            add_custom_command(
                TARGET eggs_assert_example
                POST_BUILD
                COMMAND
                    ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:_eggs_assert>
                    $<TARGET_FILE_DIR:eggs_assert_example>
            )
        else()
            set_target_properties(
                eggs_assert_example
                PROPERTIES BUILD_RPATH "$<TARGET_FILE_DIR:_eggs_assert>"
            )
        endif()
    endif()

    add_test(
        NAME eggs.assert.example
        COMMAND
            ${CMAKE_COMMAND} -DCMD=$<TARGET_FILE:eggs_assert_example>
            "-DWILL_CRASH=$<CONFIG:Debug>" -P
            "${PROJECT_SOURCE_DIR}/support/run_verify.cmake"
    )
    set_tests_properties(eggs.assert.example PROPERTIES LABELS "example")
endif()
