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

//! [Branchless sign float8 asm]
weaver::float8 sgn_float8(weaver::float8 a)
{
	return weaver::sgn(a);
}
//! [Branchless sign float8 asm]

TEST_CASE("Branchless sign float8 (asm demo)", "[doc]")
{
	weaver::float8 a{ -3.0f, 0.0f, 2.0f, -0.5f, 7.0f, -7.0f, 0.0f, 1.0f };

	REQUIRE(weaver::all(sgn_float8(a) == weaver::float8{ -1.0f, 0.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 1.0f }));
}
