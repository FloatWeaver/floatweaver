// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// C++ standard library
#include <utility>

// FloatWeaver
#include <weaver/bool8_ref.h>
#include <weaver/common.h>

namespace weaver {
	template<>
	class VectorScalar<int, 8> : protected VectorScalarBase<int, 8, SimdExt::None>
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
		WEAVER_SIMD_BINARY_OP(operator+, details::add, 8)
		WEAVER_SIMD_BINARY_OP(operator-, details::sub, 8)
		WEAVER_SIMD_BINARY_OP(operator*, details::mul, 8)

	public:
		WEAVER_SIMD_ASSIGN_OP(operator+=, details::add, 8)
		WEAVER_SIMD_ASSIGN_OP(operator-=, details::sub, 8)
		WEAVER_SIMD_ASSIGN_OP(operator*=, details::mul, 8)

	public:
		WEAVER_SIMD_COMP_OP(operator==, details::cmpeq, 8)
		WEAVER_SIMD_COMP_OP(operator!=, details::cmpne, 8)
		WEAVER_SIMD_COMP_OP(operator<, details::cmplt, 8)
		WEAVER_SIMD_COMP_OP(operator<=, details::cmple, 8)
		WEAVER_SIMD_COMP_OP(operator>, details::cmpgt, 8)
		WEAVER_SIMD_COMP_OP(operator>=, details::cmpge, 8)

	public:
		WEAVER_SIMD_UNARY_OP(abs, std::abs, 8)

	public:
		WEAVER_SIMD_BINARY_OP(operator&, details::conj, 8)
		WEAVER_SIMD_BINARY_OP(operator|, details::disj, 8)

		WEAVER_SIMD_BINARY_OP(min, std::min, 8)
		WEAVER_SIMD_BINARY_OP(max, std::max, 8)
	};
}
