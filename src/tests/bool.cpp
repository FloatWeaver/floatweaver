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
#	include <weaver/bool4_sse.h>
#	include <weaver/bool8_avx.h>
#	include <weaver/bool16_avx512.h>
#elif defined WEAVER_VECTORIZE_AVX
#	include <weaver/bool4_sse.h>
#	include <weaver/bool8_avx.h>
#	include <weaver/bool16_avx.h>
#elif defined WEAVER_VECTORIZE_SSE
#	include <weaver/bool4_sse.h>
#	include <weaver/bool8_sse.h>
#	include <weaver/bool16_sse.h>
#elif defined WEAVER_VECTORIZE_NEON
#	include <weaver/bool4_neon.h>
#	include <weaver/bool8_neon.h>
#	include <weaver/bool16_neon.h>
#else
#	include <weaver/bool4_ref.h>
#	include <weaver/bool8_ref.h>
#	include <weaver/bool16_ref.h>
#endif

TEST_CASE("booleans are initialized", "[bool]")
{
	using namespace weaver;

	VectorScalar<bool, 4> b0{true, false, true, false};
	CHECK(b0[0] == true);
	CHECK(b0[1] == false);
	CHECK(b0[2] == true);
	CHECK(b0[3] == false);

	VectorScalar<bool, 8> b1{ true, false, true, false, true, false, true, false };
	CHECK(b1[0] == true);
	CHECK(b1[1] == false);
	CHECK(b1[2] == true);
	CHECK(b1[3] == false);
	CHECK(b1[4] == true);
	CHECK(b1[5] == false);
	CHECK(b1[6] == true);
	CHECK(b1[7] == false);

	VectorScalar<bool, 16> b2{ true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false };
	CHECK(b2[0] == true);
	CHECK(b2[1] == false);
	CHECK(b2[2] == true);
	CHECK(b2[3] == false);
	CHECK(b2[4] == true);
	CHECK(b2[5] == false);
	CHECK(b2[6] == true);
	CHECK(b2[7] == false);
	CHECK(b2[8] == true);
	CHECK(b2[9] == false);
	CHECK(b2[10] == true);
	CHECK(b2[11] == false);
	CHECK(b2[12] == true);
	CHECK(b2[13] == false);
	CHECK(b2[14] == true);
	CHECK(b2[15] == false);
}

TEMPLATE_TEST_CASE("booleans are initialized the same", "[bool]", bool)
{
	using namespace weaver;

	const auto value = GENERATE(true, false);

	VectorScalar<bool, 4> b0{ value };
	CHECK(b0[0] == value);
	CHECK(b0[1] == value);
	CHECK(b0[2] == value);
	CHECK(b0[3] == value);

	VectorScalar<bool, 8> b1{ value };
	CHECK(b1[0] == value);
	CHECK(b1[1] == value);
	CHECK(b1[2] == value);
	CHECK(b1[3] == value);
	CHECK(b1[4] == value);
	CHECK(b1[5] == value);
	CHECK(b1[6] == value);
	CHECK(b1[7] == value);

	VectorScalar<bool, 16> b2{ value };
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
