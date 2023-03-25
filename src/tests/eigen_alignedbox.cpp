// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// Catch2
#include <catch2/catch_all.hpp>

// Eigen
#include <Eigen/Core>
#include <Eigen/Geometry>

// FloatWeaver
#include <weaver/eigen.h>

TEST_CASE("Initialize AlignedBox", "[eigen]")
{
	using weaver::float4;

	using weaver::all;

	Eigen::AlignedBox<float4, 2> aabb;
	Eigen::Matrix<float4, 2, 1> max = Eigen::Matrix<float4, 2, 1>::Constant(std::numeric_limits<float>::max());
	Eigen::Matrix<float4, 2, 1> min = Eigen::Matrix<float4, 2, 1>::Constant(-std::numeric_limits<float>::max());

	CHECK(all(aabb.min().x() == max.x()));
	CHECK(all(aabb.min().y() == max.y()));
	CHECK(all(aabb.max().x() == min.x()));
	CHECK(all(aabb.max().y() == min.y()));
}
