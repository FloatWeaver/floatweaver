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
	class alignas(32) VectorScalar<float, 16> : protected VectorScalarBase<float, 16, SimdExt::AVX>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(AVX)

		WEAVER_SIMD_BINARY_OP(operator+, _mm256_add_ps, 2)
		WEAVER_SIMD_BINARY_OP(operator-, _mm256_sub_ps, 2)
		WEAVER_SIMD_BINARY_OP(operator*, _mm256_mul_ps, 2)
		WEAVER_SIMD_BINARY_OP(operator/, _mm256_div_ps, 2)

		WEAVER_SIMD_ASSIGN_OP(operator+=, _mm256_add_ps, 2)
		WEAVER_SIMD_ASSIGN_OP(operator-=, _mm256_sub_ps, 2)
		WEAVER_SIMD_ASSIGN_OP(operator*=, _mm256_mul_ps, 2)
		WEAVER_SIMD_ASSIGN_OP(operator/=, _mm256_div_ps, 2)

		WEAVER_SIMD_COMP_OP(operator==, _mm256_cmpeq_ps, 2)
		WEAVER_SIMD_COMP_OP(operator!=, _mm256_cmpneq_ps, 2)
		WEAVER_SIMD_COMP_OP(operator<, _mm256_cmplt_ps, 2)
		WEAVER_SIMD_COMP_OP(operator<=, _mm256_cmple_ps, 2)
		WEAVER_SIMD_COMP_OP(operator>, _mm256_cmpgt_ps, 2)
		WEAVER_SIMD_COMP_OP(operator>=, _mm256_cmpge_ps, 2)

		WEAVER_SIMD_UNARY_OP(abs, _mm256_abs_ps, 2)
		WEAVER_SIMD_UNARY_OP(sgn, _mm256_sgn_ps, 2)

		WEAVER_SIMD_UNARY_OP(sin, _mm256_sin_ps, 2)
		WEAVER_SIMD_UNARY_OP(cos, _mm256_cos_ps, 2)
		WEAVER_SIMD_UNARY_OP(acos, _mm256_acos_ps, 2)

		WEAVER_SIMD_UNARY_OP(exp, _mm256_exp_ps, 2)
		WEAVER_SIMD_UNARY_OP(log, _mm256_log_ps, 2)
		WEAVER_SIMD_UNARY_OP(sqrt, _mm256_sqrt_ps, 2)
		WEAVER_SIMD_UNARY_OP(rcp, _mmWEAVER_rcp_ps, 2)
		WEAVER_SIMD_UNARY_OP(rsqrt, _mmWEAVER_rsqrt_ps, 2)

		WEAVER_SIMD_QUERY_OP(isinf, _mm256_isinf_ps, 2)

		WEAVER_SIMD_BINARY_OP(min, _mm256_min_ps, 2)
		WEAVER_SIMD_BINARY_OP(max, _mm256_max_ps, 2)

		WEAVER_SIMD_BINARY_REDUCTION_OP(dot, _mmWEAVER_dp_ps, details::add, 2)

		WEAVER_SIMD_UNARY_REDUCTION_OP(min, _mmWEAVER_hmin_ps, details::min, 2)
		WEAVER_SIMD_UNARY_REDUCTION_OP(max, _mmWEAVER_hmax_ps, details::max, 2)
	};

	WEAVER_STRONG_INLINE VectorScalar<float, 16> select(const VectorScalar<bool, 16>& mask, const VectorScalar<float, 16>& a, const VectorScalar<float, 16>& b) noexcept
	{
		// (((b ^ a) & mask)^b)
		return VectorScalar<float, 16>(
			_mm256_blendv_ps(b.get(0), a.get(0), mask.get(0)),
			_mm256_blendv_ps(b.get(1), a.get(1), mask.get(1)));
	}
}
