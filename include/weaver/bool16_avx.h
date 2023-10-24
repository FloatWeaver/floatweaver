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
	class alignas(32) VectorScalar<bool, 16> : protected VectorScalarBase<bool, 16, SimdExt::AVX>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(AVX)
		explicit WEAVER_STRONG_INLINE VectorScalar(__m256i I8_0, __m256i I8_1) noexcept
		{
			_data[0] = _mm256_castsi256_ps(I8_0);
			_data[1] = _mm256_castsi256_ps(I8_1);
		}

		WEAVER_SIMD_BINARY_OP(operator&&, _mm256_and_ps, 2)
		WEAVER_SIMD_BINARY_OP(operator||, _mm256_or_ps, 2)

		WEAVER_SIMD_ASSIGN_OP(operator&=, _mm256_and_ps, 2)
		WEAVER_SIMD_ASSIGN_OP(operator|=, _mm256_or_ps, 2)
	};

	WEAVER_STRONG_INLINE bool any(const VectorScalar<bool, 16>& b) noexcept
	{
		int mask = _mm256_movemask_ps(b.get(1)) << 8;
		mask |= _mm256_movemask_ps(b.get(0));

		return mask != 0;
	}

	WEAVER_STRONG_INLINE bool all(const VectorScalar<bool, 16>& b) noexcept
	{
		int mask = _mm256_movemask_ps(b.get(1)) << 8;
		mask |= _mm256_movemask_ps(b.get(0));

		return static_cast<unsigned int>(mask) == 0xffff;
	}

	WEAVER_STRONG_INLINE bool none(const VectorScalar<bool, 16>& b) noexcept
	{
		int mask = _mm256_movemask_ps(b.get(1)) << 8;
		mask |= _mm256_movemask_ps(b.get(0));

		return static_cast<unsigned int>(mask) == 0x0;
	}
}
