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

TEST_CASE("Check a condition across all lanes", "[doc]")
{
	//! [Check a condition across all lanes]
	weaver::float8 x{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 20.0f };
	weaver::float8 limit{ 9.0f };

	// Per-lane comparisons don't decide anything by themselves -- `any`
	// reduces the 8-wide result down to the single bool a real `if` needs.
	if (weaver::any(x > limit))
	{
		// at least one lane exceeded the limit
	}
	//! [Check a condition across all lanes]

	REQUIRE(weaver::any(x > limit));
	REQUIRE_FALSE(weaver::all(x > limit));
}
