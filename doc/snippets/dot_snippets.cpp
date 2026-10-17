// Copyright 2023 Basil Fierz
//
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// Catch2
#include <catch2/catch_all.hpp>

// FloatWeaver
#include <weaver/vector_types.h>

// See doc/snippets/float8_snippets.cpp for the tagging convention this file
// follows.

TEST_CASE("Dot product float8", "[doc]")
{
	//! [Dot product float8]
	weaver::float8 x{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	weaver::float8 y{ 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f };

	// Unlike the elementwise ops above, this collapses all 8 lanes into one
	// plain `float` -- a horizontal reduction, not a per-lane result.
	float d = x.dot(y);
	//! [Dot product float8]

	REQUIRE(d == 16.0f);
}
