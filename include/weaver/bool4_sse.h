// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// FloatWeaver
#include <weaver/common.h>

namespace weaver
{
	template<>
	class alignas(16) VectorScalar<bool, 4> : protected VectorScalarBase<bool, 4, SimdExt::SSE>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(SSE)
		explicit WEAVER_STRONG_INLINE VectorScalar(__m128i I4) noexcept { set(_mm_castsi128_ps(I4)); }

	public:
		WEAVER_SIMD_BINARY_OP(operator&&, _mm_and_ps, 1)
		WEAVER_SIMD_BINARY_OP(operator||, _mm_or_ps, 1)

		WEAVER_SIMD_ASSIGN_OP(operator&=, _mm_and_ps, 1)
		WEAVER_SIMD_ASSIGN_OP(operator|=, _mm_or_ps, 1)
	};

	WEAVER_STRONG_INLINE bool any(const VectorScalar<bool, 4>& b) noexcept
	{
		return _mm_movemask_ps(b.get(0)) != 0;
	}

	WEAVER_STRONG_INLINE bool all(const VectorScalar<bool, 4>& b) noexcept
	{
		return static_cast<unsigned int>(_mm_movemask_ps(b.get(0))) == 0xf;
	}

	WEAVER_STRONG_INLINE bool none(const VectorScalar<bool, 4>& b) noexcept
	{
		return static_cast<unsigned int>(_mm_movemask_ps(b.get(0))) == 0x0;
	}
}
