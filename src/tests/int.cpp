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
#	include <weaver/common_ref.h>
#endif

namespace weaver
{
	using int4 = VectorScalar<int, 4>;
	using int8 = VectorScalar<int, 8>;
	using int16 = VectorScalar<int, 16>;
}

#include "common_int.h"

TEST_CASE("IntN", "[int][operation]")
{
	WEAVER_SIMD_INTS;

	SECTION("Initialize to same value")
	{
		int4 i4{ 1 };
		int4 i4_2{ i4 };
		for (int i = 0; i < 4; i++)
		{
			CHECK(i4[i] == 1);
			CHECK(i4_2[i] == 1);
		}

		int8 i8{ 1 };
		int8 i8_2{ i8 };
		for (int i = 0; i < 8; i++)
		{
			CHECK(i8[i] == 1);
			CHECK(i8_2[i] == 1);
		}

		int16 i16{ 1 };
		int16 i16_2{ i16 };
		for (int i = 0; i < 16; i++)
		{
			CHECK(i16[i] == 1);
			CHECK(i16_2[i] == 1);
		}
	}

	SECTION("Initialize to different values")
	{
		for (int i = 0; i < 4; i++)
		{
			CHECK(i4_asc[i] == i + 1);
		}

		for (int i = 0; i < 8; i++)
		{
			CHECK(i8_asc[i] == i + 1);
		}

		for (int i = 0; i < 16; i++)
		{
			CHECK(i16_asc[i] == i + 1);
		}
	}

	SECTION("Assign")
	{
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

	SECTION("Addition")
	{
		int4 i4 = i4_asc + i4_asc;
		for (int i = 0; i < 4; i++)
		{
			CHECK(i4[i] == i4_asc[i] + i4_asc[i]);
		}

		int8 i8 = i8_asc + i8_asc;
		for (int i = 0; i < 8; i++)
		{
			CHECK(i8[i] == i8_asc[i] + i8_asc[i]);
		}

		int16 i16 = i16_asc + i16_asc;
		for (int i = 0; i < 16; i++)
		{
			CHECK(i16[i] == i16_asc[i] + i16_asc[i]);
		}
	}

	SECTION("Subtraction")
	{
		int4 i4 = i4_asc - i4_asc;
		for (int i = 0; i < 4; i++)
		{
			CHECK(i4[i] == 0);
		}

		int8 i8 = i8_asc - i8_asc;
		for (int i = 0; i < 8; i++)
		{
			CHECK(i8[i] == 0);
		}

		int16 i16 = i16_asc - i16_asc;
		for (int i = 0; i < 16; i++)
		{
			CHECK(i16[i] == 0);
		}
	}
}
