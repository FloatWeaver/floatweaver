// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

// C++ standard library
#include <cassert>
#include <cmath>
#include <type_traits>

// FloatWeaver
#include <weaver/config.h>

#include <weaver/intrinsics_sse.h>
#include <weaver/intrinsics_avx.h>
#include <weaver/intrinsics_avx512.h>
#include <weaver/intrinsics_neon.h>


// Concatenates two strings, even when the strings are macros themselves
#define WEAVER_PP_JOIN(x, y) WEAVER_PP_JOIN_HELPER(x, y)
#define WEAVER_PP_JOIN_HELPER(x, y) WEAVER_PP_JOIN_HELPER_HELPER(x, y)
#define WEAVER_PP_JOIN_HELPER_HELPER(x, y) x##y

/// Turns any legal C++ expression into nothing
#define WEAVER_UNUSED_IMPL(symExpr, n) , (void)sizeof(symExpr)
#define WEAVER_UNUSED(...) (void)sizeof(true) WEAVER_PP_EXPAND_ARGS WEAVER_PP_PASS_VA(WEAVER_UNUSED_IMPL, __VA_ARGS__)


namespace weaver {
	//! Type of vectorization extension
	enum class SimdExt
	{
		None,   //!< No vectorization
		SSE,    //!< Intel SSE (128 bits)
		AVX,    //!< Intel AVX (256 bits)
		AVX512, //!< Intel AVX512 (512 bits)
		NEON,   //!< ARM NEON (128 bits)
	};

	template<typename, SimdExt>
	struct SimdRegister
	{
		//! Scalar value type stored in registers
		using Scalar = void;

		//! Type of values in registers
		using Type = void;

		//! Number of values in a register
		static const int Width = 0;
	};

	template<>
	struct SimdRegister<float, SimdExt::None>
	{
		using Scalar = float;
		using Type = float;
		static const int Width = 1;

		WEAVER_STRONG_INLINE static Type set(Scalar s0) noexcept
		{
			return s0;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int) noexcept
		{
			return vec;
		}
	};
	template<>
	struct SimdRegister<int, SimdExt::None>
	{
		using Scalar = int;
		using Type = int;
		static const int Width = 1;

		WEAVER_STRONG_INLINE static Type set(Scalar s0) noexcept
		{
			return s0;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int) noexcept
		{
			return vec;
		}
	};
	template<>
	struct SimdRegister<bool, SimdExt::None>
	{
		using Scalar = bool;
		using Type = bool;
		static const int Width = 1;

		WEAVER_STRONG_INLINE static Type set(Scalar s0) noexcept
		{
			return s0;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int) noexcept
		{
			return vec;
		}
	};

#ifdef WEAVER_VECTORIZE_SSE
	template<>
	struct SimdRegister<float, SimdExt::SSE>
	{
		using Scalar = float;
		using Type = __m128;
		static const int Width = 4;

		WEAVER_STRONG_INLINE static Type set(Scalar s0) noexcept
		{
			return _mm_set1_ps(s0);
		}
		WEAVER_STRONG_INLINE static Type set(Scalar s0, Scalar s1, Scalar s2, Scalar s3) noexcept
		{
			return _mm_set_ps(s3, s2, s1, s0);
		}
		WEAVER_STRONG_INLINE static Type set(Type vec) noexcept
		{
			return vec;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int i) noexcept
		{
			return extract_f32(vec, i);
		}
	};
	template<>
	struct SimdRegister<int, SimdExt::SSE>
	{
		using Scalar = int;
		using Type = __m128i;
		static const int Width = 4;

		WEAVER_STRONG_INLINE static Type set(Scalar s0) noexcept
		{
			return _mm_set1_epi32(s0);
		}
		WEAVER_STRONG_INLINE static Type set(Scalar s0, Scalar s1, Scalar s2, Scalar s3) noexcept
		{
			return _mm_set_epi32(s3, s2, s1, s0);
		}
		WEAVER_STRONG_INLINE static Type set(Type vec) noexcept
		{
			return vec;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int i) noexcept
		{
			return extract_s32(vec, i);
		}
	};
	template<>
	struct SimdRegister<bool, SimdExt::SSE>
	{
		using Scalar = bool;
		using Type = __m128;
		static const int Width = 4;

		WEAVER_STRONG_INLINE static Type set(Scalar s0) noexcept
		{
			const int m0 = s0 ? -1 : 0;
			return _mm_castsi128_ps(_mm_set1_epi32(m0));
		}
		WEAVER_STRONG_INLINE static Type set(Scalar s0, Scalar s1, Scalar s2, Scalar s3) noexcept
		{
			const int m0 = s0 ? -1 : 0;
			const int m1 = s1 ? -1 : 0;
			const int m2 = s2 ? -1 : 0;
			const int m3 = s3 ? -1 : 0;
			return _mm_castsi128_ps(_mm_set_epi32(m3, m2, m1, m0));
		}
		WEAVER_STRONG_INLINE static Type set(Type vec) noexcept
		{
			return vec;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int i) noexcept
		{
			return extract_s32(_mm_castps_si128(vec), i) != 0;
		}
	};
#endif

#ifdef WEAVER_VECTORIZE_AVX
	template<>
	struct SimdRegister<float, SimdExt::AVX>
	{
		using Scalar = float;
		using Type = __m256;
		static const int Width = 8;

		WEAVER_STRONG_INLINE static Type set(Scalar s0) noexcept
		{
			return _mm256_set1_ps(s0);
		}
		WEAVER_STRONG_INLINE static Type set(
			Scalar s0,
			Scalar s1,
			Scalar s2,
			Scalar s3,
			Scalar s4,
			Scalar s5,
			Scalar s6,
			Scalar s7) noexcept
		{
			return _mm256_set_ps(s7, s6, s5, s4, s3, s2, s1, s0);
		}
		WEAVER_STRONG_INLINE static Type set(Type vec) noexcept
		{
			return vec;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int i) noexcept
		{
			return extract_f32(vec, i);
		}
	};
	template<>
	struct SimdRegister<int, SimdExt::AVX>
	{
		using Scalar = int;
		using Type = __m256i;
		static const int Width = 8;

		WEAVER_STRONG_INLINE static Type set(Scalar s0) noexcept
		{
			return _mm256_set1_epi32(s0);
		}
		WEAVER_STRONG_INLINE static Type set(
			Scalar s0,
			Scalar s1,
			Scalar s2,
			Scalar s3,
			Scalar s4,
			Scalar s5,
			Scalar s6,
			Scalar s7) noexcept
		{
			return _mm256_set_epi32(s7, s6, s5, s4, s3, s2, s1, s0);
		}
		WEAVER_STRONG_INLINE static Type set(Type vec) noexcept
		{
			return vec;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int i) noexcept
		{
			return extract_s32(vec, i);
		}
	};
	template<>
	struct SimdRegister<bool, SimdExt::AVX>
	{
		using Scalar = bool;
		using Type = __m256;
		static const int Width = 8;

		WEAVER_STRONG_INLINE static Type set(Scalar s0) noexcept
		{
			const int m0 = s0 ? -1 : 0;
			return _mm256_castsi256_ps(_mm256_set1_epi32(m0));
		}
		WEAVER_STRONG_INLINE static Type set(
			Scalar s0,
			Scalar s1,
			Scalar s2,
			Scalar s3,
			Scalar s4,
			Scalar s5,
			Scalar s6,
			Scalar s7) noexcept
		{
			const int m0 = s0 ? -1 : 0;
			const int m1 = s1 ? -1 : 0;
			const int m2 = s2 ? -1 : 0;
			const int m3 = s3 ? -1 : 0;
			const int m4 = s4 ? -1 : 0;
			const int m5 = s5 ? -1 : 0;
			const int m6 = s6 ? -1 : 0;
			const int m7 = s7 ? -1 : 0;
			return _mm256_castsi256_ps(_mm256_set_epi32(m7, m6, m5, m4, m3, m2, m1, m0));
		}
		WEAVER_STRONG_INLINE static Type set(Type vec) noexcept
		{
			return vec;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int i) noexcept
		{
			return extract_s32(_mm256_castps_si256(vec), i) != 0;
		}
	};
#endif

#ifdef WEAVER_VECTORIZE_AVX512
	template<>
	struct SimdRegister<float, SimdExt::AVX512>
	{
		using Scalar = float;
		using Type = __m512;
		static const int Width = 16;

		WEAVER_STRONG_INLINE static Type set(Scalar s0)
		{
			return _mm512_set1_ps(s0);
		}
		WEAVER_STRONG_INLINE static Type set(
			Scalar s0,
			Scalar s1,
			Scalar s2,
			Scalar s3,
			Scalar s4,
			Scalar s5,
			Scalar s6,
			Scalar s7,
			Scalar s8,
			Scalar s9,
			Scalar s10,
			Scalar s11,
			Scalar s12,
			Scalar s13,
			Scalar s14,
			Scalar s15)
		{
			return _mm512_set_ps(s15, s14, s13, s12, s11, s10, s9, s8, s7, s6, s5, s4, s3, s2, s1, s0);
		}
		WEAVER_STRONG_INLINE static Type set(Type vec)
		{
			return vec;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int i)
		{
			return extract_f32(vec, i);
		}
	};
	template<>
	struct SimdRegister<int, SimdExt::AVX512>
	{
		using Scalar = int;
		using Type = __m512i;
		static const int Width = 16;

		WEAVER_STRONG_INLINE static Type set(Scalar s0)
		{
			return _mm512_set1_epi32(s0);
		}
		WEAVER_STRONG_INLINE static Type set(
			Scalar s0,
			Scalar s1,
			Scalar s2,
			Scalar s3,
			Scalar s4,
			Scalar s5,
			Scalar s6,
			Scalar s7,
			Scalar s8,
			Scalar s9,
			Scalar s10,
			Scalar s11,
			Scalar s12,
			Scalar s13,
			Scalar s14,
			Scalar s15)
		{
			return _mm512_set_epi32(s15, s14, s13, s12, s11, s10, s9, s8, s7, s6, s5, s4, s3, s2, s1, s0);
		}
		WEAVER_STRONG_INLINE static Type set(Type vec)
		{
			return vec;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int i)
		{
			return extract_s32(vec, i);
		}
	};
	template<>
	struct SimdRegister<bool, SimdExt::AVX512>
	{
		using Scalar = bool;
		using Type = __mmask16;
		static const int Width = 16;

		WEAVER_STRONG_INLINE static Type set(Scalar s0)
		{
			const unsigned int mask = s0 ? 0xffffffff : 0;
			return _cvtu32_mask16(mask);
		}
		WEAVER_STRONG_INLINE static Type set(
			Scalar s0,
			Scalar s1,
			Scalar s2,
			Scalar s3,
			Scalar s4,
			Scalar s5,
			Scalar s6,
			Scalar s7,
			Scalar s8,
			Scalar s9,
			Scalar s10,
			Scalar s11,
			Scalar s12,
			Scalar s13,
			Scalar s14,
			Scalar s15)
		{
			unsigned int mask = 0;
			mask |= s0 ? 1 << 0 : 0;
			mask |= s1 ? 1 << 1 : 0;
			mask |= s2 ? 1 << 2 : 0;
			mask |= s3 ? 1 << 3 : 0;
			mask |= s4 ? 1 << 4 : 0;
			mask |= s5 ? 1 << 5 : 0;
			mask |= s6 ? 1 << 6 : 0;
			mask |= s7 ? 1 << 7 : 0;
			mask |= s8 ? 1 << 8 : 0;
			mask |= s9 ? 1 << 9 : 0;
			mask |= s10 ? 1 << 10 : 0;
			mask |= s11 ? 1 << 11 : 0;
			mask |= s12 ? 1 << 12 : 0;
			mask |= s13 ? 1 << 13 : 0;
			mask |= s14 ? 1 << 14 : 0;
			mask |= s15 ? 1 << 15 : 0;
			return _cvtu32_mask16(mask);
		}
		WEAVER_STRONG_INLINE static Type set(Type vec)
		{
			return vec;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int i)
		{
			return (_cvtmask16_u32(vec) & (1u << i)) != 0;
		}
	};
#endif

#ifdef WEAVER_VECTORIZE_NEON
	template<>
	struct SimdRegister<float, SimdExt::NEON>
	{
		using Scalar = float;
		using Type = float32x4_t;
		static const int Width = 4;

		WEAVER_STRONG_INLINE static Type set(Scalar s0) noexcept
		{
			return vdupq_n_f32(s0);
		}
		WEAVER_STRONG_INLINE static Type set(Scalar s0, Scalar s1, Scalar s2, Scalar s3) noexcept
		{
			alignas(8) float data[4] = { s0, s1, s2, s3 };
			return vld1q_f32(data);
		}
		WEAVER_STRONG_INLINE static Type set(Type vec) noexcept
		{
			return vec;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int i) noexcept
		{
			switch (i)
			{
			case 0:
				return vgetq_lane_f32(vec, 0);
			case 1:
				return vgetq_lane_f32(vec, 1);
			case 2:
				return vgetq_lane_f32(vec, 2);
			case 3:
				return vgetq_lane_f32(vec, 3);
			}
			return 0;
		}
	};
	template<>
	struct SimdRegister<int, SimdExt::NEON>
	{
		using Scalar = int;
		using Type = int32x4_t;
		static const int Width = 4;

		WEAVER_STRONG_INLINE static Type set(Scalar s0) noexcept
		{
			return vdupq_n_s32(s0);
		}
		WEAVER_STRONG_INLINE static Type set(Scalar s0, Scalar s1, Scalar s2, Scalar s3) noexcept
		{
			alignas(8) int data[4] = { s0, s1, s2, s3 };
			return vld1q_s32(data);
		}
		WEAVER_STRONG_INLINE static Type set(Type vec) noexcept
		{
			return vec;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int i) noexcept
		{
			switch (i)
			{
			case 0:
				return vgetq_lane_s32(vec, 0);
			case 1:
				return vgetq_lane_s32(vec, 1);
			case 2:
				return vgetq_lane_s32(vec, 2);
			case 3:
				return vgetq_lane_s32(vec, 3);
			}
			return 0;
		}
	};
	template<>
	struct SimdRegister<bool, SimdExt::NEON>
	{
		using Scalar = bool;
		using Type = uint32x4_t;
		static const int Width = 4;

		WEAVER_STRONG_INLINE static Type set(Scalar s0) noexcept
		{
			const uint32_t m0 = s0 ? uint32_t(-1) : 0;
			return vdupq_n_u32(m0);
		}
		WEAVER_STRONG_INLINE static Type set(Scalar s0, Scalar s1, Scalar s2, Scalar s3) noexcept
		{
			const uint32_t m0 = s0 ? uint32_t(-1) : 0;
			const uint32_t m1 = s1 ? uint32_t(-1) : 0;
			const uint32_t m2 = s2 ? uint32_t(-1) : 0;
			const uint32_t m3 = s3 ? uint32_t(-1) : 0;

			alignas(8) uint32_t data[4] = { m0, m1, m2, m3 };
			return vld1q_u32(data);
		}
		WEAVER_STRONG_INLINE static Type set(Type vec) noexcept
		{
			return vec;
		}
		WEAVER_STRONG_INLINE static Scalar get(Type vec, int i) noexcept
		{
			switch (i)
			{
			case 0:
				return vgetq_lane_u32(vec, 0) != 0;
			case 1:
				return vgetq_lane_u32(vec, 1) != 0;
			case 2:
				return vgetq_lane_u32(vec, 2) != 0;
			case 3:
				return vgetq_lane_u32(vec, 3) != 0;
			}
			return false;
		}
	};
#endif

	template<typename ScalarT, int Width, SimdExt Type>
	class VectorScalarBase
	{
	protected:
		using RegType = typename SimdRegister<ScalarT, Type>::Type;
		using Scalar = typename SimdRegister<ScalarT, Type>::Scalar;
		static constexpr int NrValues = Width;
		static constexpr int NrValuesPerReg = SimdRegister<Scalar, Type>::Width;
		static constexpr int NrRegs = NrValues / NrValuesPerReg;

		//! SIMD registers
		RegType _data[NrRegs];

		WEAVER_STRONG_INLINE RegType get(int i) const noexcept
		{
			assert(0 <= i && i < NrRegs);
			return _data[i];
		}
		WEAVER_STRONG_INLINE Scalar operator[](int idx) const noexcept
		{
			assert(0 <= idx && idx < Width);
			return SimdRegister<Scalar, Type>::get(get(idx / NrValuesPerReg), idx % NrValuesPerReg);
		}

		WEAVER_STRONG_INLINE void set(Scalar s) noexcept
		{
			for (int i = 0; i < NrRegs; i++)
				_data[i] = SimdRegister<Scalar, Type>::set(s);
		}

		template<typename... T>
		WEAVER_STRONG_INLINE void set(T... vals) noexcept
		{
			setImpl(SimdWidthTag<NrValuesPerReg>{}, 0, vals...);
		}

		template<typename T, size_t N, size_t... Is>
		WEAVER_STRONG_INLINE void set(const T(&arr)[N], std::index_sequence<Is...>) noexcept
		{
			set(arr[Is]...);
		}

		//! Assignment operator
		//! Define a custom assignment in order to support the compiler generating
		//! SIMD instructions for copying data
		WEAVER_STRONG_INLINE VectorScalarBase<Scalar, Width, Type>& operator=(const VectorScalarBase<Scalar, Width, Type>& rhs) noexcept
		{
			set(rhs._data, std::make_index_sequence<NrRegs>{});
			return *this;
		}

	private:
		template<int W>
		struct SimdWidthTag
		{};

		WEAVER_STRONG_INLINE void setImpl(SimdWidthTag<4>, int i, Scalar s0, Scalar s1, Scalar s2, Scalar s3) noexcept
		{
			_data[i] = SimdRegister<Scalar, Type>::set(s0, s1, s2, s3);
		}

		template<typename... T>
		WEAVER_STRONG_INLINE void setImpl(SimdWidthTag<4> tag, int i, Scalar s0, Scalar s1, Scalar s2, Scalar s3, T... vals) noexcept
		{
			_data[i] = SimdRegister<Scalar, Type>::set(s0, s1, s2, s3);
			setImpl(tag, i + 1, vals...);
		}

		WEAVER_STRONG_INLINE void setImpl(SimdWidthTag<8>, int i, Scalar s0, Scalar s1, Scalar s2, Scalar s3, Scalar s4, Scalar s5, Scalar s6, Scalar s7) noexcept
		{
			_data[i] = SimdRegister<Scalar, Type>::set(s0, s1, s2, s3, s4, s5, s6, s7);
		}

		template<typename... T>
		WEAVER_STRONG_INLINE void setImpl(SimdWidthTag<8> tag, int i, Scalar s0, Scalar s1, Scalar s2, Scalar s3, Scalar s4, Scalar s5, Scalar s6, Scalar s7, T... vals) noexcept
		{
			_data[i] = SimdRegister<Scalar, Type>::set(s0, s1, s2, s3, s4, s5, s6, s7);
			setImpl(tag, i + 1, vals...);
		}

		WEAVER_STRONG_INLINE void setImpl(
			SimdWidthTag<16>,
			int i,
			Scalar s0,
			Scalar s1,
			Scalar s2,
			Scalar s3,
			Scalar s4,
			Scalar s5,
			Scalar s6,
			Scalar s7,
			Scalar s8,
			Scalar s9,
			Scalar s10,
			Scalar s11,
			Scalar s12,
			Scalar s13,
			Scalar s14,
			Scalar s15)
		{
			_data[i] = SimdRegister<Scalar, Type>::set(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15);
		}

		template<typename... T>
		WEAVER_STRONG_INLINE void setImpl(
			SimdWidthTag<16> tag,
			int i,
			Scalar s0,
			Scalar s1,
			Scalar s2,
			Scalar s3,
			Scalar s4,
			Scalar s5,
			Scalar s6,
			Scalar s7,
			Scalar s8,
			Scalar s9,
			Scalar s10,
			Scalar s11,
			Scalar s12,
			Scalar s13,
			Scalar s14,
			Scalar s15,
			T... vals)
		{
			_data[i] = SimdRegister<Scalar, Type>::set(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15);
			setImpl(tag, i + 1, vals...);
		}

		template<int W>
		WEAVER_STRONG_INLINE void setImpl(SimdWidthTag<W>, int i, const RegType& v0) noexcept
		{
			_data[i] = SimdRegister<Scalar, Type>::set(v0);
		}

		template<int W, typename... T>
		WEAVER_STRONG_INLINE void setImpl(SimdWidthTag<W> tag, int i, const RegType& v0, T... vals) noexcept
		{
			_data[i] = SimdRegister<Scalar, Type>::set(v0);
			setImpl(tag, i + 1, vals...);
		}
	};

	template<typename Scalar, int Width>
	class VectorScalar
	{
	};

	template<typename T>
	WEAVER_STRONG_INLINE constexpr T min(const T& x) noexcept
	{
		return x;
	}

	template<typename T>
	WEAVER_STRONG_INLINE constexpr T max(const T& x) noexcept
	{
		return x;
	}

	WEAVER_STRONG_INLINE constexpr bool any(bool b) noexcept
	{
		return b;
	}

	WEAVER_STRONG_INLINE constexpr bool all(bool b) noexcept
	{
		return b;
	}

	WEAVER_STRONG_INLINE constexpr bool none(bool b) noexcept
	{
		return !b;
	}

	template<typename T>
	WEAVER_STRONG_INLINE T select(bool mask, const T& a, const T& b) noexcept
	{
		return mask ? a : b;
	}

	namespace details {
		template<typename T>
		WEAVER_STRONG_INLINE T nop(T a) noexcept
		{
			return a;
		}
		//! Add two floats
		//! \tparam T Scalar type used for operation
		//! \param a First summand
		//! \param b Second summand
		//! \returns The sum of \a and \b
		//! Helper function to allow for code generation in the VectorScalar specializations
		template<typename T>
		WEAVER_STRONG_INLINE T add(T a, T b) noexcept
		{
			return a + b;
		}
		template<typename T>
		WEAVER_STRONG_INLINE T sub(T a, T b) noexcept
		{
			return a - b;
		}
		template<typename T>
		WEAVER_STRONG_INLINE T mul(T a, T b) noexcept
		{
			return a * b;
		}
		template<typename T>
		WEAVER_STRONG_INLINE T div(T a, T b) noexcept
		{
			return a / b;
		}

		template<typename T>
		WEAVER_STRONG_INLINE bool cmpeq(T a, T b) noexcept
		{
			return a == b;
		}
		template<typename T>
		WEAVER_STRONG_INLINE bool cmpne(T a, T b) noexcept
		{
			return a != b;
		}
		template<typename T>
		WEAVER_STRONG_INLINE bool cmplt(T a, T b) noexcept
		{
			return a < b;
		}
		template<typename T>
		WEAVER_STRONG_INLINE bool cmple(T a, T b) noexcept
		{
			return a <= b;
		}
		template<typename T>
		WEAVER_STRONG_INLINE bool cmpgt(T a, T b) noexcept
		{
			return a > b;
		}
		template<typename T>
		WEAVER_STRONG_INLINE bool cmpge(T a, T b) noexcept
		{
			return a >= b;
		}

		//! Logical conjunction
		WEAVER_STRONG_INLINE constexpr bool conj(bool a, bool b) noexcept
		{
			return a && b;
		}
		//! Bitwise conjunction
		WEAVER_STRONG_INLINE constexpr int conj(int a, int b) noexcept
		{
			return a & b;
		}
		//! Logical disjunction
		WEAVER_STRONG_INLINE constexpr bool disj(bool a, bool b) noexcept
		{
			return a || b;
		}
		//! Bitwise disjunction
		WEAVER_STRONG_INLINE constexpr int disj(int a, int b) noexcept
		{
			return a | b;
		}

		WEAVER_STRONG_INLINE float max(float a, float b) noexcept
		{
#ifdef WEAVER_VECTORIZE_SSE
			return _mm_cvtss_f32(_mm_max_ss(_mm_set_ss(a), _mm_set_ss(b)));
#else
			return (a > b) ? a : b;
#endif
		}

		WEAVER_STRONG_INLINE float min(float a, float b) noexcept
		{
#ifdef WEAVER_VECTORIZE_SSE
			return _mm_cvtss_f32(_mm_min_ss(_mm_set_ss(a), _mm_set_ss(b)));
#else
			return (a < b) ? a : b;
#endif
		}

		template<typename T>
		WEAVER_STRONG_INLINE constexpr int sgn(T x, std::false_type) noexcept
		{
			return T(0) < x ? 1 : 0;
		}

		template<typename T>
		WEAVER_STRONG_INLINE constexpr int sgn(T x, std::true_type) noexcept
		{
			return (T(0) < x ? 1 : 0) - (x < T(0) ? 1 : 0);
		}

		template<typename T>
		WEAVER_STRONG_INLINE constexpr int sgn(T x) noexcept
		{
			return sgn(x, std::is_signed<T>());
	}

		WEAVER_STRONG_INLINE constexpr float sgn(float x) noexcept
		{
			return static_cast<float>((0.0f < x) - (x < 0.0f));
		}

		WEAVER_STRONG_INLINE float abs(float a) noexcept
		{
			return ::fabs(a);
		}

		WEAVER_STRONG_INLINE float sqrt(float a) noexcept
		{
			return ::sqrt(a);
		}

		WEAVER_STRONG_INLINE float rsqrt(float in) noexcept
		{
#ifdef WEAVER_VECTORIZE_SSE
			const __m128 v = _mm_set_ss(in);
			const __m128 nr = _mm_rsqrt_ss(v);
			const __m128 muls = _mm_mul_ss(_mm_mul_ss(nr, nr), v);
			const __m128 beta = _mm_mul_ss(_mm_set_ss(0.5f), nr);
			const __m128 gamma = _mm_sub_ss(_mm_set_ss(3.0f), muls);

			float out{ 0.0f };
			_mm_store_ss(&out, _mm_mul_ss(beta, gamma));
			return out;
#else
			return 1 / std::sqrt(in);
#endif
		}

		WEAVER_STRONG_INLINE float rcp(float f) noexcept
		{
#ifdef WEAVER_VECTORIZE_SSE
			const __m128 v = _mm_set_ss(f);
			const __m128 nr = _mm_rcp_ss(v);
			const __m128 muls = _mm_mul_ss(_mm_mul_ss(nr, nr), v);
			const __m128 dbl = _mm_add_ss(nr, nr);

			// Filter out zero input to ensure
			const __m128 mask = _mm_cmpeq_ss(v, _mm_setzero_ps());
			const __m128 filtered = _mm_andnot_ps(mask, muls);
			const __m128 result = _mm_sub_ss(dbl, filtered);

			float out{ 0.0f };
			_mm_store_ss(&out, result);
			return out;
#else
			return 1 / f;
#endif
		}
	}
}

#define WEAVER_SIMD_VECTORSCALAR_SETUP(SIMD_TYPE)                                                                                                       \
	using Base = weaver::VectorScalarBase<Scalar, NrValues, weaver::SimdExt::SIMD_TYPE>;                                                         \
	using Self = VectorScalar<Scalar, NrValues>;                                                                                                     \
	using Bool = VectorScalar<bool, NrValues>;                                                                                                       \
	using Base::operator[];                                                                                                                          \
	using Base::get;                                                                                                                                 \
	WEAVER_STRONG_INLINE VectorScalar() noexcept = default;                                                                                             \
	WEAVER_STRONG_INLINE VectorScalar(Scalar s) noexcept { set(s); }                                                                                    \
	template<typename... S>                                                                                                                          \
	explicit WEAVER_STRONG_INLINE VectorScalar(Scalar s0, Scalar s1, Scalar s2, Scalar s3, S... s) noexcept                                             \
	{                                                                                                                                                \
		static_assert(std::conjunction<std::is_convertible<S, Scalar>...>::value, "All parameters need to be convertible to the scalar base type");  \
		static_assert(sizeof...(S) == NrValues - 4, "Wrong number number of parameters");                                                            \
		set(s0, s1, s2, s3, static_cast<Scalar>(s)...);                                                                                              \
	}                                                                                                                                                \
	template<typename... V>                                                                                                                          \
	WEAVER_STRONG_INLINE explicit VectorScalar(const RegType& v0, V... v) noexcept                                                                      \
	{                                                                                                                                                \
		static_assert(sizeof...(V) == NrRegs - 1, "Wrong number number of parameters");                                                              \
		set(v0, v...);                                                                                                                               \
	}

#define WEAVER_SIMD_P1_1(op, i) op(get(i))
#define WEAVER_SIMD_P1_2(op, i) WEAVER_SIMD_P1_1(op, i), WEAVER_SIMD_P1_1(op, i + 1)
#define WEAVER_SIMD_P1_4(op, i) WEAVER_SIMD_P1_2(op, i), WEAVER_SIMD_P1_2(op, i + 2)
#define WEAVER_SIMD_P1_8(op, i) WEAVER_SIMD_P1_4(op, i), WEAVER_SIMD_P1_4(op, i + 4)
#define WEAVER_SIMD_P1_16(op, i) WEAVER_SIMD_P1_8(op, i), WEAVER_SIMD_P1_8(op, i + 8)

#define WEAVER_SIMD_P2_1(op, i) op(get(i), rhs.get(i))
#define WEAVER_SIMD_P2_2(op, i) WEAVER_SIMD_P2_1(op, i), WEAVER_SIMD_P2_1(op, i + 1)
#define WEAVER_SIMD_P2_4(op, i) WEAVER_SIMD_P2_2(op, i), WEAVER_SIMD_P2_2(op, i + 2)
#define WEAVER_SIMD_P2_8(op, i) WEAVER_SIMD_P2_4(op, i), WEAVER_SIMD_P2_4(op, i + 4)
#define WEAVER_SIMD_P2_16(op, i) WEAVER_SIMD_P2_8(op, i), WEAVER_SIMD_P2_8(op, i + 8)

#define WEAVER_SIMD_RED_P1_1(op1, op2, i) op1(get(i))
#define WEAVER_SIMD_RED_P1_2(op1, op2, i) op2(WEAVER_SIMD_RED_P1_1(op1, op2, i), WEAVER_SIMD_RED_P1_1(op1, op2, i + 1))
#define WEAVER_SIMD_RED_P1_4(op1, op2, i) op2(WEAVER_SIMD_RED_P1_2(op1, op2, i), WEAVER_SIMD_RED_P1_2(op1, op2, i + 2))
#define WEAVER_SIMD_RED_P1_8(op1, op2, i) op2(WEAVER_SIMD_RED_P1_4(op1, op2, i), WEAVER_SIMD_RED_P1_4(op1, op2, i + 4))
#define WEAVER_SIMD_RED_P1_16(op1, op2, i) op2(WEAVER_SIMD_RED_P1_8(op1, op2, i), WEAVER_SIMD_RED_P1_8(op1, op2, i + 8))

#define WEAVER_SIMD_RED_P2_1(op1, op2, i) op1(get(i), rhs.get(i))
#define WEAVER_SIMD_RED_P2_2(op1, op2, i) op2(WEAVER_SIMD_RED_P2_1(op1, op2, i), WEAVER_SIMD_RED_P2_1(op1, op2, i + 1))
#define WEAVER_SIMD_RED_P2_4(op1, op2, i) op2(WEAVER_SIMD_RED_P2_2(op1, op2, i), WEAVER_SIMD_RED_P2_2(op1, op2, i + 2))
#define WEAVER_SIMD_RED_P2_8(op1, op2, i) op2(WEAVER_SIMD_RED_P2_4(op1, op2, i), WEAVER_SIMD_RED_P2_4(op1, op2, i + 4))
#define WEAVER_SIMD_RED_P2_16(op1, op2, i) op2(WEAVER_SIMD_RED_P2_8(op1, op2, i), WEAVER_SIMD_RED_P2_8(op1, op2, i + 8))

#define WEAVER_SIMD_UNARY_OP(name, op, N) \
	WEAVER_STRONG_INLINE Self name() const noexcept { return Self{ WEAVER_PP_JOIN(WEAVER_SIMD_P1_, N)(op, 0) }; }
#define WEAVER_SIMD_BINARY_OP(name, op, N) \
	WEAVER_STRONG_INLINE Self name(const Self& rhs) const noexcept { return Self{ WEAVER_PP_JOIN(WEAVER_SIMD_P2_, N)(op, 0) }; }
#define WEAVER_SIMD_ASSIGN_OP(name, op, N)                    \
	WEAVER_STRONG_INLINE Self& name(const Self& rhs) noexcept \
	{                                                      \
		set(WEAVER_PP_JOIN(WEAVER_SIMD_P2_, N)(op, 0));        \
		return *this;                                      \
	}
#define WEAVER_SIMD_COMP_OP(name, op, N) \
	WEAVER_STRONG_INLINE Bool name(const Self& rhs) const noexcept { return Bool{ WEAVER_PP_JOIN(WEAVER_SIMD_P2_, N)(op, 0) }; }
#define WEAVER_SIMD_QUERY_OP(name, op, N) \
	WEAVER_STRONG_INLINE Bool name() const noexcept { return Bool{ WEAVER_PP_JOIN(WEAVER_SIMD_P1_, N)(op, 0) }; }
#define WEAVER_SIMD_UNARY_REDUCTION_OP(name, op1, op2, N) \
	WEAVER_STRONG_INLINE Scalar name() const noexcept { return Scalar{ WEAVER_PP_JOIN(WEAVER_SIMD_RED_P1_, N)(op1, op2, 0) }; }
#define WEAVER_SIMD_BINARY_REDUCTION_OP(name, op1, op2, N) \
	WEAVER_STRONG_INLINE Scalar name(const Self& rhs) const noexcept { return Scalar{ WEAVER_PP_JOIN(WEAVER_SIMD_RED_P2_, N)(op1, op2, 0) }; }
