# Eggs.Assert
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

function(add_example name)
    add_executable(eggs.assert.example.${name} ${name}.cpp)
    target_link_libraries(
        eggs.assert.example.${name}
        PRIVATE ${ARGN} Eggs::Assert::SuppressAbortDialog
    )
    _eggs_assert_setup_shared(eggs.assert.example.${name})

    if(BUILD_TESTING)
        add_test(
            NAME eggs.assert.example.${name}
            COMMAND
                ${CMAKE_COMMAND}
                -DCMD=$<TARGET_FILE:eggs.assert.example.${name}>
                "-DWILL_CRASH=$<CONFIG:Debug>" -P
                "${PROJECT_SOURCE_DIR}/support/run_verify.cmake"
        )
        set_tests_properties(
            eggs.assert.example.${name}
            PROPERTIES LABELS "example"
        )
    endif()
endfunction()

add_example(basic Eggs::Assert)
add_example(intercept Eggs::Assert::Intercept)
