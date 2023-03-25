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
	class alignas(32) VectorScalar<bool, 8> : protected VectorScalarBase<bool, 8, SimdExt::AVX>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(AVX)
		explicit WEAVER_STRONG_INLINE VectorScalar(__m256i I8) noexcept { set(_mm256_castsi256_ps(I8)); }

	public:
		WEAVER_SIMD_BINARY_OP(operator&&, _mm256_and_ps, 1)
		WEAVER_SIMD_BINARY_OP(operator||, _mm256_or_ps, 1)

		WEAVER_SIMD_ASSIGN_OP(operator&=, _mm256_and_ps, 1)
		WEAVER_SIMD_ASSIGN_OP(operator|=, _mm256_or_ps, 1)
	};

	WEAVER_STRONG_INLINE bool any(const VectorScalar<bool, 8>& b) noexcept
	{
		return _mm256_movemask_ps(b.get(0)) != 0;
	}

	WEAVER_STRONG_INLINE bool all(const VectorScalar<bool, 8>& b) noexcept
	{
		return static_cast<unsigned int>(_mm256_movemask_ps(b.get(0))) == 0xff;
	}

	WEAVER_STRONG_INLINE bool none(const VectorScalar<bool, 8>& b) noexcept
	{
		return static_cast<unsigned int>(_mm256_movemask_ps(b.get(0))) == 0x0;
	}
}
