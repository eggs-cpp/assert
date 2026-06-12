# Eggs.Assert
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Runs CMD and verifies its exit behaviour:
#   WILL_CRASH=1  -> expects a non-zero / abort exit (Debug)
#   WILL_CRASH=0  -> expects a clean exit code 0    (Release)
execute_process(
    COMMAND "${CMD}"
    ECHO_OUTPUT_VARIABLE
    ECHO_ERROR_VARIABLE
    RESULT_VARIABLE result
)
if(WILL_CRASH)
    if(result EQUAL 0)
        message(FATAL_ERROR "Expected abort but process exited 0")
    endif()
else()
    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Expected success but process exited: ${result}")
    endif()
endif()
