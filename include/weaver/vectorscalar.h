
namespace Vcl {
	template<typename T>
	struct VectorTypes
	{};

	template<>
	struct VectorTypes<float>
	{
		using float_t = float;
		using int_t = int;
		using bool_t = bool;
	};

	template<>
	struct VectorTypes<VectorScalar<float, 4>>
	{
		using float_t = VectorScalar<float, 4>;
		using int_t = VectorScalar<int, 4>;
		using bool_t = VectorScalar<bool, 4>;
	};

	template<>
	struct VectorTypes<VectorScalar<float, 8>>
	{
		using float_t = VectorScalar<float, 8>;
		using int_t = VectorScalar<int, 8>;
		using bool_t = VectorScalar<bool, 8>;
	};

	template<>
	struct VectorTypes<VectorScalar<float, 16>>
	{
		using float_t = VectorScalar<float, 16>;
		using int_t = VectorScalar<int, 16>;
		using bool_t = VectorScalar<bool, 16>;
	};

	template<typename T>
	struct NumericTrait
	{
		using base_t = T;
		using wide_t = T;
	};

	template<>
	struct NumericTrait<VectorScalar<float, 4>>
	{
		using base_t = float;
#if defined VCL_VECTORIZE_SSE
		using wide_t = __m128;
#elif defined VCL_VECTORIZE_NEON
		using wide_t = float32x4_t;
#endif
	};
	template<>
	struct NumericTrait<VectorScalar<float, 8>>
	{
		using base_t = float;
#if defined VCL_VECTORIZE_AVX
		using wide_t = __m256;
#elif defined VCL_VECTORIZE_SSE
		using wide_t = __m128;
#elif defined VCL_VECTORIZE_NEON
		using wide_t = float32x4_t;
#endif
	};
	template<>
	struct NumericTrait<VectorScalar<float, 16>>
	{
		using base_t = float;
#if defined VCL_VECTORIZE_AVX
		using wide_t = __m256;
#elif defined VCL_VECTORIZE_SSE
		using wide_t = __m128;
#elif defined VCL_VECTORIZE_NEON
		using wide_t = float32x4_t;
#endif
	};

	template<typename Scalar, int Width, int Rows, int Cols>
	VCL_STRONG_INLINE Eigen::Matrix<VectorScalar<Scalar, Width>, Rows, Cols> select(
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
	VCL_STRONG_INLINE std::array<VectorScalar<Scalar, Width>, N> select(
		const VectorScalar<bool, Width>& mask,
		const std::array<VectorScalar<Scalar, Width>, N>& a,
		const std::array<VectorScalar<Scalar, Width>, N>& b)
	{
		std::array<VectorScalar<Scalar, Width>, N> selected;

		for (size_t i = 0; i < N; i++)
			selected[i] = select(mask, a[i], b[i]);

		return selected;
	}

	template<typename Scalar, int Width>
	VCL_STRONG_INLINE VectorScalar<bool, Width> equal(
		const VectorScalar<Scalar, Width>& x,
		const VectorScalar<Scalar, Width>& y,
		const VectorScalar<Scalar, Width>& tol = 0)
	{
		return abs(x - y) <= tol * max(VectorScalar<Scalar, Width>(1), max(abs(x), abs(y)));
	}
}
