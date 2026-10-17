// Copyright 2023 Basil Fierz
//
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// C++ standard library
#include <cmath>

// Catch2
#include <catch2/catch_all.hpp>

// FloatWeaver
#include <weaver/vector_types.h>

// See doc/snippets/float8_asm.cpp for why these functions take their
// operands as parameters rather than literals.

//! [Approximate reciprocal float8 asm]
weaver::float8 rcp_float8(weaver::float8 a)
{
	return weaver::rcp(a);
}
//! [Approximate reciprocal float8 asm]

//! [Approximate reciprocal square root float8 asm]
weaver::float8 rsqrt_float8(weaver::float8 a)
{
	return weaver::rsqrt(a);
}
//! [Approximate reciprocal square root float8 asm]

TEST_CASE("Approximate reciprocal float8 (asm demo)", "[doc]")
{
	weaver::float8 a{ 2.0f, 4.0f, 8.0f, 5.0f, 1.0f, 10.0f, 16.0f, 20.0f };

	weaver::float8 r = rcp_float8(a);
	for (int i = 0; i < 8; ++i)
		REQUIRE(std::fabs(r[i] - 1.0f / a[i]) < 1e-2f * (1.0f / a[i]));
}

TEST_CASE("Approximate reciprocal square root float8 (asm demo)", "[doc]")
{
	weaver::float8 a{ 4.0f, 9.0f, 16.0f, 25.0f, 1.0f, 100.0f, 64.0f, 36.0f };

	weaver::float8 r = rsqrt_float8(a);
	for (int i = 0; i < 8; ++i)
	{
		float exact = 1.0f / std::sqrt(a[i]);
		REQUIRE(std::fabs(r[i] - exact) < 1e-2f * exact);
	}
}
