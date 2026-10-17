// Copyright 2023 Basil Fierz
//
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// Eigen
#include <Eigen/Geometry>

// Catch2
#include <catch2/catch_all.hpp>

// FloatWeaver
#include <weaver/eigen.h>

// See doc/snippets/float8_snippets.cpp for the tagging convention this file
// follows.

TEST_CASE("Eigen cross product with float8", "[doc][eigen]")
{
	//! [Eigen cross product with float8]
	Eigen::Matrix<weaver::float8, 3, 1> a{ weaver::float8{1.0f}, weaver::float8{2.0f}, weaver::float8{3.0f} };
	Eigen::Matrix<weaver::float8, 3, 1> b{ weaver::float8{4.0f}, weaver::float8{5.0f}, weaver::float8{6.0f} };

	Eigen::Matrix<weaver::float8, 3, 1> c = a.cross(b);
	//! [Eigen cross product with float8]

	REQUIRE(weaver::all(c.x() == weaver::float8{-3.0f}));
	REQUIRE(weaver::all(c.y() == weaver::float8{6.0f}));
	REQUIRE(weaver::all(c.z() == weaver::float8{-3.0f}));
}
