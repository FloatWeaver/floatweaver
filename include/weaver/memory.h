// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// C++ standard library
#include <array>

// Eigen
#include <Eigen/Core>

// FloatWeaver
#include <weaver/vector_types.h>

#if defined WEAVER_VECTORIZE_SSE
#	include <weaver/memory_sse.h>
#endif

#if defined WEAVER_VECTORIZE_AVX
#	include <weaver/memory_avx.h>
#endif

#if defined WEAVER_VECTORIZE_NEON
#	include <weaver/memory_neon.h>
#endif

namespace weaver {

	template<typename T>
	WEAVER_STRONG_INLINE void load(T& value, const T* base) noexcept
	{
		value = base[0];
	}

	template<typename T>
	WEAVER_STRONG_INLINE void store(T* base, const T& value) noexcept
	{
		base[0] = value;
	}

	template<typename Scalar>
	Scalar gather(Scalar const* base, int vindex) noexcept
	{
		return *(base + vindex * 1);
	}

	template<typename Scalar, int Width, int Rows, int Cols>
	Eigen::Matrix<VectorScalar<Scalar, Width>, Rows, Cols> gather(
		const Eigen::Matrix<Scalar, Rows, Cols>* base,
		VectorScalar<int, Width>& vindex)
	{
		static_assert(Rows != Eigen::Dynamic && Cols != Eigen::Dynamic, "Only fixed size matrices are supported.");
		static_assert(sizeof(Eigen::Matrix<Scalar, Rows, Cols>) == Rows * Cols * sizeof(Scalar), "Size of matrix type does not contain any padding.");

		using wideint_t = VectorScalar<int, Width>;

		Eigen::Matrix<VectorScalar<Scalar, Width>, Rows, Cols> res;

		for (int c = 0; c < Cols; c++)
		{
			for (int r = 0; r < Rows; r++)
			{
				const wideint_t scale = wideint_t(Rows * Cols);
				const wideint_t offset = wideint_t(Rows * c + r);
				const wideint_t idx = scale * vindex + offset;
				res(r, c) = gather(base->data(), idx);
			}
		}

		return res;
	}

	template<typename Scalar>
	void scatter(Scalar value, Scalar* base, int vindex) noexcept
	{
		*(base + vindex * 1) = value;
	}

	template<typename Scalar, int Rows, int Cols>
	void scatter(Eigen::Matrix<Scalar, Rows, Cols> value, Eigen::Matrix<Scalar, Rows, Cols>* base, int vindex) noexcept
	{
		*(base + vindex * 1) = value;
	}

	template<typename Scalar, int Width>
	void scatter(const VectorScalar<Scalar, Width>& value, Scalar* base, const VectorScalar<int, Width>& vindex) noexcept
	{
		for (int i = 0; i < Width; i++)
		{
			*(base + vindex[i] * 1) = value[i];
		}
	}

	template<typename Scalar, int Width, int Rows, int Cols>
	void scatter(
		const Eigen::Matrix<VectorScalar<Scalar, Width>, Rows, Cols>& value,
		Eigen::Matrix<Scalar, Rows, Cols>* base,
		const VectorScalar<int, Width>& vindex)
	{
		static_assert(Rows != Eigen::Dynamic && Cols != Eigen::Dynamic, "Only fixed size matrices are supported.");

		using intN_t = VectorScalar<int, Width>;
		for (int c = 0; c < Cols; c++)
		{
			for (int r = 0; r < Rows; r++)
			{
				const intN_t scale = intN_t(Rows * Cols);
				const intN_t offset = intN_t(Rows * c + r);
				const intN_t idx = scale * vindex + offset;
				scatter(value(r, c), base->data(), idx);
			}
		}
	}

#if !defined(WEAVER_VECTORIZE_SSE) && !defined(WEAVER_VECTORIZE_AVX) && !defined(WEAVER_VECTORIZE_NEON)
	template<typename T, int Width>
	WEAVER_STRONG_INLINE void load(VectorScalar<T, Width>& value, const T* base)
	{
		value = VectorScalar<T, Width>(base, 1);
	}

	template<typename T, int Width>
	WEAVER_STRONG_INLINE void load(
		Eigen::Matrix<VectorScalar<T, Width>, 2, 1>& loaded,
		const Eigen::Matrix<T, 2, 1>* base)
	{
		loaded(0) = VectorScalar<T, Width>(base->data() + 0, 2);
		loaded(1) = VectorScalar<T, Width>(base->data() + 1, 2);
	}

	template<typename T, int Width>
	WEAVER_STRONG_INLINE void load(
		Eigen::Matrix<VectorScalar<T, Width>, 3, 1>& loaded,
		const Eigen::Matrix<T, 3, 1>* base)
	{
		loaded(0) = VectorScalar<T, Width>(base->data() + 0, 3);
		loaded(1) = VectorScalar<T, Width>(base->data() + 1, 3);
		loaded(2) = VectorScalar<T, Width>(base->data() + 2, 3);
	}

	template<typename T, int Width>
	WEAVER_STRONG_INLINE void load(
		Eigen::Matrix<VectorScalar<T, Width>, 4, 1>& loaded,
		const Eigen::Matrix<T, 4, 1>* base)
	{
		loaded(0) = VectorScalar<T, Width>(base->data() + 0, 4);
		loaded(1) = VectorScalar<T, Width>(base->data() + 1, 4);
		loaded(2) = VectorScalar<T, Width>(base->data() + 2, 4);
		loaded(3) = VectorScalar<T, Width>(base->data() + 3, 4);
	}

	template<typename T, int Width>
	VectorScalar<T, Width> gather(T const* base, VectorScalar<int, Width> vindex)
	{
		VectorScalar<T, Width> gathered;
		for (size_t i = 0; i < Width; i++)
			gathered[i] = *(base + vindex[i]);

		return gathered;
	}

	template<typename T, int Width>
	WEAVER_STRONG_INLINE std::array<VectorScalar<T, Width>, 2> interleave(const VectorScalar<T, Width>& a, const VectorScalar<T, Width>& b)
	{
		static_assert(Width % 2 == 0, "Interleaving requires even number of entries");

		VectorScalar<T, Width> low, high;
		for (size_t i = 0; i < Width / 2; i++)
		{
			low[2 * i + 0] = a[i];
			low[2 * i + 1] = b[i];
		}
		for (size_t i = 0; i < Width / 2; i++)
		{
			high[2 * i + 0] = a[Width / 2 + i];
			high[2 * i + 1] = b[Width / 2 + i];
		}

		return { low, high };
	}
#endif
}
