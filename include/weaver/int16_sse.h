// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// FloatWeaver
#include <weaver/bool16_sse.h>
#include <weaver/common.h>
#include <weaver/intrinsics_sse.h>

namespace weaver {
	template<>
	class VectorScalar<int, 16> : protected VectorScalarBase<int, 16, SimdExt::SSE>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(SSE)

		WEAVER_SIMD_BINARY_OP(operator+, _mm_add_epi32, 4)
		WEAVER_SIMD_BINARY_OP(operator-, _mm_sub_epi32, 4)
		WEAVER_SIMD_BINARY_OP(operator*, _mmWEAVER_mullo_epi32, 4)

		WEAVER_SIMD_ASSIGN_OP(operator+=, _mm_add_epi32, 4)
		WEAVER_SIMD_ASSIGN_OP(operator-=, _mm_sub_epi32, 4)
		WEAVER_SIMD_ASSIGN_OP(operator*=, _mmWEAVER_mullo_epi32, 4)

		WEAVER_SIMD_COMP_OP(operator==, _mm_cmpeq_epi32, 4)
		WEAVER_SIMD_COMP_OP(operator!=, _mm_cmpneq_epi32, 4)
		WEAVER_SIMD_COMP_OP(operator<, _mm_cmplt_epi32, 4)
		WEAVER_SIMD_COMP_OP(operator<=, _mm_cmple_epi32, 4)
		WEAVER_SIMD_COMP_OP(operator>, _mm_cmpgt_epi32, 4)
		WEAVER_SIMD_COMP_OP(operator>=, _mm_cmpge_epi32, 4)

		WEAVER_SIMD_UNARY_OP(abs, abs_s32, 4)

		WEAVER_SIMD_BINARY_OP(operator&, _mm_and_si128, 4)
		WEAVER_SIMD_BINARY_OP(operator|, _mm_or_si128, 4)

		WEAVER_SIMD_BINARY_OP(min, min_s32, 4)
		WEAVER_SIMD_BINARY_OP(max, max_s32, 4)
	};

	WEAVER_STRONG_INLINE VectorScalar<int, 16> select(const VectorScalar<bool, 16>& mask, const VectorScalar<int, 16>& a, const VectorScalar<int, 16>& b) noexcept
	{
		return VectorScalar<int, 16>(
			blend_s32(b.get(0), a.get(0), mask.get(0)),
			blend_s32(b.get(1), a.get(1), mask.get(1)),
			blend_s32(b.get(2), a.get(2), mask.get(2)),
			blend_s32(b.get(3), a.get(3), mask.get(3)));
	}
}
