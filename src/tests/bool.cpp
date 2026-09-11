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

namespace weaver
{
	using bool4 = VectorScalar<bool, 4>;
	using bool8 = VectorScalar<bool, 8>;
	using bool16 = VectorScalar<bool, 16>;
}

#include "common_bool.h"

TEST_CASE("BoolN", "[bool][operation]")
{
	WEAVER_SIMD_BOOLS;

	using weaver::all;
	using weaver::none;

	SECTION("Initialize to same value")
	{
		CHECK(all(bool4{ true }));
		CHECK(all(bool8{ true }));
		CHECK(all(bool16{ true }));

		CHECK(none(bool4{ false }));
		CHECK(none(bool8{ false }));
		CHECK(none(bool16{ false }));
	}

	SECTION("Initialize to different values")
	{
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

	SECTION("Assign")
	{
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

	SECTION("And")
	{
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

	SECTION("Or")
	{
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
}
