// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// Eigen
#include <Eigen/Core>

// FloatWeaver
#include <weaver/vector_types.h>

namespace Eigen {
	template<>
	struct NumTraits<weaver::float4> : GenericNumTraits<weaver::float4>
	{
		enum
		{
			IsInteger = std::numeric_limits<float>::is_integer,
			IsSigned = std::numeric_limits<float>::is_signed,
			IsComplex = 0,
			RequireInitialization = internal::is_arithmetic<float>::value ? 0 : 1,
			ReadCost = 1,
			AddCost = 1,
			MulCost = 1
		};

		EIGEN_STRONG_INLINE static weaver::float4 epsilon() noexcept { return std::numeric_limits<float>::epsilon(); }
		EIGEN_STRONG_INLINE static int digits10() { return GenericNumTraits<float>::digits10(); }
		EIGEN_STRONG_INLINE static weaver::float4 dummy_precision() noexcept { return 1e-5f; }
		EIGEN_STRONG_INLINE static weaver::float4 highest() noexcept { return std::numeric_limits<float>::max(); }
		EIGEN_STRONG_INLINE static weaver::float4 lowest() noexcept { return std::numeric_limits<float>::lowest(); }
		EIGEN_STRONG_INLINE static weaver::float4 infinity() noexcept { return std::numeric_limits<float>::infinity(); }
		EIGEN_STRONG_INLINE static weaver::float4 quiet_NaN() noexcept { return std::numeric_limits<float>::quiet_NaN(); }
	};
	template<>
	struct NumTraits<weaver::float8> : GenericNumTraits<weaver::float8>
	{
		enum
		{
			IsInteger = std::numeric_limits<float>::is_integer,
			IsSigned = std::numeric_limits<float>::is_signed,
			IsComplex = 0,
			RequireInitialization = internal::is_arithmetic<float>::value ? 0 : 1,
			ReadCost = 1,
			AddCost = 1,
			MulCost = 1
		};

		EIGEN_STRONG_INLINE static weaver::float8 epsilon() noexcept { return std::numeric_limits<float>::epsilon(); }
		EIGEN_STRONG_INLINE static int digits10() { return GenericNumTraits<float>::digits10(); }
		EIGEN_STRONG_INLINE static weaver::float8 dummy_precision() noexcept { return 1e-5f; }
		EIGEN_STRONG_INLINE static weaver::float8 highest() noexcept { return std::numeric_limits<float>::max(); }
		EIGEN_STRONG_INLINE static weaver::float8 lowest() noexcept { return std::numeric_limits<float>::lowest(); }
		EIGEN_STRONG_INLINE static weaver::float8 infinity() noexcept { return std::numeric_limits<float>::infinity(); }
		EIGEN_STRONG_INLINE static weaver::float8 quiet_NaN() noexcept { return std::numeric_limits<float>::quiet_NaN(); }
	};
	template<>
	struct NumTraits<weaver::float16> : GenericNumTraits<weaver::float16>
	{
		enum
		{
			IsInteger = std::numeric_limits<float>::is_integer,
			IsSigned = std::numeric_limits<float>::is_signed,
			IsComplex = 0,
			RequireInitialization = internal::is_arithmetic<float>::value ? 0 : 1,
			ReadCost = 1,
			AddCost = 1,
			MulCost = 1
		};

		EIGEN_STRONG_INLINE static weaver::float16 epsilon() noexcept { return std::numeric_limits<float>::epsilon(); }
		EIGEN_STRONG_INLINE static int digits10() { return GenericNumTraits<float>::digits10(); }
		EIGEN_STRONG_INLINE static weaver::float16 dummy_precision() noexcept { return 1e-5f; }
		EIGEN_STRONG_INLINE static weaver::float16 highest() noexcept { return std::numeric_limits<float>::max(); }
		EIGEN_STRONG_INLINE static weaver::float16 lowest() noexcept { return std::numeric_limits<float>::lowest(); }
		EIGEN_STRONG_INLINE static weaver::float16 infinity() noexcept { return std::numeric_limits<float>::infinity(); }
		EIGEN_STRONG_INLINE static weaver::float16 quiet_NaN() noexcept { return std::numeric_limits<float>::quiet_NaN(); }
	};
}

// clang-format off
namespace Eigen {
	namespace numext {
		template<typename Scalar, int Width> EIGEN_STRONG_INLINE const weaver::VectorScalar<Scalar, Width>& conj(const weaver::VectorScalar<Scalar, Width>& x) { return x; }
		template<typename Scalar, int Width> EIGEN_STRONG_INLINE const weaver::VectorScalar<Scalar, Width>& real(const weaver::VectorScalar<Scalar, Width>& x) { return x; }
		template<typename Scalar, int Width> EIGEN_STRONG_INLINE weaver::VectorScalar<Scalar, Width> imag(const weaver::VectorScalar<Scalar, Width>&) { return weaver::VectorScalar<Scalar, Width>(0); }
		template<typename Scalar, int Width> EIGEN_STRONG_INLINE weaver::VectorScalar<Scalar, Width> isinf(const weaver::VectorScalar<Scalar, Width>& x) { return x.isinf(); }
		template<typename Scalar, int Width> EIGEN_STRONG_INLINE weaver::VectorScalar<Scalar, Width> abs2(const weaver::VectorScalar<Scalar, Width>& x) { return x * x; }
		template<typename Scalar, int Width> EIGEN_STRONG_INLINE weaver::VectorScalar<Scalar, Width> sqrt(const weaver::VectorScalar<Scalar, Width>& x) { return x.sqrt(); }
		template<typename Scalar, int Width> EIGEN_STRONG_INLINE weaver::VectorScalar<Scalar, Width> exp(const weaver::VectorScalar<Scalar, Width>& x) { return x.exp(); }
		template<typename Scalar, int Width> EIGEN_STRONG_INLINE weaver::VectorScalar<Scalar, Width> log(const weaver::VectorScalar<Scalar, Width>& x) { return x.log(); }
		template<typename Scalar, int Width> EIGEN_STRONG_INLINE weaver::VectorScalar<Scalar, Width> sin(const weaver::VectorScalar<Scalar, Width>& x) { return x.sin(); }
		template<typename Scalar, int Width> EIGEN_STRONG_INLINE weaver::VectorScalar<Scalar, Width> cos(const weaver::VectorScalar<Scalar, Width>& x) { return x.cos(); }
		template<typename Scalar, int Width> EIGEN_STRONG_INLINE weaver::VectorScalar<Scalar, Width> pow(const weaver::VectorScalar<Scalar, Width>& x, const weaver::VectorScalar<Scalar, Width>& y) { return x.pow(y); }
	}
}
// clang-format on
