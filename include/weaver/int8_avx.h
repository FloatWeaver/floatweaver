// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// FloatWeaver
#include <weaver/bool8_avx.h>
#include <weaver/common.h>
#include <weaver/intrinsics_avx.h>

namespace weaver {
	template<>
	class alignas(32) VectorScalar<int, 8> : protected VectorScalarBase<int, 8, SimdExt::AVX>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(AVX)

	public:
		WEAVER_SIMD_BINARY_OP(operator+, _mmWEAVER_add_epi32, 1)
		WEAVER_SIMD_BINARY_OP(operator-, _mmWEAVER_sub_epi32, 1)
		WEAVER_SIMD_BINARY_OP(operator*, _mmWEAVER_mullo_epi32, 1)

	public:
		WEAVER_SIMD_ASSIGN_OP(operator+=, _mmWEAVER_add_epi32, 1)
		WEAVER_SIMD_ASSIGN_OP(operator-=, _mmWEAVER_sub_epi32, 1)
		WEAVER_SIMD_ASSIGN_OP(operator*=, _mmWEAVER_mullo_epi32, 1)

	public:
		WEAVER_SIMD_COMP_OP(operator==, _mmWEAVER_cmpeq_epi32, 1)
		WEAVER_SIMD_COMP_OP(operator!=, _mmWEAVER_cmpneq_epi32, 1)
		WEAVER_SIMD_COMP_OP(operator<, _mmWEAVER_cmplt_epi32, 1)
		WEAVER_SIMD_COMP_OP(operator<=, _mmWEAVER_cmple_epi32, 1)
		WEAVER_SIMD_COMP_OP(operator>, _mmWEAVER_cmpgt_epi32, 1)
		WEAVER_SIMD_COMP_OP(operator>=, _mmWEAVER_cmpge_epi32, 1)

	public:
		WEAVER_SIMD_UNARY_OP(abs, _mmWEAVER_abs_epi32, 1)

	public:
		WEAVER_SIMD_BINARY_OP(operator&, _mmWEAVER_and_si256, 1)
		WEAVER_SIMD_BINARY_OP(operator|, _mmWEAVER_or_si256, 1)

		WEAVER_SIMD_BINARY_OP(min, _mmWEAVER_min_epi32, 1)
		WEAVER_SIMD_BINARY_OP(max, _mmWEAVER_max_epi32, 1)
	};

	WEAVER_STRONG_INLINE VectorScalar<int, 8> select(const VectorScalar<bool, 8>& mask, const VectorScalar<int, 8>& a, const VectorScalar<int, 8>& b) noexcept
	{
		return VectorScalar<int, 8>(_mmWEAVER_xor_si256(b.get(0), _mmWEAVER_and_si256(_mm256_castps_si256(mask.get(0)), _mmWEAVER_xor_si256(b.get(0), a.get(0)))));
	}

	//WEAVER_STRONG_INLINE std::ostream& operator<<(std::ostream& s, const VectorScalar<int, 8>& rhs)
	//{
	//	alignas(32) int vars[8];
	//	_mm256_store_si256(reinterpret_cast<__m256i*>(vars + 0), rhs.get(0));
	//
	//	s << "'" << vars[0] << ", " << vars[1] << ", " << vars[2] << ", " << vars[3]
	//	  << vars[4] << ", " << vars[5] << ", " << vars[6] << ", " << vars[7] << "'";
	//
	//	return s;
	//}
}
