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

TEST_CASE("Select branchless max", "[doc]")
{
	//! [Select branchless max]
	weaver::float8 a{ 1.0f, 5.0f, 3.0f, 8.0f, 2.0f, 9.0f, 4.0f, 0.0f };
	weaver::float8 b{ 4.0f, 2.0f, 6.0f, 1.0f, 7.0f, 3.0f, 5.0f, 9.0f };

	// Equivalent to "for each lane: a[i] < b[i] ? b[i] : a[i]" -- but with no
	// branch, so every lane is computed regardless of the others.
	weaver::float8 m = weaver::select(a < b, b, a);
	//! [Select branchless max]

	REQUIRE(weaver::all(m == weaver::float8{ 4.0f, 5.0f, 6.0f, 8.0f, 7.0f, 9.0f, 5.0f, 9.0f }));
}
