# Eggs.Assert
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# ── Verify interface header sets ────────────────────────────────────────────

add_test(
    NAME eggs.assert.verify_headers
    COMMAND
        ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target
        _eggs_assert_verify_interface_header_sets --config $<CONFIG>
)
set_tests_properties(
    eggs.assert.verify_headers
    PROPERTIES LABELS "header" RESOURCE_LOCK cmake_build
)

# add_lib_test(<name> [WILL_CRASH <expr>] [DEBUG_ONLY])
#
# Builds <name>.cpp as a test executable linked against Eggs::Assert and
# registers it with CTest under the label "unit". The test is run via
# run_verify.cmake, which checks the exit behaviour:
#
#   WILL_CRASH <expr>   generator expression; truthy = expects abort,
#                       falsy/omitted = expects clean exit (code 0).
#
#   DEBUG_ONLY          the source #errors when NDEBUG is defined, so this
#                       flag compiles a no-op stub in non-Debug configs and
#                       restricts CTest registration to Debug only. On
#                       single-config generators the target is skipped
#                       entirely when CMAKE_BUILD_TYPE is not Debug.
function(add_lib_test name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "DEBUG_ONLY" "WILL_CRASH" "")

    # For single-config generators, skip Debug-only tests entirely when not Debug.
    if(
        ARG_DEBUG_ONLY
        AND NOT CMAKE_CONFIGURATION_TYPES
        AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug"
    )
        return()
    endif()

    add_executable(${name})
    if(ARG_DEBUG_ONLY)
        # Non-Debug configs compile a no-op stub so the source (which #errors on
        # NDEBUG) is never compiled in Release. CTest is gated via CONFIGURATIONS.
        target_sources(
            ${name}
            PRIVATE
                "$<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp>"
                "$<$<NOT:$<CONFIG:Debug>>:${CMAKE_CURRENT_FUNCTION_LIST_DIR}/empty_stub.cpp>"
        )
        set(_configurations CONFIGURATIONS Debug)
    else()
        target_sources(${name} PRIVATE "${name}.cpp")
    endif()
    target_link_libraries(
        ${name}
        PRIVATE Eggs::Assert Eggs::Assert::SuppressAbortDialog
    )

    _eggs_assert_setup_shared(${name})
    add_test(
        NAME eggs.assert.${name}
        ${_configurations}
        COMMAND
            ${CMAKE_COMMAND} -DCMD=$<TARGET_FILE:${name}>
            -DWILL_CRASH=${ARG_WILL_CRASH} -P
            "${PROJECT_SOURCE_DIR}/support/run_verify.cmake"
    )
    set_tests_properties(eggs.assert.${name} PROPERTIES LABELS "unit")
endfunction()

# ── Re-includability ──────────────────────────────────────────────────────────

add_lib_test(reinclusion_h_disable)
add_lib_test(reinclusion_h_enable WILL_CRASH 1)

add_lib_test(reinclusion_hpp_disable)
add_lib_test(reinclusion_hpp_enable WILL_CRASH 1)

# ── Pass / fail ───────────────────────────────────────────────────────────────

add_lib_test(assert_pass)
add_lib_test(assert_hpp_fail WILL_CRASH "$<CONFIG:Debug>")
add_lib_test(assert_h_fail WILL_CRASH "$<CONFIG:Debug>")

# ── Comma in expression ───────────────────────────────────────────────────────

add_lib_test(assert_comma)

# ── Single evaluation ─────────────────────────────────────────────────────────

add_lib_test(assert_single_eval DEBUG_ONLY)

# ── assert.h in C mode ────────────────────────────────────────────────────────

add_executable(assert_h_c assert_h_c.c)
set_target_properties(assert_h_c PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(
    assert_h_c
    PRIVATE Eggs::Assert Eggs::Assert::SuppressAbortDialog
)

_eggs_assert_setup_shared(assert_h_c)

add_test(
    NAME eggs.assert.assert_h_c
    COMMAND
        ${CMAKE_COMMAND} -DCMD=$<TARGET_FILE:assert_h_c>
        -DWILL_CRASH=$<CONFIG:Debug> -P
        "${PROJECT_SOURCE_DIR}/support/run_verify.cmake"
)
set_tests_properties(eggs.assert.assert_h_c PROPERTIES LABELS "unit")
