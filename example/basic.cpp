// Eggs.Assert
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/assert.h>

namespace foo::bar {
namespace {

void validate()
{
    assert(false && "invariant violated");
}
} // namespace

void process(int value, char const* name)
{
    validate();
}

} // namespace foo::bar

int main()
{
    foo::bar::process(42, "example");
}
