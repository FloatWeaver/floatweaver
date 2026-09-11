// Copyright 2023 Basil Fierz
//
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

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
