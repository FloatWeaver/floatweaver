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

//! [Square root float8 asm]
weaver::float8 sqrt_float8(weaver::float8 a)
{
	return weaver::sqrt(a);
}
//! [Square root float8 asm]

TEST_CASE("Square root float8 (asm demo)", "[doc]")
{
	weaver::float8 a{ 4.0f, 9.0f, 16.0f, 25.0f, 1.0f, 100.0f, 64.0f, 36.0f };

	REQUIRE(weaver::all(sqrt_float8(a) == weaver::float8{ 2.0f, 3.0f, 4.0f, 5.0f, 1.0f, 10.0f, 8.0f, 6.0f }));
}
