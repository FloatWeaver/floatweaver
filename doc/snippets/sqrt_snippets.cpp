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

TEST_CASE("Square root float8", "[doc]")
{
	//! [Square root float8]
	weaver::float8 x{ 4.0f, 9.0f, 16.0f, 25.0f, 1.0f, 100.0f, 64.0f, 36.0f };

	weaver::float8 r = weaver::sqrt(x);
	//! [Square root float8]

	// Not every backend has a native sqrt instruction -- NEON without one
	// builds it from an approximate rsqrt, so this isn't bit-exact across
	// backends and is compared with a tolerance like the library's own
	// "Sqrt FloatN" test (src/tests/simd.cpp) does.
	weaver::float8 expected{ 2.0f, 3.0f, 4.0f, 5.0f, 1.0f, 10.0f, 8.0f, 6.0f };
	REQUIRE(weaver::all(weaver::equal(r, expected, weaver::float8{ 1e-5f })));
}
