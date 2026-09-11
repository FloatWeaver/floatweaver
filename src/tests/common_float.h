// Copyright 2023 Basil Fierz
//
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

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
