// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// FloatWeaver
#include <weaver/bool16_avx.h>
#include <weaver/common.h>
#include <weaver/intrinsics_avx.h>

namespace weaver {
	template<>
	class alignas(32) VectorScalar<int, 16> : protected VectorScalarBase<int, 16, SimdExt::AVX>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(AVX)

		WEAVER_SIMD_BINARY_OP(operator+, _mmWEAVER_add_epi32, 2)
		WEAVER_SIMD_BINARY_OP(operator-, _mmWEAVER_sub_epi32, 2)
		WEAVER_SIMD_BINARY_OP(operator*, _mmWEAVER_mullo_epi32, 2)

		WEAVER_SIMD_ASSIGN_OP(operator+=, _mmWEAVER_add_epi32, 2)
		WEAVER_SIMD_ASSIGN_OP(operator-=, _mmWEAVER_sub_epi32, 2)
		WEAVER_SIMD_ASSIGN_OP(operator*=, _mmWEAVER_mullo_epi32, 2)

		WEAVER_SIMD_COMP_OP(operator==, _mmWEAVER_cmpeq_epi32, 2)
		WEAVER_SIMD_COMP_OP(operator!=, _mmWEAVER_cmpneq_epi32, 2)
		WEAVER_SIMD_COMP_OP(operator<, _mmWEAVER_cmplt_epi32, 2)
		WEAVER_SIMD_COMP_OP(operator<=, _mmWEAVER_cmple_epi32, 2)
		WEAVER_SIMD_COMP_OP(operator>, _mmWEAVER_cmpgt_epi32, 2)
		WEAVER_SIMD_COMP_OP(operator>=, _mmWEAVER_cmpge_epi32, 2)

		WEAVER_SIMD_UNARY_OP(abs, _mmWEAVER_abs_epi32, 2)

		WEAVER_SIMD_BINARY_OP(operator&, _mmWEAVER_and_si256, 2)
		WEAVER_SIMD_BINARY_OP(operator|, _mmWEAVER_or_si256, 2)

		WEAVER_SIMD_BINARY_OP(min, _mmWEAVER_min_epi32, 2)
		WEAVER_SIMD_BINARY_OP(max, _mmWEAVER_max_epi32, 2)
	};

	WEAVER_STRONG_INLINE VectorScalar<int, 16> select(const VectorScalar<bool, 16>& mask, const VectorScalar<int, 16>& a, const VectorScalar<int, 16>& b) noexcept
	{
		return VectorScalar<int, 16>(
			_mmWEAVER_xor_si256(b.get(0), _mmWEAVER_and_si256(_mm256_castps_si256(mask.get(0)), _mmWEAVER_xor_si256(b.get(0), a.get(0)))),
			_mmWEAVER_xor_si256(b.get(1), _mmWEAVER_and_si256(_mm256_castps_si256(mask.get(1)), _mmWEAVER_xor_si256(b.get(1), a.get(1)))));
	}
}
