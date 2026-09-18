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

TEST_CASE("Eigen vector add with float8", "[doc][eigen]")
{
	//! [Eigen vector add with float8]
	// weaver::float8 works as the Scalar of an Eigen::Matrix directly, no
	// special vector type required -- this Vector3 holds 8 3D points.
	Eigen::Matrix<weaver::float8, 3, 1> a{ weaver::float8{1.0f}, weaver::float8{2.0f}, weaver::float8{3.0f} };
	Eigen::Matrix<weaver::float8, 3, 1> b{ weaver::float8{4.0f}, weaver::float8{5.0f}, weaver::float8{6.0f} };

	Eigen::Matrix<weaver::float8, 3, 1> c = a + b;
	//! [Eigen vector add with float8]

	REQUIRE(weaver::all(c.x() == weaver::float8{5.0f}));
	REQUIRE(weaver::all(c.y() == weaver::float8{7.0f}));
	REQUIRE(weaver::all(c.z() == weaver::float8{9.0f}));
}
