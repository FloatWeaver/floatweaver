// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// FloatWeaver
#include <weaver/bool4_sse.h>
#include <weaver/common.h>
#include <weaver/intrinsics_sse.h>

namespace weaver {
	template<>
	class VectorScalar<int, 4> : protected VectorScalarBase<int, 4, SimdExt::SSE>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(SSE)

		WEAVER_SIMD_BINARY_OP(operator+, _mm_add_epi32, 1)
		WEAVER_SIMD_BINARY_OP(operator-, _mm_sub_epi32, 1)
		WEAVER_SIMD_BINARY_OP(operator*, _mmWEAVER_mullo_epi32, 1)

		WEAVER_SIMD_ASSIGN_OP(operator+=, _mm_add_epi32, 1)
		WEAVER_SIMD_ASSIGN_OP(operator-=, _mm_sub_epi32, 1)
		WEAVER_SIMD_ASSIGN_OP(operator*=, _mmWEAVER_mullo_epi32, 1)

		WEAVER_SIMD_COMP_OP(operator==, _mm_cmpeq_epi32, 1)
		WEAVER_SIMD_COMP_OP(operator!=, _mm_cmpneq_epi32, 1)
		WEAVER_SIMD_COMP_OP(operator<, _mm_cmplt_epi32, 1)
		WEAVER_SIMD_COMP_OP(operator<=, _mm_cmple_epi32, 1)
		WEAVER_SIMD_COMP_OP(operator>, _mm_cmpgt_epi32, 1)
		WEAVER_SIMD_COMP_OP(operator>=, _mm_cmpge_epi32, 1)

		WEAVER_SIMD_UNARY_OP(abs, abs_s32, 1)

		WEAVER_SIMD_BINARY_OP(operator&, _mm_and_si128, 1)
		WEAVER_SIMD_BINARY_OP(operator|, _mm_or_si128, 1)

		WEAVER_SIMD_BINARY_OP(min, min_s32, 1)
		WEAVER_SIMD_BINARY_OP(max, max_s32, 1)
	};

	WEAVER_STRONG_INLINE VectorScalar<int, 4> select(const VectorScalar<bool, 4>& mask, const VectorScalar<int, 4>& a, const VectorScalar<int, 4>& b) noexcept
	{
		return VectorScalar<int, 4>(blend_s32(b.get(0), a.get(0), mask.get(0)));
	}
}
