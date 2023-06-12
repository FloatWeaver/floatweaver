// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// Catch2
#include <catch2/catch_all.hpp>

// FloatWeaver
#include <weaver/memory.h>

#include "common.h"

TEST_CASE("Interleave floatN", "[memory][float]")
{
	using weaver::interleave;

	WEAVER_SIMD_FLOATS;

	const auto f4 = interleave(f4_asc, f4_desc);
	for (int i = 0; i < 2; i++)
	{
		CHECK(f4[0][2 * i + 0] == f4_asc[i]);
		CHECK(f4[0][2 * i + 1] == f4_desc[i]);

		CHECK(f4[1][2 * i + 0] == f4_asc[i + 2]);
		CHECK(f4[1][2 * i + 1] == f4_desc[i + 2]);
	}

	const auto f8 = interleave(f8_asc, f8_desc);
	for (int i = 0; i < 4; i++)
	{
		CHECK(f8[0][2 * i + 0] == f8_asc[i]);
		CHECK(f8[0][2 * i + 1] == f8_desc[i]);

		CHECK(f8[1][2 * i + 0] == f8_asc[i + 4]);
		CHECK(f8[1][2 * i + 1] == f8_desc[i + 4]);
	}

	const auto f16 = interleave(f16_asc, f16_desc);
	for (int i = 0; i < 8; i++)
	{
		CHECK(f16[0][2 * i + 0] == f16_asc[i]);
		CHECK(f16[0][2 * i + 1] == f16_desc[i]);

		CHECK(f16[1][2 * i + 0] == f16_asc[i + 8]);
		CHECK(f16[1][2 * i + 1] == f16_desc[i + 8]);
	}
}
