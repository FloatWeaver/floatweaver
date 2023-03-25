#pragma once

// FloatWeaver
#include <weaver/common.h>

namespace weaver
{
	template<typename Scalar, int Width>
	VectorScalar<Scalar, Width> select(
		const VectorScalar<bool, Width>& mask,
		const VectorScalar<Scalar, Width>& a,
		const VectorScalar<Scalar, Width>& b)
	{
		VectorScalar<Scalar, Width> res;
		for (int i = 0; i < Width; i++)
			res[i] = mask[i] ? a[i] : b[i];

		return res;
	}

	template<int Width>
	bool any(const VectorScalar<bool, Width>& x)
	{
		bool res = false;
		for (int i = 0; i < Width; i++)
			res = res || x[i];

		return res;
	}
	template<int Width>
	bool all(const VectorScalar<bool, Width>& x)
	{
		bool res = true;
		for (int i = 0; i < Width; i++)
			res = res && x[i];

		return res;
	}
	template<int Width>
	bool none(const VectorScalar<bool, Width>& x)
	{
		bool res = false;
		for (int i = 0; i < Width; i++)
			res = res || x[i];

		return !res;
	}

	//template<typename Scalar, int Width>
	//std::ostream& operator<<(std::ostream& s, const VectorScalar<Scalar, Width>& rhs)
	//{
	//	s << "'" << rhs[0];
	//	for (int i = 1; i < Width; i++)
	//		s << ", " << rhs[1];
	//	s << "'";
	//	return s;
	//}
}
