// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// C++ standard library
#include <utility>

// FloatWeaver
#include <weaver/bool16_ref.h>
#include <weaver/common.h>

namespace weaver {
	template<>
	class VectorScalar<float, 16> : protected VectorScalarBase<float, 16, SimdExt::None>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(None)
		explicit VectorScalar(const Scalar* scalars, size_t stride)
		{
			for (int i = 0; i < NrValues; i++)
				_data[i] = scalars[i * stride];
		}
		WEAVER_STRONG_INLINE Scalar& operator[](int idx) { return _data[idx]; }

	public:
		WEAVER_SIMD_BINARY_OP(operator+, details::add, 16)
		WEAVER_SIMD_BINARY_OP(operator-, details::sub, 16)
		WEAVER_SIMD_BINARY_OP(operator*, details::mul, 16)
		WEAVER_SIMD_BINARY_OP(operator/, details::div, 16)

	public:
		WEAVER_SIMD_ASSIGN_OP(operator+=, details::add, 16)
		WEAVER_SIMD_ASSIGN_OP(operator-=, details::sub, 16)
		WEAVER_SIMD_ASSIGN_OP(operator*=, details::mul, 16)
		WEAVER_SIMD_ASSIGN_OP(operator/=, details::div, 16)

	public:
		WEAVER_SIMD_COMP_OP(operator==, details::cmpeq, 16)
		WEAVER_SIMD_COMP_OP(operator!=, details::cmpne, 16)
		WEAVER_SIMD_COMP_OP(operator<, details::cmplt, 16)
		WEAVER_SIMD_COMP_OP(operator<=, details::cmple, 16)
		WEAVER_SIMD_COMP_OP(operator>, details::cmpgt, 16)
		WEAVER_SIMD_COMP_OP(operator>=, details::cmpge, 16)

	public:
		WEAVER_SIMD_UNARY_OP(abs, std::abs, 16)
		WEAVER_SIMD_UNARY_OP(sgn, details::sgn, 16)

		WEAVER_SIMD_UNARY_OP(sin, std::sin, 16)
		WEAVER_SIMD_UNARY_OP(cos, std::cos, 16)
		WEAVER_SIMD_UNARY_OP(acos, std::acos, 16)

		WEAVER_SIMD_UNARY_OP(exp, std::exp, 16)
		WEAVER_SIMD_UNARY_OP(log, std::log, 16)
		WEAVER_SIMD_UNARY_OP(sqrt, std::sqrt, 16)
		WEAVER_SIMD_UNARY_OP(rcp, details::rcp, 16)
		WEAVER_SIMD_UNARY_OP(rsqrt, details::rsqrt, 16)

		WEAVER_SIMD_QUERY_OP(isinf, std::isinf, 16)

	public:
		WEAVER_SIMD_BINARY_OP(min, std::min, 16)
		WEAVER_SIMD_BINARY_OP(max, std::max, 16)

		WEAVER_SIMD_BINARY_REDUCTION_OP(dot, details::mul, details::add, 16)

		WEAVER_SIMD_UNARY_REDUCTION_OP(min, details::nop, std::min, 16)
		WEAVER_SIMD_UNARY_REDUCTION_OP(max, details::nop, std::max, 16)
	};
}
