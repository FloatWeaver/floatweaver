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

//! [Check a condition across all lanes asm]
bool any_greater_float8(weaver::float8 a, weaver::float8 b)
{
	return weaver::any(a > b);
}
//! [Check a condition across all lanes asm]

TEST_CASE("Check a condition across all lanes (asm demo)", "[doc]")
{
	weaver::float8 limit{ 9.0f };

	REQUIRE_FALSE(any_greater_float8(weaver::float8{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f }, limit));
	REQUIRE(any_greater_float8(weaver::float8{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 20.0f }, limit));
}
