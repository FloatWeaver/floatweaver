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

//! [Dot product float8 asm]
float dot_float8(weaver::float8 a, weaver::float8 b)
{
	return a.dot(b);
}
//! [Dot product float8 asm]

TEST_CASE("Dot product float8 (asm demo)", "[doc]")
{
	weaver::float8 a{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	weaver::float8 b{ 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f };

	REQUIRE(dot_float8(a, b) == 16.0f);
}
