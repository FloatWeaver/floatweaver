// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// FloatWeaver
#include <weaver/vector_types.h>

namespace weaver
{
	template<typename Scalar>
	WEAVER_STRONG_INLINE void cswap(bool mask, Scalar& a, Scalar& b)
	{
		Scalar c = a;
		a = select(mask, b, a);
		b = select(mask, c, b);
	}
	template<typename Scalar>
	WEAVER_STRONG_INLINE void cnswap(bool mask, Scalar& a, Scalar& b)
	{
		Scalar c = -a;
		a = select(mask, b, a);
		b = select(mask, c, b);
	}

	template<typename Scalar, int Width>
	WEAVER_STRONG_INLINE void cswap(const VectorScalar<bool, Width>& mask, VectorScalar<Scalar, Width>& a, VectorScalar<Scalar, Width>& b)
	{
		VectorScalar<Scalar, Width> c = select(mask, b, a);
		VectorScalar<Scalar, Width> d = select(mask, a, b);
		a = c;
		b = d;
	}

	template<typename Scalar, int Width>
	WEAVER_STRONG_INLINE void cnswap(const VectorScalar<bool, Width>& mask, VectorScalar<Scalar, Width>& a, VectorScalar<Scalar, Width>& b)
	{
		VectorScalar<Scalar, Width> c = -a;
		a = select(mask, b, a);
		b = select(mask, c, b);
	}
}
