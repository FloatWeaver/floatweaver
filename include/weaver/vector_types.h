// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// C++ standard library
#include <utility>

// Eigen
#include <Eigen/Core>

// FloatWeaver
#include <weaver/config.h>

#if defined WEAVER_VECTORIZE_AVX512
#	include <weaver/bool4_sse.h>
#	include <weaver/bool8_avx.h>
#	include <weaver/bool16_avx512.h>
#	include <weaver/float4_sse.h>
#	include <weaver/float8_avx.h>
#	include <weaver/float16_avx512.h>
#	include <weaver/int4_sse.h>
#	include <weaver/int8_avx.h>
#	include <weaver/int16_avx512.h>
#elif defined WEAVER_VECTORIZE_AVX
#	include <weaver/bool4_sse.h>
#	include <weaver/bool8_avx.h>
#	include <weaver/bool16_avx.h>
#	include <weaver/float4_sse.h>
#	include <weaver/float8_avx.h>
#	include <weaver/float16_avx.h>
#	include <weaver/int4_sse.h>
#	include <weaver/int8_avx.h>
#	include <weaver/int16_avx.h>
#elif defined WEAVER_VECTORIZE_SSE
#	include <weaver/bool4_sse.h>
#	include <weaver/bool8_sse.h>
#	include <weaver/bool16_sse.h>
#	include <weaver/float4_sse.h>
#	include <weaver/float8_sse.h>
#	include <weaver/float16_sse.h>
#	include <weaver/int4_sse.h>
#	include <weaver/int8_sse.h>
#	include <weaver/int16_sse.h>
#elif defined WEAVER_VECTORIZE_NEON
#	include <weaver/bool4_neon.h>
#	include <weaver/bool8_neon.h>
#	include <weaver/bool16_neon.h>
#	include <weaver/float4_neon.h>
#	include <weaver/float8_neon.h>
#	include <weaver/float16_neon.h>
#	include <weaver/int4_neon.h>
#	include <weaver/int8_neon.h>
#	include <weaver/int16_neon.h>
#else
#	include <weaver/bool4_ref.h>
#	include <weaver/bool8_ref.h>
#	include <weaver/bool16_ref.h>
#	include <weaver/float4_ref.h>
#	include <weaver/float8_ref.h>
#	include <weaver/float16_ref.h>
#	include <weaver/int4_ref.h>
#	include <weaver/int8_ref.h>
#	include <weaver/int16_ref.h>
#	include <weaver/common_ref.h>
#endif

namespace weaver
{
	using float4 = VectorScalar<float, 4>;
	using float8 = VectorScalar<float, 8>;
	using float16 = VectorScalar<float, 16>;

	using int4 = VectorScalar<int, 4>;
	using int8 = VectorScalar<int, 8>;
	using int16 = VectorScalar<int, 16>;

	using bool4 = VectorScalar<bool, 4>;
	using bool8 = VectorScalar<bool, 8>;
	using bool16 = VectorScalar<bool, 16>;
}

namespace weaver
{
	//! Component-wise negation
	template<int Width>
	WEAVER_STRONG_INLINE VectorScalar<float, Width> operator-(const VectorScalar<float, Width>& a)
	{
		return a * VectorScalar<float, Width>(-1);
	}

	//! Component-wise negation
	template<int Width>
	WEAVER_STRONG_INLINE VectorScalar<int, Width> operator-(const VectorScalar<int, Width>& a)
	{
		return a * VectorScalar<int, Width>(-1);
	}

	template<typename Scalar, int Width>
	WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> operator+(Scalar a, const VectorScalar<Scalar, Width>& b)
	{
		return VectorScalar<Scalar, Width>(a) + b;
	}
	template<typename Scalar, int Width>
	WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> operator-(Scalar a, const VectorScalar<Scalar, Width>& b)
	{
		return VectorScalar<Scalar, Width>(a) - b;
	}
	template<typename Scalar, int Width>
	WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> operator*(Scalar a, const VectorScalar<Scalar, Width>& b)
	{
		return VectorScalar<Scalar, Width>(a) * b;
	}
	template<typename Scalar, int Width>
	WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> operator/(Scalar a, const VectorScalar<Scalar, Width>& b)
	{
		return VectorScalar<Scalar, Width>(a) / b;
	}

	// clang-format off
	template<int Width> WEAVER_STRONG_INLINE VectorScalar<bool, Width> isinf(const VectorScalar<float, Width>& x) noexcept { return x.isinf(); }

	template<typename Scalar, int Width> WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> abs(const VectorScalar<Scalar, Width>& x) noexcept { return x.abs(); }
	template<typename Scalar, int Width> WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> abs2(const VectorScalar<Scalar, Width>& x) noexcept { return x * x; }
	template<typename Scalar, int Width> WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> sqrt(const VectorScalar<Scalar, Width>& x) noexcept { return x.sqrt(); }
	template<typename Scalar, int Width> WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> exp(const VectorScalar<Scalar, Width>& x) noexcept { return x.exp(); }
	template<typename Scalar, int Width> WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> log(const VectorScalar<Scalar, Width>& x) noexcept { return x.log(); }
	template<typename Scalar, int Width> WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> rcp(const VectorScalar<Scalar, Width>& x) noexcept { return x.rcp(); }
	template<typename Scalar, int Width> WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> rsqrt(const VectorScalar<Scalar, Width>& x) noexcept { return x.rsqrt(); }

	template<typename Scalar, int Width> WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> sin(const VectorScalar<Scalar, Width>& x)  noexcept { return x.sin(); }
	template<typename Scalar, int Width> WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> cos(const VectorScalar<Scalar, Width>& x)  noexcept { return x.cos(); }
	template<typename Scalar, int Width> WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> acos(const VectorScalar<Scalar, Width>& x) noexcept { return x.acos(); }

	template<typename Scalar, int Width> WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> pow(const VectorScalar<Scalar, Width>& x, const VectorScalar<Scalar, Width>& y) noexcept { return exp(log(x) * y); }

	template<typename Scalar, int Width> WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> min(const VectorScalar<Scalar, Width>& x, const VectorScalar<Scalar, Width>& y) noexcept { return x.min(y); }
	template<typename Scalar, int Width> WEAVER_STRONG_INLINE VectorScalar<Scalar, Width> max(const VectorScalar<Scalar, Width>& x, const VectorScalar<Scalar, Width>& y) noexcept { return x.max(y); }

	template<typename Scalar, int Width> WEAVER_STRONG_INLINE Scalar min(const VectorScalar<Scalar, Width>& x) noexcept { return x.min(); }
	template<typename Scalar, int Width> WEAVER_STRONG_INLINE Scalar max(const VectorScalar<Scalar, Width>& x) noexcept { return x.max(); }
	// clang-format on

	template<int Width>
	WEAVER_STRONG_INLINE VectorScalar<float, Width> sgn(const VectorScalar<float, Width>& x)
	{
		return x.sgn();
	}
	template<int Width>
	WEAVER_STRONG_INLINE VectorScalar<int, Width> sgn(const VectorScalar<int, Width>& a)
	{
		using int_t = VectorScalar<int, Width>;
		return select(int_t(0) < a, int_t(1), int_t(0)) - select(a < int_t(0), int_t(1), int_t(0));
	}

	// Based on: http://realtimecollisiondetection.net/pubs/GDC08_Ericson_Physics_Tutorial_Numerical_Robustness.ppt
	WEAVER_STRONG_INLINE bool equal(float x, float y, float tol = 0)
	{
		return std::abs(x - y) <= tol * std::max(1.0f, std::max(std::abs(x), std::abs(y)));
	}

	template<int Width>
	WEAVER_STRONG_INLINE VectorScalar<bool, Width> equal(
		const VectorScalar<float, Width>& x,
		const VectorScalar<float, Width>& y,
		const VectorScalar<float, Width>& tol = 0)
	{
		return abs(x - y) <= tol * max(VectorScalar<float, Width>(1), max(abs(x), abs(y)));
	}

	template<typename Scalar, int Width, int Rows, int Cols>
	WEAVER_STRONG_INLINE Eigen::Matrix<VectorScalar<Scalar, Width>, Rows, Cols> select(
		const VectorScalar<bool, Width>& mask,
		const Eigen::Matrix<VectorScalar<Scalar, Width>, Rows, Cols>& a,
		const Eigen::Matrix<VectorScalar<Scalar, Width>, Rows, Cols>& b)
	{
		Eigen::Matrix<VectorScalar<Scalar, Width>, Rows, Cols> selected;
		for (int c = 0; c < Cols; c++)
		{
			for (int r = 0; r < Rows; r++)
			{
				selected(r, c) = select(mask, a(r, c), b(r, c));
			}
		}

		return selected;
	}

	template<typename Scalar, int Width, size_t N>
	WEAVER_STRONG_INLINE std::array<VectorScalar<Scalar, Width>, N> select(
		const VectorScalar<bool, Width>& mask,
		const std::array<VectorScalar<Scalar, Width>, N>& a,
		const std::array<VectorScalar<Scalar, Width>, N>& b)
	{
		std::array<VectorScalar<Scalar, Width>, N> selected;

		for (size_t i = 0; i < N; i++)
			selected[i] = select(mask, a[i], b[i]);

		return selected;
	}
}
