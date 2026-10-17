// Copyright 2023 Basil Fierz
//
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// Catch2
#include <catch2/catch_all.hpp>

// FloatWeaver
#include <weaver/eigen.h>

// See doc/snippets/float8_snippets.cpp for the tagging convention this file
// follows.

TEST_CASE("Branchless select over an Eigen vector", "[doc][eigen]")
{
	//! [Branchless select over an Eigen vector]
	// Two candidate velocities for 8 particles at once.
	Eigen::Matrix<weaver::float8, 3, 1> fast{ weaver::float8{1.0f}, weaver::float8{2.0f}, weaver::float8{3.0f} };
	Eigen::Matrix<weaver::float8, 3, 1> slow{ weaver::float8{4.0f}, weaver::float8{5.0f}, weaver::float8{6.0f} };

	weaver::VectorScalar<bool, 8> useFast{ true, false, true, false, true, false, true, false };

	// The same branchless pattern as "Pick a value without branching", but
	// applied component-wise to a whole Eigen vector instead of one scalar.
	Eigen::Matrix<weaver::float8, 3, 1> v = weaver::select(useFast, fast, slow);
	//! [Branchless select over an Eigen vector]

	REQUIRE(v.x()[0] == 1.0f);
	REQUIRE(v.x()[1] == 4.0f);
}
