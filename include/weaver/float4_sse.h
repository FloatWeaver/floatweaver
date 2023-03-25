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

namespace weaver
{
	template<>
	class VectorScalar<float, 4> : protected VectorScalarBase<float, 4, SimdExt::SSE>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(SSE)

	public:
		WEAVER_SIMD_BINARY_OP(operator+, _mm_add_ps, 1)
		WEAVER_SIMD_BINARY_OP(operator-, _mm_sub_ps, 1)
		WEAVER_SIMD_BINARY_OP(operator*, _mm_mul_ps, 1)
		WEAVER_SIMD_BINARY_OP(operator/, _mm_div_ps, 1)

	public:
		WEAVER_SIMD_ASSIGN_OP(operator+=, _mm_add_ps, 1)
		WEAVER_SIMD_ASSIGN_OP(operator-=, _mm_sub_ps, 1)
		WEAVER_SIMD_ASSIGN_OP(operator*=, _mm_mul_ps, 1)
		WEAVER_SIMD_ASSIGN_OP(operator/=, _mm_div_ps, 1)

	public:
		WEAVER_SIMD_COMP_OP(operator==, _mm_cmpeq_ps, 1)
		WEAVER_SIMD_COMP_OP(operator!=, _mm_cmpneq_ps, 1)
		WEAVER_SIMD_COMP_OP(operator<, _mm_cmplt_ps, 1)
		WEAVER_SIMD_COMP_OP(operator<=, _mm_cmple_ps, 1)
		WEAVER_SIMD_COMP_OP(operator>, _mm_cmpgt_ps, 1)
		WEAVER_SIMD_COMP_OP(operator>=, _mm_cmpge_ps, 1)

	public:
		WEAVER_SIMD_UNARY_OP(abs, abs_f32, 1)
		WEAVER_SIMD_UNARY_OP(sgn, sgn_f32, 1)

		WEAVER_SIMD_UNARY_OP(sin, _mm_sin_ps, 1)
		WEAVER_SIMD_UNARY_OP(cos, _mm_cos_ps, 1)
		WEAVER_SIMD_UNARY_OP(acos, _mm_acos_ps, 1)

		WEAVER_SIMD_UNARY_OP(exp, _mm_exp_ps, 1)
		WEAVER_SIMD_UNARY_OP(log, _mm_log_ps, 1)
		WEAVER_SIMD_UNARY_OP(sqrt, _mm_sqrt_ps, 1)
		WEAVER_SIMD_UNARY_OP(rcp, _mmWEAVER_rcp_ps, 1)
		WEAVER_SIMD_UNARY_OP(rsqrt, _mmWEAVER_rsqrt_ps, 1)

		WEAVER_SIMD_QUERY_OP(isinf, _mm_isinf_ps, 1)

	public:
		WEAVER_SIMD_BINARY_OP(min, _mm_min_ps, 1)
		WEAVER_SIMD_BINARY_OP(max, _mm_max_ps, 1)

		WEAVER_SIMD_BINARY_REDUCTION_OP(dot, _mmWEAVER_dp_ps, WEAVER_UNUSED, 1)

		WEAVER_SIMD_UNARY_REDUCTION_OP(min, _mmWEAVER_hmin_ps, WEAVER_UNUSED, 1)
		WEAVER_SIMD_UNARY_REDUCTION_OP(max, _mmWEAVER_hmax_ps, WEAVER_UNUSED, 1)
	};

	WEAVER_STRONG_INLINE VectorScalar<float, 4> select(const VectorScalar<bool, 4>& mask, const VectorScalar<float, 4>& a, const VectorScalar<float, 4>& b) noexcept
	{
		return VectorScalar<float, 4>(blend_f32(b.get(0), a.get(0), mask.get(0)));
	}

	//WEAVER_STRONG_INLINE std::ostream& operator<<(std::ostream& s, const VectorScalar<float, 4>& rhs)
	//{
	//	alignas(16) float vars[4];
	//	_mm_store_ps(vars + 0, rhs.get(0));
	//
	//	s << "'" << vars[0] << ", " << vars[1] << ", " << vars[2] << ", " << vars[3] << "'";
	//	return s;
	//}
}
