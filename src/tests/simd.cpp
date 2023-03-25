// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// C++ standard library
#include <cmath>
#include <random>

// Catch2
#include <catch2/catch_all.hpp>

// FloatWeaver
#include <weaver/vector_types.h>

#define WEAVER_SIMD_BOOLS                                               \
	using bool4 = weaver::bool4;                                        \
	using bool8 = weaver::bool8;                                        \
	using bool16 = weaver::bool16;                                      \
	bool4 b4_0{ true, false, true, false };                             \
	bool4 b4_1{ false, true, false, true };                             \
	bool8 b8_0{ true, false, true, false, true, false, true, false };   \
	bool8 b8_1{ false, true, false, true, false, true, false, true };   \
	bool16 b16_0{ true, false, true, false, true, false, true, false,   \
				  true, false, true, false, true, false, true, false }; \
	bool16 b16_1{ false, true, false, true, false, true, false, true,   \
				  false, true, false, true, false, true, false, true }

#define WEAVER_SIMD_FLOATS                                                    \
	using float4 = weaver::float4;                                            \
	using float8 = weaver::float8;                                            \
	using float16 = weaver::float16;                                          \
	float4 f4_asc{ 1, 2, 3, 4 };                                              \
	float4 f4_desc{ 4, 3, 2, 1 };                                             \
	float8 f8_asc{ 1, 2, 3, 4, 5, 6, 7, 8 };                                  \
	float8 f8_desc{ 8, 7, 6, 5, 4, 3, 2, 1 };                                 \
	float16 f16_asc{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }; \
	float16 f16_desc{ 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }

#define WEAVER_SIMD_INTS                                                    \
	using int4 = weaver::int4;                                              \
	using int8 = weaver::int8;                                              \
	using int16 = weaver::int16;                                            \
	int4 i4_asc{ 1, 2, 3, 4 };                                              \
	int4 i4_desc{ 4, 3, 2, 1 };                                             \
	int8 i8_asc{ 1, 2, 3, 4, 5, 6, 7, 8 };                                  \
	int8 i8_desc{ 8, 7, 6, 5, 4, 3, 2, 1 };                                 \
	int16 i16_asc{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }; \
	int16 i16_desc{ 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }

TEST_CASE("Construct bools", "operations")
{
	WEAVER_SIMD_BOOLS;

	using weaver::all;
	using weaver::none;

	CHECK(all(bool4{ true }));
	CHECK(all(bool8{ true }));
	CHECK(all(bool16{ true }));

	CHECK(none(bool4{ false }));
	CHECK(none(bool8{ false }));
	CHECK(none(bool16{ false }));

	bool4 b4{ b4_0 };
	for (int i = 0; i < 4; i++)
	{
		CHECK(b4_0[i] == (i % 2 == 0));
		CHECK(b4[i] == (i % 2 == 0));
	}

	bool8 b8{ b8_0 };
	for (int i = 0; i < 8; i++)
	{
		CHECK(b8_0[i] == (i % 2 == 0));
		CHECK(b8[i] == (i % 2 == 0));
	}

	bool16 b16{ b16_0 };
	for (int i = 0; i < 16; i++)
	{
		CHECK(b16_0[i] == (i % 2 == 0));
		CHECK(b16[i] == (i % 2 == 0));
	}
}

TEST_CASE("Assign bools", "operations")
{
	WEAVER_SIMD_BOOLS;

	using weaver::all;

	bool4 b4{ true };
	bool8 b8{ true };
	bool16 b16{ true };

	CHECK(all(b4));
	CHECK(all(b8));
	CHECK(all(b16));

	b4 = b4_0;
	b8 = b8_0;
	b16 = b16_0;

	for (int i = 0; i < 4; i++)
		CHECK(b4[i] == (i % 2 == 0));

	for (int i = 0; i < 8; i++)
		CHECK(b8[i] == (i % 2 == 0));

	for (int i = 0; i < 16; i++)
		CHECK(b16[i] == (i % 2 == 0));
}

TEST_CASE("And bools", "operations")
{
	WEAVER_SIMD_BOOLS;

	using weaver::none;

	bool4 b4 = b4_0 && b4_0;
	for (int i = 0; i < 4; i++)
		CHECK(b4[i] == (i % 2 == 0));
	CHECK(none(b4_0 && b4_1));

	bool8 b8 = b8_0 && b8_0;
	for (int i = 0; i < 8; i++)
		CHECK(b8[i] == (i % 2 == 0));
	CHECK(none(b8_0 && b8_1));

	bool16 b16 = b16_0 && b16_0;
	for (int i = 0; i < 16; i++)
		CHECK(b16[i] == (i % 2 == 0));
	CHECK(none(b16_0 && b16_1));
}

TEST_CASE("Or bools", "operations")
{
	WEAVER_SIMD_BOOLS;

	using weaver::all;

	bool4 b4 = b4_0 || b4_0;
	for (int i = 0; i < 4; i++)
		CHECK(b4[i] == (i % 2 == 0));
	CHECK(all(b4_0 || b4_1));

	bool8 b8 = b8_0 || b8_0;
	for (int i = 0; i < 8; i++)
		CHECK(b8[i] == (i % 2 == 0));
	CHECK(all(b8_0 || b8_1));

	bool16 b16 = b16_0 || b16_0;
	for (int i = 0; i < 16; i++)
		CHECK(b16[i] == (i % 2 == 0));
	CHECK(all(b16_0 || b16_1));
}

TEST_CASE("Construct floats", "operations")
{
	WEAVER_SIMD_FLOATS;

	float4 f4{ 1 };
	float4 f4_2{ f4 };
	for (int i = 0; i < 4; i++)
	{
		CHECK(f4[i] == 1);
		CHECK(f4_2[i] == 1);
		CHECK(f4_asc[i] == i + 1);
	}

	float8 f8{ 1 };
	float8 f8_2{ f8 };
	for (int i = 0; i < 8; i++)
	{
		CHECK(f8[i] == 1);
		CHECK(f8_2[i] == 1);
		CHECK(f8_asc[i] == i + 1);
	}

	float16 f16{ 1 };
	float16 f16_2{ f16 };
	for (int i = 0; i < 16; i++)
	{
		CHECK(f16[i] == 1);
		CHECK(f16_2[i] == 1);
		CHECK(f16_asc[i] == i + 1);
	}
}

TEST_CASE("Assign floats", "operations")
{
	WEAVER_SIMD_FLOATS;

	float4 f4{ 0 };
	for (int i = 0; i < 4; i++)
		CHECK(f4[i] == 0.0f);
	f4 = f4_asc;
	for (int i = 0; i < 4; i++)
		CHECK(f4_asc[i] == i + 1);

	float8 f8{ 0 };
	for (int i = 0; i < 8; i++)
		CHECK(f8[i] == 0.0f);
	f8 = f8_asc;
	for (int i = 0; i < 8; i++)
		CHECK(f8_asc[i] == i + 1);

	float16 f16{ 0 };
	for (int i = 0; i < 16; i++)
		CHECK(f16[i] == 0.0f);
	f16 = f16_asc;
	for (int i = 0; i < 16; i++)
		CHECK(f16_asc[i] == i + 1);
}

TEST_CASE("Negate floats", "operations")
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

TEST_CASE("Add floats", "operations")
{
	WEAVER_SIMD_FLOATS;

	float4 f4 = f4_asc + f4_asc;
	for (int i = 0; i < 4; i++)
	{
		CHECK(f4[i] == f4_asc[i] + f4_asc[i]);
	}

	float8 f8 = f8_asc + f8_asc;
	for (int i = 0; i < 8; i++)
	{
		CHECK(f8[i] == f8_asc[i] + f8_asc[i]);
	}

	float16 f16 = f16_asc + f16_asc;
	for (int i = 0; i < 16; i++)
	{
		CHECK(f16[i] == f16_asc[i] + f16_asc[i]);
	}
}

TEST_CASE("Subtract floats", "operations")
{
	WEAVER_SIMD_FLOATS;

	float4 f4 = f4_asc - f4_asc;
	for (int i = 0; i < 4; i++)
	{
		CHECK(f4[i] == 0);
	}

	float8 f8 = f8_asc - f8_asc;
	for (int i = 0; i < 8; i++)
	{
		CHECK(f8[i] == 0);
	}

	float16 f16 = f16_asc - f16_asc;
	for (int i = 0; i < 16; i++)
	{
		CHECK(f16[i] == 0);
	}
}

TEST_CASE("Check infinity", "operations")
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

TEST_CASE("Sqrt floats", "operations")
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

TEST_CASE("Signum floats", "operations")
{
	fltSignumTest<4>();
	fltSignumTest<8>();
	fltSignumTest<16>();
}

template<typename T, int W>
void selectTest(
	const weaver::VectorScalar<bool, W>& t,
	const weaver::VectorScalar<T, W>& a,
	const weaver::VectorScalar<T, W>& b,
	const weaver::VectorScalar<T, W>& c)
{
	const auto selected = weaver::select(t, a, b);
	CHECK(weaver::all(selected == c));
}

TEST_CASE("Select floats", "operations")
{
	WEAVER_SIMD_FLOATS;

	selectTest<float, 4>(true, 1.0f, 0.0f, 1.0f);
	selectTest<float, 8>(true, 1.0f, 0.0f, 1.0f);
	selectTest<float, 16>(true, 1.0f, 0.0f, 1.0f);

	selectTest<float, 4>(false, 1.0f, 0.0f, 0.0f);
	selectTest<float, 8>(false, 1.0f, 0.0f, 0.0f);
	selectTest<float, 16>(false, 1.0f, 0.0f, 0.0f);

	selectTest<float, 4>(float4{ 1 } / float4{ 0 } < 0, 1.0f, 0.0f, 0.0f);
	selectTest<float, 8>(float8{ 1 } / float8{ 0 } < 0, 1.0f, 0.0f, 0.0f);
	selectTest<float, 16>(float16{ 1 } / float16{ 0 } < 0, 1.0f, 0.0f, 0.0f);
}

TEST_CASE("Rsqrt floats", "operations")
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

TEST_CASE("Rcp floats", "operations")
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

TEST_CASE("Pow floats", "operations")
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

TEST_CASE("Log floats", "operations")
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

TEST_CASE("Abs floats", "operations")
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

TEST_CASE("Dot floats", "operations")
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

TEST_CASE("Horizontal min/max floats", "operations")
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

TEST_CASE("Construct ints", "operations")
{
	WEAVER_SIMD_INTS;

	int4 i4{ 1 };
	int4 i4_2{ i4 };
	for (int i = 0; i < 4; i++)
	{
		CHECK(i4[i] == 1);
		CHECK(i4_2[i] == 1);
		CHECK(i4_asc[i] == i + 1);
	}

	int8 i8{ 1 };
	int8 i8_2{ i8 };
	for (int i = 0; i < 8; i++)
	{
		CHECK(i8[i] == 1);
		CHECK(i8_2[i] == 1);
		CHECK(i8_asc[i] == i + 1);
	}

	int16 i16{ 1 };
	int16 i16_2{ i16 };
	for (int i = 0; i < 16; i++)
	{
		CHECK(i16[i] == 1);
		CHECK(i16_2[i] == 1);
		CHECK(i16_asc[i] == i + 1);
	}
}

TEST_CASE("Assign ints", "operations")
{
	WEAVER_SIMD_INTS;

	int4 i4{ 0 };
	for (int i = 0; i < 4; i++)
		CHECK(i4[i] == 0);
	i4 = i4_asc;
	for (int i = 0; i < 4; i++)
		CHECK(i4_asc[i] == i + 1);

	int8 i8{ 0 };
	for (int i = 0; i < 8; i++)
		CHECK(i8[i] == 0);
	i8 = i8_asc;
	for (int i = 0; i < 8; i++)
		CHECK(i8_asc[i] == i + 1);

	int16 i16{ 0 };
	for (int i = 0; i < 16; i++)
		CHECK(i16[i] == 0);
	i16 = i16_asc;
	for (int i = 0; i < 16; i++)
		CHECK(i16_asc[i] == i + 1);
}

TEST_CASE("Abs ints", "operations")
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

TEST_CASE("Max ints", "operations")
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

TEST_CASE("Min ints", "operations")
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

TEST_CASE("Signum ints", "operations")
{
	intSignumTest<4>();
	intSignumTest<8>();
	intSignumTest<16>();
}

TEST_CASE("Select ints", "operations")
{
	selectTest<int, 4>(true, 1, 0, 1);
	selectTest<int, 8>(true, 1, 0, 1);
	selectTest<int, 16>(true, 1, 0, 1);

	selectTest<int, 4>(false, 1, 0, 0);
	selectTest<int, 8>(false, 1, 0, 0);
	selectTest<int, 16>(false, 1, 0, 0);
}
