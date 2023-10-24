// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// FloatWeaver
#include <weaver/bool16_avx512.h>
#include <weaver/common.h>
#include <weaver/intrinsics_avx512.h>

namespace weaver {
	template<>
	class alignas(64) VectorScalar<int, 16> : protected VectorScalarBase<int, 16, SimdExt::AVX512>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(AVX512)

		WEAVER_SIMD_BINARY_OP(operator+, _mm512_add_epi32, 1)
		WEAVER_SIMD_BINARY_OP(operator-, _mm512_sub_epi32, 1)
		WEAVER_SIMD_BINARY_OP(operator*, _mm512_mullo_epi32, 1)

		WEAVER_SIMD_ASSIGN_OP(operator+=, _mm512_add_epi32, 1)
		WEAVER_SIMD_ASSIGN_OP(operator-=, _mm512_sub_epi32, 1)
		WEAVER_SIMD_ASSIGN_OP(operator*=, _mm512_mullo_epi32, 1)

		WEAVER_SIMD_COMP_OP(operator==, _mm512_cmpeq_epi32_mask, 1)
		WEAVER_SIMD_COMP_OP(operator!=, _mm512_cmpneq_epi32_mask, 1)
		WEAVER_SIMD_COMP_OP(operator<, _mm512_cmplt_epi32_mask, 1)
		WEAVER_SIMD_COMP_OP(operator<=, _mm512_cmple_epi32_mask, 1)
		WEAVER_SIMD_COMP_OP(operator>, _mm512_cmpgt_epi32_mask, 1)
		WEAVER_SIMD_COMP_OP(operator>=, _mm512_cmpge_epi32_mask, 1)

		WEAVER_SIMD_UNARY_OP(abs, _mm512_abs_epi32, 1)

		WEAVER_SIMD_BINARY_OP(operator&, _mm512_and_si512, 1)
		WEAVER_SIMD_BINARY_OP(operator|, _mm512_or_si512, 1)

		WEAVER_SIMD_BINARY_OP(min, _mm512_min_epi32, 1)
		WEAVER_SIMD_BINARY_OP(max, _mm512_max_epi32, 1)
	};

	WEAVER_STRONG_INLINE VectorScalar<int, 16> select(const VectorScalar<bool, 16>& mask, const VectorScalar<int, 16>& a, const VectorScalar<int, 16>& b)
	{
		return VectorScalar<int, 16>(
			_mm512_mask_blend_epi32(mask.get(0), b.get(0), a.get(0)));
	}
}
