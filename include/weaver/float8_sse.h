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
	class VectorScalar<float, 8> : protected VectorScalarBase<float, 8, SimdExt::SSE>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(SSE)

		WEAVER_SIMD_BINARY_OP(operator+, _mm_add_ps, 2)
		WEAVER_SIMD_BINARY_OP(operator-, _mm_sub_ps, 2)
		WEAVER_SIMD_BINARY_OP(operator*, _mm_mul_ps, 2)
		WEAVER_SIMD_BINARY_OP(operator/, _mm_div_ps, 2)

		WEAVER_SIMD_ASSIGN_OP(operator+=, _mm_add_ps, 2)
		WEAVER_SIMD_ASSIGN_OP(operator-=, _mm_sub_ps, 2)
		WEAVER_SIMD_ASSIGN_OP(operator*=, _mm_mul_ps, 2)
		WEAVER_SIMD_ASSIGN_OP(operator/=, _mm_div_ps, 2)

		WEAVER_SIMD_COMP_OP(operator==, _mm_cmpeq_ps, 2)
		WEAVER_SIMD_COMP_OP(operator!=, _mm_cmpneq_ps, 2)
		WEAVER_SIMD_COMP_OP(operator<, _mm_cmplt_ps, 2)
		WEAVER_SIMD_COMP_OP(operator<=, _mm_cmple_ps, 2)
		WEAVER_SIMD_COMP_OP(operator>, _mm_cmpgt_ps, 2)
		WEAVER_SIMD_COMP_OP(operator>=, _mm_cmpge_ps, 2)

		WEAVER_SIMD_UNARY_OP(abs, abs_f32, 2)
		WEAVER_SIMD_UNARY_OP(sgn, sgn_f32, 2)

		WEAVER_SIMD_UNARY_OP(sin, _mm_sin_ps, 2)
		WEAVER_SIMD_UNARY_OP(cos, _mm_cos_ps, 2)
		WEAVER_SIMD_UNARY_OP(acos, _mm_acos_ps, 2)

		WEAVER_SIMD_UNARY_OP(exp, _mm_exp_ps, 2)
		WEAVER_SIMD_UNARY_OP(log, _mm_log_ps, 2)
		WEAVER_SIMD_UNARY_OP(sqrt, _mm_sqrt_ps, 2)
		WEAVER_SIMD_UNARY_OP(rcp, _mmWEAVER_rcp_ps, 2)
		WEAVER_SIMD_UNARY_OP(rsqrt, _mmWEAVER_rsqrt_ps, 2)

		WEAVER_SIMD_QUERY_OP(isinf, _mm_isinf_ps, 2)

		WEAVER_SIMD_BINARY_OP(min, _mm_min_ps, 2)
		WEAVER_SIMD_BINARY_OP(max, _mm_max_ps, 2)

		WEAVER_SIMD_BINARY_REDUCTION_OP(dot, _mmWEAVER_dp_ps, details::add, 2)

		WEAVER_SIMD_UNARY_REDUCTION_OP(min, _mmWEAVER_hmin_ps, details::min, 2)
		WEAVER_SIMD_UNARY_REDUCTION_OP(max, _mmWEAVER_hmax_ps, details::max, 2)
	};

	WEAVER_STRONG_INLINE VectorScalar<float, 8> select(const VectorScalar<bool, 8>& mask, const VectorScalar<float, 8>& a, const VectorScalar<float, 8>& b) noexcept
	{
		return VectorScalar<float, 8>(
			blend_f32(b.get(0), a.get(0), mask.get(0)),
			blend_f32(b.get(1), a.get(1), mask.get(1)));
	}
}
