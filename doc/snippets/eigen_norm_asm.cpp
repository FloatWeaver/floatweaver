// Copyright 2023 Basil Fierz
//
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// Catch2
#include <catch2/catch_all.hpp>

// FloatWeaver
#include <weaver/eigen.h>

// See doc/snippets/float8_asm.cpp for why this function takes its operands
// as parameters rather than literals.

//! [Eigen squared norm with float8 asm]
weaver::float8 squared_norm_vec3_float8(Eigen::Matrix<weaver::float8, 3, 1> v)
{
	return v.squaredNorm();
}
//! [Eigen squared norm with float8 asm]

TEST_CASE("Eigen squared norm with float8 (asm demo)", "[doc][eigen]")
{
	Eigen::Matrix<weaver::float8, 3, 1> v{ weaver::float8{1.0f}, weaver::float8{2.0f}, weaver::float8{3.0f} };

	REQUIRE(weaver::all(squared_norm_vec3_float8(v) == weaver::float8{14.0f}));
}
