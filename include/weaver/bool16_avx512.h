// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// FloatWeaver
#include <weaver/common.h>

namespace weaver {
	template<>
	class VectorScalar<bool, 16> : protected VectorScalarBase<bool, 16, SimdExt::AVX512>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(AVX512)

		WEAVER_SIMD_BINARY_OP(operator&&, _mm512_kand, 1)
		WEAVER_SIMD_BINARY_OP(operator||, _mm512_kor, 1)

		WEAVER_SIMD_ASSIGN_OP(operator&=, _mm512_kand, 1)
		WEAVER_SIMD_ASSIGN_OP(operator|=, _mm512_kor, 1)
	};

	WEAVER_STRONG_INLINE bool any(const VectorScalar<bool, 16>& b)
	{
		const auto mask = _cvtmask16_u32(b.get(0));
		return mask != 0;
	}

	WEAVER_STRONG_INLINE bool all(const VectorScalar<bool, 16>& b)
	{
		const auto mask = _cvtmask16_u32(b.get(0));
		return mask == 0xffff;
	}

	WEAVER_STRONG_INLINE bool none(const VectorScalar<bool, 16>& b)
	{
		const auto mask = _cvtmask16_u32(b.get(0));
		return mask == 0x0;
	}
}
