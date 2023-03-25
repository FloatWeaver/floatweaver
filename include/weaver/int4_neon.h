// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// FloatWeaver
#include <weaver/bool4_neon.h>
#include <weaver/common.h>

namespace weaver {
	template<>
	class alignas(16) VectorScalar<int, 4> : protected VectorScalarBase<int, 4, SimdExt::NEON>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(NEON)

	public:
		WEAVER_SIMD_BINARY_OP(operator+, vaddq_s32, 1)
		WEAVER_SIMD_BINARY_OP(operator-, vsubq_s32, 1)
		WEAVER_SIMD_BINARY_OP(operator*, vmulq_s32, 1)

	public:
		WEAVER_SIMD_ASSIGN_OP(operator+=, vaddq_s32, 1)
		WEAVER_SIMD_ASSIGN_OP(operator-=, vsubq_s32, 1)
		WEAVER_SIMD_ASSIGN_OP(operator*=, vmulq_s32, 1)

	public:
		WEAVER_SIMD_COMP_OP(operator==, vceqq_s32, 1)
		//WEAVER_SIMD_COMP_OP(operator!=, vcneqq_s32, 1)
		WEAVER_SIMD_COMP_OP(operator<, vcltq_s32, 1)
		WEAVER_SIMD_COMP_OP(operator<=, vcleq_s32, 1)
		WEAVER_SIMD_COMP_OP(operator>, vcgtq_s32, 1)
		WEAVER_SIMD_COMP_OP(operator>=, vcgeq_s32, 1)

	public:
		WEAVER_SIMD_UNARY_OP(abs, vabsq_s32, 1)

	public:
		WEAVER_SIMD_BINARY_OP(operator&, vandq_s32, 1)
		WEAVER_SIMD_BINARY_OP(operator|, vorrq_s32, 1)

		WEAVER_SIMD_BINARY_OP(min, vminq_s32, 1)
		WEAVER_SIMD_BINARY_OP(max, vmaxq_s32, 1)
	};

	WEAVER_STRONG_INLINE VectorScalar<int, 4> select(const VectorScalar<bool, 4>& mask, const VectorScalar<int, 4>& a, const VectorScalar<int, 4>& b) noexcept
	{
		// (((b ^ a) & mask)^b)
		return VectorScalar<int, 4>(vbslq_s32(mask.get(0), a.get(0), b.get(0)));
	}

	//WEAVER_STRONG_INLINE std::ostream& operator<<(std::ostream& s, const VectorScalar<int, 4>& rhs)
	//{
	//	alignas(8) int vars[4];
	//	vst1q_s32(vars + 0, rhs.get(0));
	//
	//	s << "'" << vars[0] << "," << vars[1] << "," << vars[2] << "," << vars[3] << "'";
	//
	//	return s;
	//}
}
