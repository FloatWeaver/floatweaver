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
#	include <weaver/common_ref.h>
#endif

namespace weaver
{
	using float4 = VectorScalar<float, 4>;
	using float8 = VectorScalar<float, 8>;
	using float16 = VectorScalar<float, 16>;
}

#include "common_float.h"

TEST_CASE("FloatN", "[float][operation]")
{
	WEAVER_SIMD_FLOATS;

	SECTION("Initialize to same value")
	{
		float4 f4{ 1 };
		float4 f4_2{ f4 };
		for (int i = 0; i < 4; i++)
		{
			CHECK(f4[i] == 1);
			CHECK(f4_2[i] == 1);
		}

		float8 f8{ 1 };
		float8 f8_2{ f8 };
		for (int i = 0; i < 8; i++)
		{
			CHECK(f8[i] == 1);
			CHECK(f8_2[i] == 1);
		}

		float16 f16{ 1 };
		float16 f16_2{ f16 };
		for (int i = 0; i < 16; i++)
		{
			CHECK(f16[i] == 1);
			CHECK(f16_2[i] == 1);
		}
	}

	SECTION("Initialize to different values")
	{
		for (int i = 0; i < 4; i++)
		{
			CHECK(f4_asc[i] == i + 1);
		}

		for (int i = 0; i < 8; i++)
		{
			CHECK(f8_asc[i] == i + 1);
		}

		for (int i = 0; i < 16; i++)
		{
			CHECK(f16_asc[i] == i + 1);
		}
	}

	SECTION("Assign")
	{
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

	SECTION("Addition")
	{
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

	SECTION("Subtraction")
	{
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
}
