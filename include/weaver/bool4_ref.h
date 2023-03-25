// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// FloatWeaver
#include <weaver/common.h>

namespace weaver {
#ifdef WEAVER_COMPILER_MSVC
#	pragma warning(disable : 4324)
#endif
	template<>
	class alignas(16) VectorScalar<bool, 4> : protected VectorScalarBase<bool, 4, SimdExt::None>
	{
	public:
		WEAVER_SIMD_VECTORSCALAR_SETUP(None)
		WEAVER_STRONG_INLINE Scalar& operator[](int idx) noexcept { return _data[idx]; }

	public:
		WEAVER_SIMD_BINARY_OP(operator&&, details::conj, 4)
		WEAVER_SIMD_BINARY_OP(operator||, details::disj, 4)

		WEAVER_SIMD_ASSIGN_OP(operator&=, details::conj, 4)
		WEAVER_SIMD_ASSIGN_OP(operator|=, details::disj, 4)
	};
#ifdef WEAVER_COMPILER_MSVC
#	pragma warning(default : 4324)
#endif
}
