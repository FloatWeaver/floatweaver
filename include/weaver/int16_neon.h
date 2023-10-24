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
	class alignas(16) VectorScalar<int, 16> : protected VectorScalarBase<int, 16, SimdExt::NEON>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(NEON)

		WEAVER_SIMD_BINARY_OP(operator+, vaddq_s32, 4)
		WEAVER_SIMD_BINARY_OP(operator-, vsubq_s32, 4)
		WEAVER_SIMD_BINARY_OP(operator*, vmulq_s32, 4)

		WEAVER_SIMD_ASSIGN_OP(operator+=, vaddq_s32, 4)
		WEAVER_SIMD_ASSIGN_OP(operator-=, vsubq_s32, 4)
		WEAVER_SIMD_ASSIGN_OP(operator*=, vmulq_s32, 4)

		WEAVER_SIMD_COMP_OP(operator==, vceqq_s32, 4)
		//WEAVER_SIMD_COMP_OP(operator!=, vcneqq_s32, 4)
		WEAVER_SIMD_COMP_OP(operator<, vcltq_s32, 4)
		WEAVER_SIMD_COMP_OP(operator<=, vcleq_s32, 4)
		WEAVER_SIMD_COMP_OP(operator>, vcgtq_s32, 4)
		WEAVER_SIMD_COMP_OP(operator>=, vcgeq_s32, 4)

		WEAVER_SIMD_UNARY_OP(abs, vabsq_s32, 4)

		WEAVER_SIMD_BINARY_OP(operator&, vandq_s32, 4)
		WEAVER_SIMD_BINARY_OP(operator|, vorrq_s32, 4)

		WEAVER_SIMD_BINARY_OP(min, vminq_s32, 4)
		WEAVER_SIMD_BINARY_OP(max, vmaxq_s32, 4)
	};

	WEAVER_STRONG_INLINE VectorScalar<int, 16> select(const VectorScalar<bool, 16>& mask, const VectorScalar<int, 16>& a, const VectorScalar<int, 16>& b) noexcept
	{
		return VectorScalar<int, 16>(
			vbslq_s32(mask.get(0), a.get(0), b.get(0)),
			vbslq_s32(mask.get(1), a.get(1), b.get(1)),
			vbslq_s32(mask.get(2), a.get(2), b.get(2)),
			vbslq_s32(mask.get(3), a.get(3), b.get(3)));
	}
}
