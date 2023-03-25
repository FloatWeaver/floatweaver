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
	class alignas(16) VectorScalar<bool, 16> : protected VectorScalarBase<bool, 16, SimdExt::SSE>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(SSE)
		explicit WEAVER_STRONG_INLINE VectorScalar(__m128i I4_0, __m128i I4_1, __m128i I4_2, const __m128i& I4_3) noexcept
		{
			set(_mm_castsi128_ps(I4_0), _mm_castsi128_ps(I4_1), _mm_castsi128_ps(I4_2), _mm_castsi128_ps(I4_3));
		}

	public:
		WEAVER_SIMD_BINARY_OP(operator&&, _mm_and_ps, 4)
		WEAVER_SIMD_BINARY_OP(operator||, _mm_or_ps, 4)

		WEAVER_SIMD_ASSIGN_OP(operator&=, _mm_and_ps, 4)
		WEAVER_SIMD_ASSIGN_OP(operator|=, _mm_or_ps, 4)
	};

	WEAVER_STRONG_INLINE bool any(const VectorScalar<bool, 16>& b) noexcept
	{
		int mask = _mm_movemask_ps(b.get(3)) << 12;
		mask |= _mm_movemask_ps(b.get(2)) << 8;
		mask |= _mm_movemask_ps(b.get(1)) << 4;
		mask |= _mm_movemask_ps(b.get(0));

		return mask != 0;
	}

	WEAVER_STRONG_INLINE bool all(const VectorScalar<bool, 16>& b) noexcept
	{
		int mask = _mm_movemask_ps(b.get(3)) << 12;
		mask |= _mm_movemask_ps(b.get(2)) << 8;
		mask |= _mm_movemask_ps(b.get(1)) << 4;
		mask |= _mm_movemask_ps(b.get(0));

		return static_cast<unsigned int>(mask) == 0xffff;
	}

	WEAVER_STRONG_INLINE bool none(const VectorScalar<bool, 16>& b) noexcept
	{
		int mask = _mm_movemask_ps(b.get(3)) << 12;
		mask |= _mm_movemask_ps(b.get(2)) << 8;
		mask |= _mm_movemask_ps(b.get(1)) << 4;
		mask |= _mm_movemask_ps(b.get(0));

		return static_cast<unsigned int>(mask) == 0x0;
	}
}
