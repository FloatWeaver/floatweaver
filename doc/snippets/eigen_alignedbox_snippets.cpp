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

TEST_CASE("Merge axis-aligned boxes with float8", "[doc][eigen]")
{
	//! [Merge axis-aligned boxes with float8]
	// 8 independent 2D boxes, merged in one call -- Eigen::AlignedBox needs
	// no changes to work with weaver::float8 as its Scalar.
	Eigen::AlignedBox<weaver::float8, 2> a{
		Eigen::Matrix<weaver::float8, 2, 1>{ weaver::float8{-1.0f}, weaver::float8{-1.0f} },
		Eigen::Matrix<weaver::float8, 2, 1>{ weaver::float8{1.0f}, weaver::float8{1.0f} }
	};
	Eigen::AlignedBox<weaver::float8, 2> b{
		Eigen::Matrix<weaver::float8, 2, 1>{ weaver::float8{0.0f}, weaver::float8{0.0f} },
		Eigen::Matrix<weaver::float8, 2, 1>{ weaver::float8{2.0f}, weaver::float8{2.0f} }
	};

	Eigen::AlignedBox<weaver::float8, 2> merged = a.merged(b);
	//! [Merge axis-aligned boxes with float8]

	REQUIRE(weaver::all(merged.min().x() == weaver::float8{-1.0f}));
	REQUIRE(weaver::all(merged.max().x() == weaver::float8{2.0f}));
}
