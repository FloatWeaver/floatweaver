// Copyright 2023 Basil Fierz
//
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// Catch2
#include <catch2/catch_all.hpp>

// FloatWeaver
#include <weaver/vector_types.h>

// Functions in this file are compiled in isolation (see the WEAVER_BUILD_DOCS
// block in the root CMakeLists.txt and doc/tools/extract_asm.py) and
// disassembled so the how-to docs can show the actual vector instructions
// FloatWeaver generates, not just an assertion that it's "fast".
//
// Operands are taken as parameters rather than literals on purpose: literal
// operands let the compiler constant-fold the whole operation away at -O2,
// which would leave a `mov` of a precomputed constant in the disassembly
// instead of the vector instruction we're trying to show.

//! [Add float8 asm]
weaver::float8 add_float8(weaver::float8 a, weaver::float8 b)
{
	return a + b;
}
//! [Add float8 asm]

TEST_CASE("Add float8 (asm demo)", "[doc]")
{
	weaver::float8 a{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
	weaver::float8 b{ 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };

	REQUIRE(weaver::all(add_float8(a, b) == weaver::float8{ 9.0f }));
}
