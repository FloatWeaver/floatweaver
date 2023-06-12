// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// FloatWeaver
#include "common.h"

TEST_CASE("Negate FloatN", "[float][operation]")
{
	WEAVER_SIMD_FLOATS;

	float4 f4 = -f4_asc;
	for (int i = 0; i < 4; i++)
	{
		CHECK(f4[i] == -f4_asc[i]);
	}

	float8 f8 = -f8_asc;
	for (int i = 0; i < 8; i++)
	{
		CHECK(f8[i] == -f8_asc[i]);
	}

	float16 f16 = -f16_asc;
	for (int i = 0; i < 16; i++)
	{
		CHECK(f16[i] == -f16_asc[i]);
	}
}

TEST_CASE("Check infinity", "[float][operation]")
{
	using weaver::all;
	using weaver::float16;
	using weaver::float4;
	using weaver::float8;
	using weaver::isinf;

	const float inf = std::numeric_limits<float>::infinity();

	CHECK(all(isinf(float4(inf))));
	CHECK(all(float4(inf) == float4(inf)));
	CHECK(all(float4(-inf) == -float4(inf)));
	CHECK(all(float4(2 * inf) == float4(2) * float4(inf)));
	CHECK(none(isinf(float4(0) * float4(inf))));
}

TEST_CASE("Sqrt FloatN", "[float][operation]")
{
	using weaver::float16;
	using weaver::float4;
	using weaver::float8;

	// Source data
	float4 vec1{ 0.0f, 6.10116f, 11.6117f, 11.8436f };
	float8 vec2{ 0.0f, 6.10116f, 11.6117f, 11.8436f,
				 0.0f, 6.10116f, 11.6117f, 11.8436f };
	float16 vec3{ 0.0f, 6.10116f, 11.6117f, 11.8436f,
				  0.0f, 6.10116f, 11.6117f, 11.8436f,
				  0.0f, 6.10116f, 11.6117f, 11.8436f,
				  0.0f, 6.10116f, 11.6117f, 11.8436f };

	// Compute 1 / sqrt(x)
	float4 res1 = sqrt(vec1);
	float8 res2 = sqrt(vec2);
	float16 res3 = sqrt(vec3);

	// Reference result
	float4 ref1{ 0.0f, 2.4700526f, 3.4075945f, 3.4414532f };
	float8 ref2{ 0.0f, 2.4700526f, 3.4075945f, 3.4414532f,
				 0.0f, 2.4700526f, 3.4075945f, 3.4414532f };
	float16 ref3{ 0.0f, 2.4700526f, 3.4075945f, 3.4414532f,
				  0.0f, 2.4700526f, 3.4075945f, 3.4414532f,
				  0.0f, 2.4700526f, 3.4075945f, 3.4414532f,
				  0.0f, 2.4700526f, 3.4075945f, 3.4414532f };

	CHECK(all(equal(ref1, res1, float4(1e-5f))));
	CHECK(all(equal(ref2, res2, float8(1e-5f))));
	CHECK(all(equal(ref3, res3, float16(1e-5f))));
}

TEST_CASE("Rsqrt FloatN", "[float][operation]")
{
	using weaver::float16;
	using weaver::float4;
	using weaver::float8;

	// Source data
	float4 vec1{ 11.3805f, 6.10116f, 11.6117f, 11.8436f };
	float8 vec2{ 11.3805f, 6.10116f, 11.6117f, 11.8436f,
				 11.3805f, 6.10116f, 11.6117f, 11.8436f };
	float16 vec3{ 11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f };

	// Compute 1 / sqrt(x)
	float4 res1 = rsqrt(vec1);
	float8 res2 = rsqrt(vec2);
	float16 res3 = rsqrt(vec3);

	// Reference result
	float4 ref1{ 0.2964281f, 0.4048497f, 0.2934622f, 0.2905749f };
	float8 ref2{ 0.2964281f, 0.4048497f, 0.2934622f, 0.2905749f,
				 0.2964281f, 0.4048497f, 0.2934622f, 0.2905749f };
	float16 ref3{ 0.2964281f, 0.4048497f, 0.2934622f, 0.2905749f,
				  0.2964281f, 0.4048497f, 0.2934622f, 0.2905749f,
				  0.2964281f, 0.4048497f, 0.2934622f, 0.2905749f,
				  0.2964281f, 0.4048497f, 0.2934622f, 0.2905749f };

	CHECK(all(equal(ref1, res1, float4(1e-5f))));
	CHECK(all(equal(ref2, res2, float8(1e-5f))));
	CHECK(all(equal(ref3, res3, float16(1e-5f))));
}

TEST_CASE("Rcp FloatN", "[float][operation]")
{
	using weaver::float16;
	using weaver::float4;
	using weaver::float8;

	// Source data
	float4 vec1{ 11.3805f, 6.10116f, 11.6117f, 11.8436f };
	float8 vec2{ 11.3805f, 6.10116f, 11.6117f, 11.8436f,
				 11.3805f, 6.10116f, 11.6117f, 11.8436f };
	float16 vec3{ 11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f };

	// Compute 1 / x
	float4 res1 = rcp(vec1);
	float8 res2 = rcp(vec2);
	float16 res3 = rcp(vec3);

	// Reference result
	float4 ref1{ 0.0878696f, 0.16390326f, 0.0861200f, 0.0844338f };
	float8 ref2{ 0.0878696f, 0.16390326f, 0.0861200f, 0.0844338f,
				 0.0878696f, 0.16390326f, 0.0861200f, 0.0844338f };
	float16 ref3{ 0.0878696f, 0.16390326f, 0.0861200f, 0.0844338f,
				  0.0878696f, 0.16390326f, 0.0861200f, 0.0844338f,
				  0.0878696f, 0.16390326f, 0.0861200f, 0.0844338f,
				  0.0878696f, 0.16390326f, 0.0861200f, 0.0844338f };

	CHECK(all(equal(ref1, res1, float4(1e-5f))));
	CHECK(all(equal(ref2, res2, float8(1e-5f))));
	CHECK(all(equal(ref3, res3, float16(1e-5f))));
}

TEST_CASE("Pow FloatN", "[float][operation]")
{
	using weaver::float16;
	using weaver::float4;
	using weaver::float8;

	// Source data
	float4 vec1{ 11.3805f, 6.10116f, 11.6117f, 11.8436f };
	float8 vec2{ 11.3805f, 6.10116f, 11.6117f, 11.8436f,
				 11.3805f, 6.10116f, 11.6117f, 11.8436f };
	float16 vec3{ 11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f };

	// Compute pow(x, 2)
	float4 res1 = pow(vec1, float4(2.0f));
	float8 res2 = pow(vec2, float8(2.0f));
	float16 res3 = pow(vec3, float16(2.0f));

	// Reference result
	float4 ref1{ 129.51578025f, 37.2241533456f, 134.83157689f, 140.27086096f };
	float8 ref2{ 129.51578025f, 37.2241533456f, 134.83157689f, 140.27086096f,
				 129.51578025f, 37.2241533456f, 134.83157689f, 140.27086096f };
	float16 ref3{ 129.51578025f, 37.2241533456f, 134.83157689f, 140.27086096f,
				  129.51578025f, 37.2241533456f, 134.83157689f, 140.27086096f,
				  129.51578025f, 37.2241533456f, 134.83157689f, 140.27086096f,
				  129.51578025f, 37.2241533456f, 134.83157689f, 140.27086096f };

	CHECK(all(equal(ref1, res1, float4(1e-5f))));
	CHECK(all(equal(ref2, res2, float8(1e-5f))));
	CHECK(all(equal(ref3, res3, float16(1e-5f))));
}

TEST_CASE("Log FloatN", "[float][operation]")
{
	using weaver::float16;
	using weaver::float4;
	using weaver::float8;

	// Source data
	float4 vec1{ 11.3805f, 6.10116f, 11.6117f, 11.8436f };
	float8 vec2{ 11.3805f, 6.10116f, 11.6117f, 11.8436f,
				 11.3805f, 6.10116f, 11.6117f, 11.8436f };
	float16 vec3{ 11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f };

	// Compute log(x)
	float4 res1 = log(vec1);
	float8 res2 = log(vec2);
	float16 res3 = log(vec3);

	// Reference result
	float4 ref1{ 2.43190136f, 1.8084789170f, 2.45201321f, 2.47178763f };
	float8 ref2{ 2.43190136f, 1.8084789170f, 2.45201321f, 2.47178763f,
				 2.43190136f, 1.8084789170f, 2.45201321f, 2.47178763f };
	float16 ref3{ 2.43190136f, 1.8084789170f, 2.45201321f, 2.47178763f,
				  2.43190136f, 1.8084789170f, 2.45201321f, 2.47178763f,
				  2.43190136f, 1.8084789170f, 2.45201321f, 2.47178763f,
				  2.43190136f, 1.8084789170f, 2.45201321f, 2.47178763f };

	CHECK(all(equal(ref1, res1, float4(1e-5f))));
	CHECK(all(equal(ref2, res2, float8(1e-5f))));
	CHECK(all(equal(ref3, res3, float16(1e-5f))));
}

TEST_CASE("Abs FloatN", "[float][operation]")
{
	using weaver::all;

	using weaver::float16;
	using weaver::float4;
	using weaver::float8;

	// Source data
	float4 vec1{ -1 };
	float8 vec2{ -1 };
	float16 vec3{ -1 };

	// Compute dot(x, x)
	float4 res1 = abs(vec1);
	float8 res2 = abs(vec2);
	float16 res3 = abs(vec3);

	// Reference result
	CHECK(all(equal(res1, float4{ 1 })));
	CHECK(all(equal(res2, float8{ 1 })));
	CHECK(all(equal(res3, float16{ 1 })));
}

TEST_CASE("Dot FloatN", "[float][operation]")
{
	using weaver::float16;
	using weaver::float4;
	using weaver::float8;

	using weaver::equal;

	// Source data
	float4 vec1{ 11.3805f, 6.10116f, 11.6117f, 11.8436f };
	float8 vec2{ 11.3805f, 6.10116f, 11.6117f, 11.8436f,
				 11.3805f, 6.10116f, 11.6117f, 11.8436f };
	float16 vec3{ 11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f,
				  11.3805f, 6.10116f, 11.6117f, 11.8436f };

	// Compute dot(x, x)
	float res1 = vec1.dot(vec1);
	float res2 = vec2.dot(vec2);
	float res3 = vec3.dot(vec3);

	// Reference result
	float ref = 11.3805f * 11.3805f + 6.10116f * 6.10116f + 11.6117f * 11.6117f + 11.8436f * 11.8436f;

	CHECK(equal(ref * 1, res1, 1e-5f));
	CHECK(equal(ref * 2, res2, 1e-5f));
	CHECK(equal(ref * 4, res3, 1e-5f));
}

template<int W>
void fltSignumTest()
{
	using weaver::all;
	using weaver::sgn;

	using floatN = weaver::VectorScalar<float, W>;

	std::random_device rnd;
	std::uniform_real_distribution<float> dist{ -10, 10 };

	CHECK(all(sgn(floatN(0)) == floatN(0)));
	CHECK(all(sgn(floatN(-0.0f)) == floatN(0)));
	for (int i = 0; i < 50; i++)
	{
		const float d = dist(rnd);
		if (d < 0)
		{
			CHECK(all(sgn(floatN(d)) == floatN(-1)));
		}
		else if (d > 0)
		{
			CHECK(all(sgn(floatN(d)) == floatN(1)));
		}
	}
}

TEST_CASE("Signum FloatN", "[float][operation]")
{
	fltSignumTest<4>();
	fltSignumTest<8>();
	fltSignumTest<16>();
}

TEST_CASE("Horizontal min/max FloatN", "[float][operation]")
{
	using weaver::float16;
	using weaver::float4;
	using weaver::float8;

	float4 vec4(1, 2, 3, 4);
	CHECK(vec4.min() == 1);
	CHECK(vec4.max() == 4);

	float8 vec8(1, 2, 3, 4, 5, 6, 7, 8);
	CHECK(vec8.min() == 1);
	CHECK(vec8.max() == 8);

	float16 vec16(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	CHECK(vec16.min() == 1);
	CHECK(vec16.max() == 16);
}

TEST_CASE("Abs IntN", "[int][operation]")
{
	using weaver::all;

	using weaver::int16;
	using weaver::int4;
	using weaver::int8;

	// Source data
	int4 vec1{ -1 };
	int8 vec2{ -1 };
	int16 vec3{ -1 };

	// Compute dot(x, x)
	int4 res1 = abs(vec1);
	int8 res2 = abs(vec2);
	int16 res3 = abs(vec3);

	// Reference result
	CHECK(all(res1 == int4{ 1 }));
	CHECK(all(res2 == int8{ 1 }));
	CHECK(all(res3 == int16{ 1 }));
}

TEST_CASE("Max IntN", "[int][operation]")
{
	WEAVER_SIMD_INTS;

	int4 i4 = max(i4_asc, i4_desc);
	for (int i = 0; i < 4; i++)
		CHECK(i4[i] == std::max(i4_asc[i], i4_desc[i]));

	int8 i8 = max(i8_asc, i8_desc);
	for (int i = 0; i < 8; i++)
		CHECK(i8[i] == std::max(i8_asc[i], i8_desc[i]));

	int16 i16 = max(i16_asc, i16_desc);
	for (int i = 0; i < 16; i++)
		CHECK(i16[i] == std::max(i16_asc[i], i16_desc[i]));
}

TEST_CASE("Min IntN", "[int][operation]")
{
	WEAVER_SIMD_INTS;

	int4 i4 = min(i4_asc, i4_desc);
	for (int i = 0; i < 4; i++)
		CHECK(i4[i] == std::min(i4_asc[i], i4_desc[i]));

	int8 i8 = min(i8_asc, i8_desc);
	for (int i = 0; i < 8; i++)
		CHECK(i8[i] == std::min(i8_asc[i], i8_desc[i]));

	int16 i16 = min(i16_asc, i16_desc);
	for (int i = 0; i < 16; i++)
		CHECK(i16[i] == std::min(i16_asc[i], i16_desc[i]));
}

template<int W>
void intSignumTest()
{
	using weaver::all;
	using weaver::sgn;

	using intN = weaver::VectorScalar<int, W>;

	std::random_device rnd;
	std::uniform_int_distribution<int> dist{ -10, 10 };

	CHECK(all(sgn(intN(0)) == intN(0)));
	for (int i = 0; i < 50; i++)
	{
		const int d = dist(rnd);
		if (d < 0)
		{
			CHECK(all(sgn(intN(d)) == intN(-1)));
		}
		else if (d > 0)
		{
			CHECK(all(sgn(intN(d)) == intN(1)));
		}
	}
}

TEST_CASE("Signum IntN", "[int][operation]")
{
	intSignumTest<4>();
	intSignumTest<8>();
	intSignumTest<16>();
}
