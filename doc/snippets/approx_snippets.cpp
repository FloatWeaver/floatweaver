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

// See doc/snippets/float8_snippets.cpp for the tagging convention this file
// follows.

TEST_CASE("Approximate reciprocal float8", "[doc]")
{
	//! [Approximate reciprocal float8]
	weaver::float8 x{ 2.0f, 4.0f, 8.0f, 5.0f, 1.0f, 10.0f, 16.0f, 20.0f };

	// An approximation of `1 / x`, not an exact divide -- trades a couple of
	// ULPs of accuracy for a much cheaper instruction.
	weaver::float8 r = weaver::rcp(x);
	//! [Approximate reciprocal float8]

	for (int i = 0; i < 8; ++i)
		REQUIRE(std::fabs(r[i] - 1.0f / x[i]) < 1e-2f * (1.0f / x[i]));
}

TEST_CASE("Approximate reciprocal square root float8", "[doc]")
{
	//! [Approximate reciprocal square root float8]
	weaver::float8 x{ 4.0f, 9.0f, 16.0f, 25.0f, 1.0f, 100.0f, 64.0f, 36.0f };

	// An approximation of `1 / sqrt(x)` that avoids both the divide and the
	// square root -- the common case for normalizing a vector.
	weaver::float8 r = weaver::rsqrt(x);
	//! [Approximate reciprocal square root float8]

	for (int i = 0; i < 8; ++i)
	{
		float exact = 1.0f / std::sqrt(x[i]);
		REQUIRE(std::fabs(r[i] - exact) < 1e-2f * exact);
	}
}
