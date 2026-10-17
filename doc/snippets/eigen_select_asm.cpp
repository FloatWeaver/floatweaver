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

//! [Branchless select over an Eigen vector asm]
Eigen::Matrix<weaver::float8, 3, 1> select_vec3_float8(
	weaver::VectorScalar<bool, 8> mask,
	Eigen::Matrix<weaver::float8, 3, 1> a,
	Eigen::Matrix<weaver::float8, 3, 1> b)
{
	return weaver::select(mask, a, b);
}
//! [Branchless select over an Eigen vector asm]

TEST_CASE("Branchless select over an Eigen vector (asm demo)", "[doc][eigen]")
{
	Eigen::Matrix<weaver::float8, 3, 1> fast{ weaver::float8{1.0f}, weaver::float8{2.0f}, weaver::float8{3.0f} };
	Eigen::Matrix<weaver::float8, 3, 1> slow{ weaver::float8{4.0f}, weaver::float8{5.0f}, weaver::float8{6.0f} };
	weaver::VectorScalar<bool, 8> useFast{ true, false, true, false, true, false, true, false };

	auto v = select_vec3_float8(useFast, fast, slow);
	REQUIRE(v.x()[0] == 1.0f);
	REQUIRE(v.x()[1] == 4.0f);
}
