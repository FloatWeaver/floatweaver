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
	class alignas(64) VectorScalar<float, 16> : protected VectorScalarBase<float, 16, SimdExt::AVX512>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(AVX512)

		WEAVER_SIMD_BINARY_OP(operator+, _mm512_add_ps, 1)
		WEAVER_SIMD_BINARY_OP(operator-, _mm512_sub_ps, 1)
		WEAVER_SIMD_BINARY_OP(operator*, _mm512_mul_ps, 1)
		WEAVER_SIMD_BINARY_OP(operator/, _mm512_div_ps, 1)

		WEAVER_SIMD_ASSIGN_OP(operator+=, _mm512_add_ps, 1)
		WEAVER_SIMD_ASSIGN_OP(operator-=, _mm512_sub_ps, 1)
		WEAVER_SIMD_ASSIGN_OP(operator*=, _mm512_mul_ps, 1)
		WEAVER_SIMD_ASSIGN_OP(operator/=, _mm512_div_ps, 1)

		WEAVER_SIMD_COMP_OP(operator==, _mm512_cmpeq_ps, 1)
		WEAVER_SIMD_COMP_OP(operator!=, _mm512_cmpneq_ps, 1)
		WEAVER_SIMD_COMP_OP(operator<, _mm512_cmplt_ps, 1)
		WEAVER_SIMD_COMP_OP(operator<=, _mm512_cmple_ps, 1)
		WEAVER_SIMD_COMP_OP(operator>, _mm512_cmpgt_ps, 1)
		WEAVER_SIMD_COMP_OP(operator>=, _mm512_cmpge_ps, 1)

		WEAVER_SIMD_UNARY_OP(abs, _mm512_abs_ps, 1)
		WEAVER_SIMD_UNARY_OP(sgn, _mm512_sgn_ps, 1)

		WEAVER_SIMD_UNARY_OP(sin, _mm512_sin_ps, 1)
		WEAVER_SIMD_UNARY_OP(cos, _mm512_cos_ps, 1)
		WEAVER_SIMD_UNARY_OP(acos, _mm512_acos_ps, 1)

		WEAVER_SIMD_UNARY_OP(exp, _mm512_exp_ps, 1)
		WEAVER_SIMD_UNARY_OP(log, _mm512_log_ps, 1)
		WEAVER_SIMD_UNARY_OP(sqrt, _mm512_sqrt_ps, 1)
		WEAVER_SIMD_UNARY_OP(rcp, _mmWEAVER_rcp_ps, 1)
		WEAVER_SIMD_UNARY_OP(rsqrt, _mmWEAVER_rsqrt_ps, 1)

		WEAVER_SIMD_QUERY_OP(isinf, _mm512_isinf_ps, 1)

		WEAVER_SIMD_BINARY_OP(min, _mm512_min_ps, 1)
		WEAVER_SIMD_BINARY_OP(max, _mm512_max_ps, 1)

		WEAVER_SIMD_BINARY_REDUCTION_OP(dot, _mmWEAVER_dp_ps, WEAVER_UNUSED, 1)

		WEAVER_SIMD_UNARY_REDUCTION_OP(min, _mmWEAVER_hmin_ps, WEAVER_UNUSED, 1)
		WEAVER_SIMD_UNARY_REDUCTION_OP(max, _mmWEAVER_hmax_ps, WEAVER_UNUSED, 1)
	};

	WEAVER_STRONG_INLINE VectorScalar<float, 16> select(const VectorScalar<bool, 16>& mask, const VectorScalar<float, 16>& a, const VectorScalar<float, 16>& b)
	{
		// (((b ^ a) & mask)^b)
		return VectorScalar<float, 16>(
			_mm512_mask_blend_ps(mask.get(0), b.get(0), a.get(0)));
	}
}
