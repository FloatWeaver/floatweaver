// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

#include <weaver/config.h>

 // C++ standard library
#include <cstdint>
#include <limits>

#if defined(WEAVER_VECTORIZE_SSE)

#	define WEAVER_M128I_SIGNBIT _mm_set1_epi32(int(0x80000000))
#	define WEAVER_M128I_ALLBITS _mm_set1_epi32(int(0xffffffff))

namespace weaver {

	/// Per element absolut value
	WEAVER_STRONG_INLINE __m128 abs_f32(__m128 a) noexcept
	{
		// Compute abs using logical operations
		return _mm_andnot_ps(_mm_castsi128_ps(WEAVER_M128I_SIGNBIT), a);
	}

	WEAVER_STRONG_INLINE __m128i abs_s32(__m128i a) noexcept
	{
#	ifdef WEAVER_VECTORIZE_SSSE3
		return _mm_abs_epi32(a);
#	else
		__m128i mask = _mm_cmplt_epi32(a, _mm_setzero_si128()); // FFFF   where a < 0
		a = _mm_xor_si128(a, mask);                             // Invert where a < 0
		mask = _mm_srli_epi32(mask, 31);                        // 0001   where a < 0
		a = _mm_add_epi32(a, mask);                             // Add 1  where a < 0
		return a;
#	endif
	}

	WEAVER_STRONG_INLINE __m128 blend_f32(__m128 a, __m128 b, __m128 mask) noexcept
	{
#	ifdef WEAVER_VECTORIZE_SSE4_1
		return _mm_blendv_ps(a, b, mask);
#	else
		// Straight forward method
		// (a & ~mask) | (b & mask)
		return _mm_or_ps(_mm_andnot_ps(mask, a), _mm_and_ps(mask, b));

		// xor-method
		// (((a ^ b) & mask)^a)
		//return _mm_xor_ps(a, _mm_and_ps(mask, _mm_xor_ps(a, b)));
#	endif
	}

	WEAVER_STRONG_INLINE __m128i blend_s32(__m128i a, __m128i b, __m128 mask) noexcept
	{
#	ifdef WEAVER_VECTORIZE_SSE4_1
		return _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b), mask));
#	else
		// Straight forward method
		// (a & ~mask) | (b & mask)
		return _mm_or_si128(_mm_andnot_si128(_mm_castps_si128(mask), a), _mm_and_si128(_mm_castps_si128(mask), b));

		// xor-method
		// (((a ^ b) & mask)^a)
		//return _mm_xor_si128(a, _mm_and_si128(_mm_castps_si128(mask), _mm_xor_si128(a, b)));
#	endif
	}

	WEAVER_STRONG_INLINE __m128i max_s32(__m128i a, __m128i b) noexcept
	{
#	ifdef WEAVER_VECTORIZE_SSE4_1
		return _mm_max_epi32(a, b);
#	else
		__m128i mask = _mm_cmpgt_epi32(a, b);
		a = blend_s32(b, a, _mm_castsi128_ps(mask));
		return a;
#	endif
	}
	WEAVER_STRONG_INLINE __m128i min_s32(__m128i a, __m128i b) noexcept
	{
#	ifdef WEAVER_VECTORIZE_SSE4_1
		return _mm_min_epi32(a, b);
#	else
		__m128i mask = _mm_cmplt_epi32(a, b);
		a = blend_s32(b, a, _mm_castsi128_ps(mask));
		return a;
#	endif
	}

	WEAVER_STRONG_INLINE __m128 sgn_f32(__m128 v) noexcept
	{
		return _mm_and_ps(_mm_or_ps(_mm_and_ps(v, _mm_castsi128_ps(WEAVER_M128I_SIGNBIT)), _mm_set1_ps(1.0f)), _mm_cmpneq_ps(v, _mm_setzero_ps()));
	}

	WEAVER_STRONG_INLINE float extract_f32(__m128 v, int i) noexcept
	{
		using F32 = union
		{
			__m128 x;
			float a[4];
		};

		return F32{ v }.a[i];
	}

	WEAVER_STRONG_INLINE int extract_s32(__m128i v, int i) noexcept
	{
		using U32 = union
		{
			__m128i x;
			int32_t a[4];
		};

		return U32{ v }.a[i];
	}
}

WEAVER_STRONG_INLINE __m128i _mm_cmpneq_epi32(__m128i a, __m128i b) noexcept
{
	return _mm_andnot_si128(_mm_cmpeq_epi32(a, b), WEAVER_M128I_ALLBITS);
}
WEAVER_STRONG_INLINE __m128i _mm_cmple_epi32(__m128i a, __m128i b) noexcept
{
	return _mm_andnot_si128(_mm_cmpgt_epi32(a, b), WEAVER_M128I_ALLBITS);
}
WEAVER_STRONG_INLINE __m128i _mm_cmpge_epi32(__m128i a, __m128i b) noexcept
{
	return _mm_andnot_si128(_mm_cmplt_epi32(a, b), WEAVER_M128I_ALLBITS);
}

WEAVER_STRONG_INLINE __m128 _mm_isinf_ps(__m128 x) noexcept
{
	const __m128 sign_mask = _mm_set1_ps(-0.0);
	const __m128 inf = _mm_set1_ps(std::numeric_limits<float>::infinity());

	x = _mm_andnot_ps(sign_mask, x);
	x = _mm_cmpeq_ps(x, inf);
	return x;
}

#	if !defined(WEAVER_COMPILER_MSVC) || _MSC_VER < 1920
__m128 _mm_sin_ps(__m128 v) noexcept;
__m128 _mm_cos_ps(__m128 v) noexcept;
__m128 _mm_log_ps(__m128 v) noexcept;
__m128 _mm_exp_ps(__m128 v) noexcept;

__m128 _mm_acos_ps(__m128 v) noexcept;
__m128 _mm_asin_ps(__m128 v) noexcept;
__m128 _mm_atan2_ps(__m128 in_y, __m128 in_x) noexcept;

__m128 _mm_pow_ps(__m128 x, __m128 y) noexcept;
#	endif

namespace weaver {
	__m128 _mmWEAVER_floor_ps(__m128 x) noexcept;

	WEAVER_STRONG_INLINE __m128i _mmWEAVER_mullo_epi32(__m128i a, __m128i b) noexcept
	{
#	ifdef WEAVER_VECTORIZE_SSE4_1
		return _mm_mullo_epi32(a, b);
#	else
		const __m128i tmp1 = _mm_mul_epu32(a, b);                                                                                      /* mul 2,0*/
		const __m128i tmp2 = _mm_mul_epu32(_mm_srli_si128(a, 4), _mm_srli_si128(b, 4));                                                /* mul 3,1 */
		return _mm_unpacklo_epi32(_mm_shuffle_epi32(tmp1, _MM_SHUFFLE(0, 0, 2, 0)), _mm_shuffle_epi32(tmp2, _MM_SHUFFLE(0, 0, 2, 0))); /* shuffle results to [63..0] and pack */
#	endif
	}

	// AP-803 Newton-Raphson Method with Streaming SIMD Extensions
	WEAVER_STRONG_INLINE __m128 _mmWEAVER_rsqrt_ps(__m128 v) noexcept
	{
		const __m128 nr = _mm_rsqrt_ps(v);
		const __m128 muls = _mm_mul_ps(_mm_mul_ps(nr, nr), v);
		const __m128 beta = _mm_mul_ps(_mm_set1_ps(0.5f), nr);
		const __m128 gamma = _mm_sub_ps(_mm_set1_ps(3.0f), muls);

		return _mm_mul_ps(beta, gamma);
	}

	// AP-803 Newton-Raphson Method with Streaming SIMD Extensions
	WEAVER_STRONG_INLINE __m128 _mmWEAVER_rcp_ps(__m128 v) noexcept
	{
		const __m128 nr = _mm_rcp_ps(v);
		const __m128 muls = _mm_mul_ps(_mm_mul_ps(nr, nr), v);
		const __m128 dbl = _mm_add_ps(nr, nr);

		// Filter out zero input to ensure
		const __m128 mask = _mm_cmpeq_ps(v, _mm_setzero_ps());
		const __m128 filtered = _mm_andnot_ps(mask, muls);
		const __m128 result = _mm_sub_ps(dbl, filtered);

		return result;
	}

	WEAVER_STRONG_INLINE float _mmWEAVER_hmin_ps(__m128 v) noexcept
	{
		const __m128 data = v;                                                              /* [0, 1, 2, 3] */
		const __m128 low = _mm_movehl_ps(data, data);                                       /* [2, 3, 2, 3] */
		const __m128 low_accum = _mm_min_ps(low, data);                                     /* [0|2, 1|3, 2|2, 3|3] */
		const __m128 elem1 = _mm_shuffle_ps(low_accum, low_accum, _MM_SHUFFLE(1, 1, 1, 1)); /* [1|3, 1|3, 1|3, 1|3] */
		const __m128 accum = _mm_min_ss(low_accum, elem1);
		return _mm_cvtss_f32(accum);
	}

	WEAVER_STRONG_INLINE float _mmWEAVER_hmax_ps(__m128 v) noexcept
	{
		const __m128 data = v;                                                                /* [0, 1, 2, 3] */
		const __m128 high = _mm_movehl_ps(data, data);                                        /* [2, 3, 2, 3] */
		const __m128 high_accum = _mm_max_ps(high, data);                                     /* [0|2, 1|3, 2|2, 3|3] */
		const __m128 elem1 = _mm_shuffle_ps(high_accum, high_accum, _MM_SHUFFLE(1, 1, 1, 1)); /* [1|3, 1|3, 1|3, 1|3] */
		const __m128 accum = _mm_max_ss(high_accum, elem1);
		return _mm_cvtss_f32(accum);
	}

	WEAVER_STRONG_INLINE float _mmWEAVER_dp_ps(__m128 a, __m128 b) noexcept
	{
		using F32 = union
		{
			__m128 x;
			float a[4];
		};

#	ifdef WEAVER_VECTORIZE_SSE4_1
		return F32{ _mm_dp_ps(a, b, 0xff) }.a[0];
#	elif defined WEAVER_VECTORIZE_SSE3
		const __m128 ab = _mm_mul_ps(a, b);
		const __m128 dp = _mm_hadd_ps(ab, ab);
		return F32{ dp }.a[0] + F32{ dp }.a[1];
#	else
		const __m128 ab = _mm_mul_ps(a, b);
		return F32{ ab }.a[0] + F32{ ab }.a[1] + F32{ ab }.a[2] + F32{ ab }.a[3];
#	endif
	}

#	ifdef WEAVER_VECTORIZE_SSE4_1
#		define _mmWEAVER_insert_ps _mm_insert_ps
#	else
	WEAVER_STRONG_INLINE __m128 _mmWEAVER_insert_ps(__m128 a, __m128 b, const int sel) noexcept
	{
		using F32 = union
		{
			__m128 x;
			float a[4];
		};

		float tmp;
		int count_d, zmask;

		F32 A, B;
		A.x = a;
		B.x = b;

		tmp = B.a[(sel & 0xC0) >> 6]; // 0xC0 = sel[7:6]
		count_d = (sel & 0x30) >> 4;  // 0x30 = sel[5:4]
		zmask = sel & 0x0F;           // 0x0F = sel[3:0]

		A.a[count_d] = tmp;

		A.a[0] = (zmask & 0x1) ? 0 : A.a[0];
		A.a[1] = (zmask & 0x2) ? 0 : A.a[1];
		A.a[2] = (zmask & 0x4) ? 0 : A.a[2];
		A.a[3] = (zmask & 0x8) ? 0 : A.a[3];
		return A.x;
	}
#	endif
}
#endif
