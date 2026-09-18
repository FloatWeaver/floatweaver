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

//! [Eigen vector add with float8 asm]
Eigen::Matrix<weaver::float8, 3, 1> add_vec3_float8(
	Eigen::Matrix<weaver::float8, 3, 1> a,
	Eigen::Matrix<weaver::float8, 3, 1> b)
{
	return a + b;
}
//! [Eigen vector add with float8 asm]

TEST_CASE("Eigen vector add with float8 (asm demo)", "[doc][eigen]")
{
	Eigen::Matrix<weaver::float8, 3, 1> a{ weaver::float8{1.0f}, weaver::float8{2.0f}, weaver::float8{3.0f} };
	Eigen::Matrix<weaver::float8, 3, 1> b{ weaver::float8{4.0f}, weaver::float8{5.0f}, weaver::float8{6.0f} };

	auto c = add_vec3_float8(a, b);
	REQUIRE(weaver::all(c.x() == weaver::float8{5.0f}));
	REQUIRE(weaver::all(c.y() == weaver::float8{7.0f}));
	REQUIRE(weaver::all(c.z() == weaver::float8{9.0f}));
}
