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

TEMPLATE_TEST_CASE("Eigen::AlignedBox with floatN", "[eigen][float]", weaver::float4, weaver::float8, weaver::float16)
{
	using weaver::all;

	SECTION("Initialize to default values")
	{
		Eigen::AlignedBox<TestType, 2> aabb;
		Eigen::Matrix<TestType, 2, 1> max = Eigen::Matrix<TestType, 2, 1>::Constant(std::numeric_limits<float>::max());
		Eigen::Matrix<TestType, 2, 1> min = Eigen::Matrix<TestType, 2, 1>::Constant(-std::numeric_limits<float>::max());

		CHECK(all(aabb.min().x() == max.x()));
		CHECK(all(aabb.min().y() == max.y()));
		CHECK(all(aabb.max().x() == min.x()));
		CHECK(all(aabb.max().y() == min.y()));
	}

	SECTION("Compute Intersection")
	{
		Eigen::Matrix<TestType, 2, 1> m1{ TestType{-1.0f}, TestType{-1.0f} };
		Eigen::Matrix<TestType, 2, 1> null{ TestType{0.0f}, TestType{0.0f} };
		Eigen::Matrix<TestType, 2, 1> p1{ TestType{1.0f}, TestType{1.0f} };
		Eigen::Matrix<TestType, 2, 1> p2{ TestType{2.0f}, TestType{2.0f} };

		Eigen::AlignedBox<TestType, 2> aabb0{ m1, p1 };
		Eigen::AlignedBox<TestType, 2> aabb1{ null, p2 };

		const auto intersection = aabb0.intersection(aabb1);

		CHECK(all(intersection.min().x() == null.x()));
		CHECK(all(intersection.min().y() == null.y()));
		CHECK(all(intersection.max().x() == p1.x()));
		CHECK(all(intersection.max().y() == p1.y()));
	}

	SECTION("Compute Merged")
	{
		Eigen::Matrix<TestType, 2, 1> m1{ TestType{-1.0f}, TestType{-1.0f} };
		Eigen::Matrix<TestType, 2, 1> null{ TestType{0.0f}, TestType{0.0f} };
		Eigen::Matrix<TestType, 2, 1> p1{ TestType{1.0f}, TestType{1.0f} };
		Eigen::Matrix<TestType, 2, 1> p2{ TestType{2.0f}, TestType{2.0f} };

		Eigen::AlignedBox<TestType, 2> aabb0{ m1, p1 };
		Eigen::AlignedBox<TestType, 2> aabb1{ null, p2 };

		const auto merged = aabb0.merged(aabb1);

		CHECK(all(merged.min().x() == m1.x()));
		CHECK(all(merged.min().y() == m1.y()));
		CHECK(all(merged.max().x() == p2.x()));
		CHECK(all(merged.max().y() == p2.y()));
	}
}
