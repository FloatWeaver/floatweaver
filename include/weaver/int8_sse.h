// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// FloatWeaver
#include <weaver/bool8_sse.h>
#include <weaver/common.h>
#include <weaver/intrinsics_sse.h>

namespace weaver {
	template<>
	class VectorScalar<int, 8> : protected VectorScalarBase<int, 8, SimdExt::SSE>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(SSE)

	public:
		WEAVER_SIMD_BINARY_OP(operator+, _mm_add_epi32, 2)
		WEAVER_SIMD_BINARY_OP(operator-, _mm_sub_epi32, 2)
		WEAVER_SIMD_BINARY_OP(operator*, _mmWEAVER_mullo_epi32, 2)

	public:
		WEAVER_SIMD_ASSIGN_OP(operator+=, _mm_add_epi32, 2)
		WEAVER_SIMD_ASSIGN_OP(operator-=, _mm_sub_epi32, 2)
		WEAVER_SIMD_ASSIGN_OP(operator*=, _mmWEAVER_mullo_epi32, 2)

	public:
		WEAVER_SIMD_COMP_OP(operator==, _mm_cmpeq_epi32, 2)
		WEAVER_SIMD_COMP_OP(operator!=, _mm_cmpneq_epi32, 2)
		WEAVER_SIMD_COMP_OP(operator<, _mm_cmplt_epi32, 2)
		WEAVER_SIMD_COMP_OP(operator<=, _mm_cmple_epi32, 2)
		WEAVER_SIMD_COMP_OP(operator>, _mm_cmpgt_epi32, 2)
		WEAVER_SIMD_COMP_OP(operator>=, _mm_cmpge_epi32, 2)

	public:
		WEAVER_SIMD_UNARY_OP(abs, abs_s32, 2)

	public:
		WEAVER_SIMD_BINARY_OP(operator&, _mm_and_si128, 2)
		WEAVER_SIMD_BINARY_OP(operator|, _mm_or_si128, 2)

		WEAVER_SIMD_BINARY_OP(min, min_s32, 2)
		WEAVER_SIMD_BINARY_OP(max, max_s32, 2)
	};

	WEAVER_STRONG_INLINE VectorScalar<int, 8> select(const VectorScalar<bool, 8>& mask, const VectorScalar<int, 8>& a, const VectorScalar<int, 8>& b) noexcept
	{
		return VectorScalar<int, 8>(
			blend_s32(b.get(0), a.get(0), mask.get(0)),
			blend_s32(b.get(1), a.get(1), mask.get(1)));
	}

	//WEAVER_STRONG_INLINE std::ostream& operator<<(std::ostream& s, const VectorScalar<int, 8>& rhs)
	//{
	//	alignas(16) int vars[8];
	//	_mm_store_si128(reinterpret_cast<__m128i*>(vars + 0), rhs.get(0));
	//	_mm_store_si128(reinterpret_cast<__m128i*>(vars + 4), rhs.get(1));
	//
	//	s << "'" << vars[0] << ", " << vars[1] << ", " << vars[2] << ", " << vars[3]
	//	  << vars[4] << ", " << vars[5] << ", " << vars[6] << ", " << vars[7] << "'";
	//
	//	return s;
	//}
}
