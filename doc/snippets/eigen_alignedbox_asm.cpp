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

// See doc/snippets/float8_asm.cpp for why this function takes its operands
// as parameters rather than literals.

//! [Merge axis-aligned boxes with float8 asm]
Eigen::AlignedBox<weaver::float8, 2> merge_box2_float8(
	Eigen::AlignedBox<weaver::float8, 2> a,
	Eigen::AlignedBox<weaver::float8, 2> b)
{
	return a.merged(b);
}
//! [Merge axis-aligned boxes with float8 asm]

TEST_CASE("Merge axis-aligned boxes with float8 (asm demo)", "[doc][eigen]")
{
	Eigen::AlignedBox<weaver::float8, 2> a{
		Eigen::Matrix<weaver::float8, 2, 1>{ weaver::float8{-1.0f}, weaver::float8{-1.0f} },
		Eigen::Matrix<weaver::float8, 2, 1>{ weaver::float8{1.0f}, weaver::float8{1.0f} }
	};
	Eigen::AlignedBox<weaver::float8, 2> b{
		Eigen::Matrix<weaver::float8, 2, 1>{ weaver::float8{0.0f}, weaver::float8{0.0f} },
		Eigen::Matrix<weaver::float8, 2, 1>{ weaver::float8{2.0f}, weaver::float8{2.0f} }
	};

	auto merged = merge_box2_float8(a, b);
	REQUIRE(weaver::all(merged.min().x() == weaver::float8{-1.0f}));
	REQUIRE(weaver::all(merged.max().x() == weaver::float8{2.0f}));
}
