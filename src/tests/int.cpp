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
#	include <weaver/int4_sse.h>
#	include <weaver/int8_avx.h>
#	include <weaver/int16_avx512.h>
#elif defined WEAVER_VECTORIZE_AVX
#	include <weaver/int4_sse.h>
#	include <weaver/int8_avx.h>
#	include <weaver/int16_avx.h>
#elif defined WEAVER_VECTORIZE_SSE
#	include <weaver/int4_sse.h>
#	include <weaver/int8_sse.h>
#	include <weaver/int16_sse.h>
#elif defined WEAVER_VECTORIZE_NEON
#	include <weaver/int4_neon.h>
#	include <weaver/int8_neon.h>
#	include <weaver/int16_neon.h>
#else
#	include <weaver/int4_ref.h>
#	include <weaver/int8_ref.h>
#	include <weaver/int16_ref.h>
#endif

TEST_CASE("ints are initialized", "[int]")
{
	using namespace weaver;

	VectorScalar<int, 4> b0{-1, 0, 1, 0};
	CHECK(b0[0] == -1);
	CHECK(b0[1] == 0);
	CHECK(b0[2] == 1);
	CHECK(b0[3] == 0);

	VectorScalar<int, 8> b1{ -1, 0, 1, 0, -1, 0, 1, 0 };
	CHECK(b1[0] == -1);
	CHECK(b1[1] == 0);
	CHECK(b1[2] == 1);
	CHECK(b1[3] == 0);
	CHECK(b1[4] == -1);
	CHECK(b1[5] == 0);
	CHECK(b1[6] == 1);
	CHECK(b1[7] == 0);

	VectorScalar<int, 16> b2{ -1, 0, 1, 0, -1, 0, 1, 0, -1, 0, 1, 0, -1, 0, 1, 0 };
	CHECK(b2[0] == -1);
	CHECK(b2[1] == 0);
	CHECK(b2[2] == 1);
	CHECK(b2[3] == 0);
	CHECK(b2[4] == -1);
	CHECK(b2[5] == 0);
	CHECK(b2[6] == 1);
	CHECK(b2[7] == 0);
	CHECK(b2[8] == -1);
	CHECK(b2[9] == 0);
	CHECK(b2[10] == 1);
	CHECK(b2[11] == 0);
	CHECK(b2[12] == -1);
	CHECK(b2[13] == 0);
	CHECK(b2[14] == 1);
	CHECK(b2[15] == 0);
}

TEMPLATE_TEST_CASE("ints are initialized the same", "[int]", bool)
{
	using namespace weaver;

	const auto value = GENERATE(-1, 0, 1);

	VectorScalar<int, 4> b0{ value };
	CHECK(b0[0] == value);
	CHECK(b0[1] == value);
	CHECK(b0[2] == value);
	CHECK(b0[3] == value);

	VectorScalar<int, 8> b1{ value };
	CHECK(b1[0] == value);
	CHECK(b1[1] == value);
	CHECK(b1[2] == value);
	CHECK(b1[3] == value);
	CHECK(b1[4] == value);
	CHECK(b1[5] == value);
	CHECK(b1[6] == value);
	CHECK(b1[7] == value);

	VectorScalar<int, 16> b2{ value };
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
