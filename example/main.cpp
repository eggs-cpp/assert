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

#ifdef _MSC_VER
#    include <cstdlib>
#endif

int main()
{
#ifdef _MSC_VER
    // Suppress blocking MSVC debug error dialog.
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#endif

    foo::bar::process(42, "example");
}
