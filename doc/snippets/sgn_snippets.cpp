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

TEST_CASE("Branchless sign float8", "[doc]")
{
	//! [Branchless sign float8]
	weaver::float8 x{ -3.0f, 0.0f, 2.0f, -0.5f, 7.0f, -7.0f, 0.0f, 1.0f };

	// -1 / 0 / 1 per lane, with no per-lane branch on the sign.
	weaver::float8 s = weaver::sgn(x);
	//! [Branchless sign float8]

	REQUIRE(weaver::all(s == weaver::float8{ -1.0f, 0.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 1.0f }));
}
