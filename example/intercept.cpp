// Eggs.Assert
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Demonstrates intercepting <assert.h> from third-party or stdlib code.
// Requires Eggs::Assert to be built with INTERCEPT_ASSERT_H=ON.
// No changes to the "third-party" code are needed — just the build configuration.

#include <cassert>

namespace third_party {
namespace {

void validate(int value)
{
    assert(value > 0 && "value must be positive");
}
} // namespace

void process(int value)
{
    validate(value);
}

} // namespace third_party

int main()
{
    third_party::process(-1);
}
