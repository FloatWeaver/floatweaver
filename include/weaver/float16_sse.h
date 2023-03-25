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
	class VectorScalar<float, 16> : protected VectorScalarBase<float, 16, SimdExt::SSE>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(SSE)

	public:
		WEAVER_SIMD_BINARY_OP(operator+, _mm_add_ps, 4)
		WEAVER_SIMD_BINARY_OP(operator-, _mm_sub_ps, 4)
		WEAVER_SIMD_BINARY_OP(operator*, _mm_mul_ps, 4)
		WEAVER_SIMD_BINARY_OP(operator/, _mm_div_ps, 4)

	public:
		WEAVER_SIMD_ASSIGN_OP(operator+=, _mm_add_ps, 4)
		WEAVER_SIMD_ASSIGN_OP(operator-=, _mm_sub_ps, 4)
		WEAVER_SIMD_ASSIGN_OP(operator*=, _mm_mul_ps, 4)
		WEAVER_SIMD_ASSIGN_OP(operator/=, _mm_div_ps, 4)

	public:
		WEAVER_SIMD_COMP_OP(operator==, _mm_cmpeq_ps, 4)
		WEAVER_SIMD_COMP_OP(operator!=, _mm_cmpneq_ps, 4)
		WEAVER_SIMD_COMP_OP(operator<, _mm_cmplt_ps, 4)
		WEAVER_SIMD_COMP_OP(operator<=, _mm_cmple_ps, 4)
		WEAVER_SIMD_COMP_OP(operator>, _mm_cmpgt_ps, 4)
		WEAVER_SIMD_COMP_OP(operator>=, _mm_cmpge_ps, 4)

	public:
		WEAVER_SIMD_UNARY_OP(abs, abs_f32, 4)
		WEAVER_SIMD_UNARY_OP(sgn, sgn_f32, 4)

		WEAVER_SIMD_UNARY_OP(sin, _mm_sin_ps, 4)
		WEAVER_SIMD_UNARY_OP(cos, _mm_cos_ps, 4)
		WEAVER_SIMD_UNARY_OP(acos, _mm_acos_ps, 4)

		WEAVER_SIMD_UNARY_OP(exp, _mm_exp_ps, 4)
		WEAVER_SIMD_UNARY_OP(log, _mm_log_ps, 4)
		WEAVER_SIMD_UNARY_OP(sqrt, _mm_sqrt_ps, 4)
		WEAVER_SIMD_UNARY_OP(rcp, _mmWEAVER_rcp_ps, 4)
		WEAVER_SIMD_UNARY_OP(rsqrt, _mmWEAVER_rsqrt_ps, 4)

		WEAVER_SIMD_QUERY_OP(isinf, _mm_isinf_ps, 4)

	public:
		WEAVER_SIMD_BINARY_OP(min, _mm_min_ps, 4)
		WEAVER_SIMD_BINARY_OP(max, _mm_max_ps, 4)

		WEAVER_SIMD_BINARY_REDUCTION_OP(dot, _mmWEAVER_dp_ps, details::add, 4)

		WEAVER_SIMD_UNARY_REDUCTION_OP(min, _mmWEAVER_hmin_ps, details::min, 4)
		WEAVER_SIMD_UNARY_REDUCTION_OP(max, _mmWEAVER_hmax_ps, details::max, 4)
	};

	WEAVER_STRONG_INLINE VectorScalar<float, 16> select(const VectorScalar<bool, 16>& mask, const VectorScalar<float, 16>& a, const VectorScalar<float, 16>& b) noexcept
	{
		return VectorScalar<float, 16>(
			blend_f32(b.get(0), a.get(0), mask.get(0)),
			blend_f32(b.get(1), a.get(1), mask.get(1)),
			blend_f32(b.get(2), a.get(2), mask.get(2)),
			blend_f32(b.get(3), a.get(3), mask.get(3)));
	}

	//WEAVER_STRONG_INLINE std::ostream& operator<<(std::ostream& s, const VectorScalar<float, 16>& rhs)
	//{
	//	alignas(16) float vars[16];
	//	_mm_store_ps(vars + 0, rhs.get(0));
	//	_mm_store_ps(vars + 4, rhs.get(1));
	//	_mm_store_ps(vars + 8, rhs.get(2));
	//	_mm_store_ps(vars + 12, rhs.get(3));
	//
	//	s << "'" << vars[0] << ", " << vars[1] << ", " << vars[2] << ", " << vars[3]
	//	  << vars[4] << ", " << vars[5] << ", " << vars[6] << ", " << vars[7]
	//	  << vars[8] << ", " << vars[9] << ", " << vars[10] << ", " << vars[11]
	//	  << vars[12] << ", " << vars[13] << ", " << vars[14] << ", " << vars[15] << "'";
	//
	//	return s;
	//}
}
