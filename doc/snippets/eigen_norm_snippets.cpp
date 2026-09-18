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

TEST_CASE("Eigen squared norm with float8", "[doc][eigen]")
{
	//! [Eigen squared norm with float8]
	Eigen::Matrix<weaver::float8, 3, 1> v{ weaver::float8{1.0f}, weaver::float8{2.0f}, weaver::float8{3.0f} };

	// squaredNorm() is an Eigen algorithm, not a FloatWeaver primitive --
	// nobody wrote it for this Scalar type, Eigen just calls operator* and
	// operator+ generically.
	weaver::float8 lenSq = v.squaredNorm();
	//! [Eigen squared norm with float8]

	REQUIRE(weaver::all(lenSq == weaver::float8{14.0f}));
}
