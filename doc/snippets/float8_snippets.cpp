// Copyright 2023 Basil Fierz
//
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// Catch2
#include <catch2/catch_all.hpp>

// FloatWeaver
#include <weaver/vector_types.h>

// This file holds documentation examples, not unit tests: each TEST_CASE is
// a `//! [tag] ... //! [tag]` bracketed snippet referenced verbatim from the
// Sphinx docs (via `literalinclude`) and/or from a Doxygen `\snippet`
// command. Keep every snippet self-contained -- a reader copying it out of
// the docs must not need any fixture or helper that isn't shown here.

TEST_CASE("Add float8", "[doc]")
{
	//! [Add float8]
	weaver::float8 x{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
	weaver::float8 y{ 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };

	weaver::float8 z = x + y;
	//! [Add float8]

	REQUIRE(weaver::all(z == weaver::float8{ 9.0f }));
}
