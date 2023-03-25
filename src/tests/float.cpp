// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// Catch2
#include <catch2/catch_all.hpp>

// FloatWeaver
#include <weaver/common.h>

#if defined WEAVER_VECTORIZE_AVX512
#	include <weaver/float4_sse.h>
#	include <weaver/float8_avx.h>
#	include <weaver/float16_avx512.h>
#elif defined WEAVER_VECTORIZE_AVX
#	include <weaver/float4_sse.h>
#	include <weaver/float8_avx.h>
#	include <weaver/float16_avx.h>
#elif defined WEAVER_VECTORIZE_SSE
#	include <weaver/float4_sse.h>
#	include <weaver/float8_sse.h>
#	include <weaver/float16_sse.h>
#elif defined WEAVER_VECTORIZE_NEON
#	include <weaver/float4_neon.h>
#	include <weaver/float8_neon.h>
#	include <weaver/float16_neon.h>
#else
#	include <weaver/float4_ref.h>
#	include <weaver/float8_ref.h>
#	include <weaver/float16_ref.h>
#endif

TEST_CASE("floats are initialized", "[float]")
{
	using namespace weaver;

	VectorScalar<float, 4> b0{-1.0f, 0.0f, 1.0f, 0.0f};
	CHECK(b0[0] == -1.0f);
	CHECK(b0[1] == 0.0f);
	CHECK(b0[2] == 1.0f);
	CHECK(b0[3] == 0.0f);

	VectorScalar<float, 8> b1{ -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	CHECK(b1[0] == -1.0f);
	CHECK(b1[1] == 0.0f);
	CHECK(b1[2] == 1.0f);
	CHECK(b1[3] == 0.0f);
	CHECK(b1[4] == -1.0f);
	CHECK(b1[5] == 0.0f);
	CHECK(b1[6] == 1.0f);
	CHECK(b1[7] == 0.0f);

	VectorScalar<float, 16> b2{ -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	CHECK(b2[0] == -1.0f);
	CHECK(b2[1] == 0.0f);
	CHECK(b2[2] == 1.0f);
	CHECK(b2[3] == 0.0f);
	CHECK(b2[4] == -1.0f);
	CHECK(b2[5] == 0.0f);
	CHECK(b2[6] == 1.0f);
	CHECK(b2[7] == 0.0f);
	CHECK(b2[8] == -1.0f);
	CHECK(b2[9] == 0.0f);
	CHECK(b2[10] == 1.0f);
	CHECK(b2[11] == 0.0f);
	CHECK(b2[12] == -1.0f);
	CHECK(b2[13] == 0.0f);
	CHECK(b2[14] == 1.0f);
	CHECK(b2[15] == 0.0f);
}

TEMPLATE_TEST_CASE("floats are initialized the same", "[float]", bool)
{
	using namespace weaver;

	const auto value = GENERATE(-1.0f, 0.0f, 1.0f);

	VectorScalar<float, 4> b0{ value };
	CHECK(b0[0] == value);
	CHECK(b0[1] == value);
	CHECK(b0[2] == value);
	CHECK(b0[3] == value);

	VectorScalar<float, 8> b1{ value };
	CHECK(b1[0] == value);
	CHECK(b1[1] == value);
	CHECK(b1[2] == value);
	CHECK(b1[3] == value);
	CHECK(b1[4] == value);
	CHECK(b1[5] == value);
	CHECK(b1[6] == value);
	CHECK(b1[7] == value);

	VectorScalar<float, 16> b2{ value };
	CHECK(b2[0] == value);
	CHECK(b2[1] == value);
	CHECK(b2[2] == value);
	CHECK(b2[3] == value);
	CHECK(b2[4] == value);
	CHECK(b2[5] == value);
	CHECK(b2[6] == value);
	CHECK(b2[7] == value);
	CHECK(b2[8] == value);
	CHECK(b2[9] == value);
	CHECK(b2[10] == value);
	CHECK(b2[11] == value);
	CHECK(b2[12] == value);
	CHECK(b2[13] == value);
	CHECK(b2[14] == value);
	CHECK(b2[15] == value);
}
