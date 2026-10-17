// Copyright 2023 Basil Fierz
//
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// Catch2
#include <catch2/catch_all.hpp>

// FloatWeaver
#include <weaver/vector_types.h>

// See doc/snippets/float8_asm.cpp for why this function takes its operands
// as parameters rather than literals.

//! [Select branchless max asm]
weaver::float8 select_max(weaver::float8 a, weaver::float8 b)
{
	return weaver::select(a < b, b, a);
}
//! [Select branchless max asm]

TEST_CASE("Select branchless max (asm demo)", "[doc]")
{
	weaver::float8 a{ 1.0f, 5.0f, 3.0f, 8.0f, 2.0f, 9.0f, 4.0f, 0.0f };
	weaver::float8 b{ 4.0f, 2.0f, 6.0f, 1.0f, 7.0f, 3.0f, 5.0f, 9.0f };

	REQUIRE(weaver::all(select_max(a, b) == weaver::float8{ 4.0f, 5.0f, 6.0f, 8.0f, 7.0f, 9.0f, 5.0f, 9.0f }));
}
