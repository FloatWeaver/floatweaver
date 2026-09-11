// Copyright 2023 Basil Fierz
//
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

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
