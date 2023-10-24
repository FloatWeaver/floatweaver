// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// FloatWeaver
#include <weaver/bool16_neon.h>
#include <weaver/common.h>

namespace weaver {
	template<>
	class alignas(16) VectorScalar<float, 16> : protected VectorScalarBase<float, 16, SimdExt::NEON>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(NEON)

		WEAVER_SIMD_BINARY_OP(operator+, vaddq_f32, 4)
		WEAVER_SIMD_BINARY_OP(operator-, vsubq_f32, 4)
		WEAVER_SIMD_BINARY_OP(operator*, vmulq_f32, 4)
		WEAVER_SIMD_BINARY_OP(operator/, vdivq_f32, 4)

		WEAVER_SIMD_ASSIGN_OP(operator+=, vaddq_f32, 4)
		WEAVER_SIMD_ASSIGN_OP(operator-=, vsubq_f32, 4)
		WEAVER_SIMD_ASSIGN_OP(operator*=, vmulq_f32, 4)
		WEAVER_SIMD_ASSIGN_OP(operator/=, vdivq_f32, 4)

		WEAVER_SIMD_COMP_OP(operator==, vceqq_f32, 4)
		WEAVER_SIMD_COMP_OP(operator!=, vcneqq_f32, 4)
		WEAVER_SIMD_COMP_OP(operator<, vcltq_f32, 4)
		WEAVER_SIMD_COMP_OP(operator<=, vcleq_f32, 4)
		WEAVER_SIMD_COMP_OP(operator>, vcgtq_f32, 4)
		WEAVER_SIMD_COMP_OP(operator>=, vcgeq_f32, 4)

		WEAVER_SIMD_UNARY_OP(abs, vabsq_f32, 4)
		WEAVER_SIMD_UNARY_OP(sin, vsinq_f32, 4)
		WEAVER_SIMD_UNARY_OP(cos, vcosq_f32, 4)
		WEAVER_SIMD_UNARY_OP(exp, vexpq_f32, 4)
		WEAVER_SIMD_UNARY_OP(log, vlogq_f32, 4)
		WEAVER_SIMD_UNARY_OP(sgn, vsgnq_f32, 4)
		WEAVER_SIMD_UNARY_OP(sqrt, vsqrtq_f32, 4)
		WEAVER_SIMD_UNARY_OP(rcp, vrcpq_f32, 4)
		WEAVER_SIMD_UNARY_OP(rsqrt, vrsqrtq_f32, 4)

		WEAVER_SIMD_UNARY_OP(acos, vacosq_f32, 4)

		WEAVER_SIMD_QUERY_OP(isinf, visinfq_f32, 4)

		WEAVER_SIMD_BINARY_OP(min, vminq_f32, 4)
		WEAVER_SIMD_BINARY_OP(max, vmaxq_f32, 4)

		WEAVER_SIMD_BINARY_REDUCTION_OP(dot, vdotq_f32, details::add, 4)

		WEAVER_SIMD_UNARY_REDUCTION_OP(min, weaver_vpminq_f32, details::min, 4)
		WEAVER_SIMD_UNARY_REDUCTION_OP(max, weaver_vpmaxq_f32, details::max, 4)
	};

	WEAVER_STRONG_INLINE VectorScalar<float, 16> select(const VectorScalar<bool, 16>& mask, const VectorScalar<float, 16>& a, const VectorScalar<float, 16>& b) noexcept
	{
		// (((b ^ a) & mask)^b)
		return VectorScalar<float, 16>(
			vbslq_f32(mask.get(0), a.get(0), b.get(0)),
			vbslq_f32(mask.get(1), a.get(1), b.get(1)),
			vbslq_f32(mask.get(2), a.get(2), b.get(2)),
			vbslq_f32(mask.get(3), a.get(3), b.get(3)));
	}
}
