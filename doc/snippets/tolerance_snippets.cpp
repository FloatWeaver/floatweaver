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

TEST_CASE("Compare floats with a tolerance", "[doc]")
{
	//! [Compare floats with a tolerance]
	weaver::float8 a{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	weaver::float8 b{ 1.0000001f, 1.1f, 0.9f, 1.0f, 2.0f, 1.00001f, 1.0f, 1.0f };

	// A relative-tolerance compare, per lane -- see the reference this is
	// based on in vector_types.h.
	weaver::VectorScalar<bool, 8> close = weaver::equal(a, b, weaver::float8{ 1e-4f });
	//! [Compare floats with a tolerance]

	bool expected[8] = { true, false, false, true, false, true, true, true };
	for (int i = 0; i < 8; ++i)
		REQUIRE(close[i] == expected[i]);
}
