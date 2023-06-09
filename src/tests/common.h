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
