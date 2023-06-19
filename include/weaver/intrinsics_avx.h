// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

#include <weaver/config.h>

// C++ standard library
#include <cstdint>

#ifdef WEAVER_VECTORIZE_AVX

#	include <weaver/intrinsics_sse.h>

// Macros for GCC compatibility
#	ifndef _mm256_set_m128
#		define _mm256_set_m128(/* __m128 */ hi, /* __m128 */ lo) _mm256_insertf128_ps(_mm256_castps128_ps256(lo), (hi), 0x1)
#	endif // _mm256_set_m128

#	ifndef _mm256_set_m128d
#		define _mm256_set_m128d(/* __m128d */ hi, /* __m128d */ lo) _mm256_insertf128_pd(_mm256_castpd128_pd256(lo), (hi), 0x1)
#	endif // _mm256_set_m128d

#	ifndef _mm256_set_m128i
#		define _mm256_set_m128i(/* __m128i */ hi, /* __m128i */ lo) _mm256_insertf128_si256(_mm256_castsi128_si256(lo), (hi), 0x1)
#	endif // _mm256_set_m128i

#	define WEAVER_M256I_SIGNBIT _mm256_set1_epi32(int(0x80000000))
#	define WEAVER_M256I_ALLBITS _mm256_set1_epi32(int(0xffffffff))

namespace weaver {
	WEAVER_STRONG_INLINE __m256 _mm256_abs_ps(__m256 v)
	{
		return _mm256_andnot_ps(_mm256_castsi256_ps(WEAVER_M256I_SIGNBIT), v);
	}
	WEAVER_STRONG_INLINE __m256 _mm256_sgn_ps(__m256 v)
	{
		return _mm256_and_ps(_mm256_or_ps(_mm256_and_ps(v, _mm256_castsi256_ps(WEAVER_M256I_SIGNBIT)), _mm256_set1_ps(1.0f)), _mm256_cmp_ps(v, _mm256_setzero_ps(), _CMP_NEQ_OQ));
	}

#	ifdef WEAVER_VECTORIZE_AVX2
	WEAVER_STRONG_INLINE __m256i _mm256_cmplt_epi32(__m256i a, __m256i b)
	{
		return _mm256_cmpgt_epi32(b, a);
	}
	WEAVER_STRONG_INLINE __m256i _mm256_cmpneq_epi32(__m256i a, __m256i b)
	{
		return _mm256_andnot_si256(_mm256_cmpeq_epi32(a, b), WEAVER_M256I_ALLBITS);
	}
	WEAVER_STRONG_INLINE __m256i _mm256_cmple_epi32(__m256i a, __m256i b)
	{
		return _mm256_andnot_si256(_mm256_cmpgt_epi32(a, b), WEAVER_M256I_ALLBITS);
	}
	WEAVER_STRONG_INLINE __m256i _mm256_cmpge_epi32(__m256i a, __m256i b)
	{
		return _mm256_andnot_si256(_mm256_cmplt_epi32(a, b), WEAVER_M256I_ALLBITS);
	}
#	endif

#	if !defined(WEAVER_COMPILER_MSVC) || _MSC_VER < 1920
	__m256 _mm256_sin_ps(__m256 v);
	__m256 _mm256_cos_ps(__m256 v);
	__m256 _mm256_log_ps(__m256 v);
	__m256 _mm256_exp_ps(__m256 v);

	__m256 _mm256_acos_ps(__m256 v);
	__m256 _mm256_asin_ps(__m256 v);
	__m256 _mm256_atan2_ps(__m256 y, __m256 x);

	__m256 _mm256_pow_ps(__m256 x, __m256 y);
#	endif

	WEAVER_STRONG_INLINE __m256 _mm256_cmpeq_ps(__m256 a, __m256 b)
	{
		return _mm256_cmp_ps(a, b, _CMP_EQ_OQ);
	}
	WEAVER_STRONG_INLINE __m256 _mm256_cmpneq_ps(__m256 a, __m256 b) { return _mm256_cmp_ps(a, b, _CMP_NEQ_OQ); }
	WEAVER_STRONG_INLINE __m256 _mm256_cmplt_ps(__m256 a, __m256 b) { return _mm256_cmp_ps(a, b, _CMP_LT_OQ); }
	WEAVER_STRONG_INLINE __m256 _mm256_cmple_ps(__m256 a, __m256 b) { return _mm256_cmp_ps(a, b, _CMP_LE_OQ); }
	WEAVER_STRONG_INLINE __m256 _mm256_cmpgt_ps(__m256 a, __m256 b) { return _mm256_cmp_ps(a, b, _CMP_GT_OQ); }
	WEAVER_STRONG_INLINE __m256 _mm256_cmpge_ps(__m256 a, __m256 b) { return _mm256_cmp_ps(a, b, _CMP_GE_OQ); }

	WEAVER_STRONG_INLINE __m256 _mm256_isinf_ps(__m256 x)
	{
		const __m256 sign_mask = _mm256_set1_ps(-0.0);
		const __m256 inf = _mm256_set1_ps(std::numeric_limits<float>::infinity());

		x = _mm256_andnot_ps(sign_mask, x);
		x = _mm256_cmpeq_ps(x, inf);
		return x;
	}

	WEAVER_STRONG_INLINE __m256 _mmWEAVER_rsqrt_ps(__m256 v)
	{
		const __m256 nr = _mm256_rsqrt_ps(v);
		const __m256 muls = _mm256_mul_ps(_mm256_mul_ps(nr, nr), v);
		const __m256 beta = _mm256_mul_ps(_mm256_set1_ps(0.5f), nr);
		const __m256 gamma = _mm256_sub_ps(_mm256_set1_ps(3.0f), muls);

		return _mm256_mul_ps(beta, gamma);
	}

	WEAVER_STRONG_INLINE __m256 _mmWEAVER_rcp_ps(__m256 v)
	{
		const __m256 nr = _mm256_rcp_ps(v);
		const __m256 muls = _mm256_mul_ps(_mm256_mul_ps(nr, nr), v);
		const __m256 dbl = _mm256_add_ps(nr, nr);

		// Filter out zero input to ensure
		const __m256 mask = _mm256_cmpeq_ps(v, _mm256_setzero_ps());
		const __m256 filtered = _mm256_andnot_ps(mask, muls);
		const __m256 result = _mm256_sub_ps(dbl, filtered);

		return result;
	}

	WEAVER_STRONG_INLINE __m256i _mmWEAVER_add_epi32(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_add_epi32(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_add_epi32(x0, y0);
		const __m128i z1 = _mm_add_epi32(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}
	WEAVER_STRONG_INLINE __m256i _mmWEAVER_sub_epi32(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_sub_epi32(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_sub_epi32(x0, y0);
		const __m128i z1 = _mm_sub_epi32(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}
	WEAVER_STRONG_INLINE __m256i _mmWEAVER_mullo_epi32(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_mullo_epi32(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_mullo_epi32(x0, y0);
		const __m128i z1 = _mm_mullo_epi32(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}

	WEAVER_STRONG_INLINE __m256i _mmWEAVER_max_epi32(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_max_epi32(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_max_epi32(x0, y0);
		const __m128i z1 = _mm_max_epi32(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}

	WEAVER_STRONG_INLINE __m256i _mmWEAVER_min_epi32(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_min_epi32(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_min_epi32(x0, y0);
		const __m128i z1 = _mm_min_epi32(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}

	WEAVER_STRONG_INLINE __m256i _mmWEAVER_abs_epi32(__m256i x)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_abs_epi32(x);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm_abs_epi32(x0);
		const __m128i y1 = _mm_abs_epi32(x1);

		return _mm256_set_m128i(y1, y0);
#	endif
	}

	WEAVER_STRONG_INLINE __m256i _mmWEAVER_cmpeq_epi32(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_cmpeq_epi32(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_cmpeq_epi32(x0, y0);
		const __m128i z1 = _mm_cmpeq_epi32(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}
	WEAVER_STRONG_INLINE __m256i _mmWEAVER_cmpneq_epi32(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_cmpneq_epi32(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_cmpneq_epi32(x0, y0);
		const __m128i z1 = _mm_cmpneq_epi32(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}
	WEAVER_STRONG_INLINE __m256i _mmWEAVER_cmplt_epi32(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_cmplt_epi32(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_cmplt_epi32(x0, y0);
		const __m128i z1 = _mm_cmplt_epi32(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}
	WEAVER_STRONG_INLINE __m256i _mmWEAVER_cmple_epi32(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_cmple_epi32(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_cmple_epi32(x0, y0);
		const __m128i z1 = _mm_cmple_epi32(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}
	WEAVER_STRONG_INLINE __m256i _mmWEAVER_cmpgt_epi32(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_cmpgt_epi32(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_cmpgt_epi32(x0, y0);
		const __m128i z1 = _mm_cmpgt_epi32(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}
	WEAVER_STRONG_INLINE __m256i _mmWEAVER_cmpge_epi32(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_cmpge_epi32(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_cmpge_epi32(x0, y0);
		const __m128i z1 = _mm_cmpge_epi32(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}

	WEAVER_STRONG_INLINE __m256i _mmWEAVER_and_si256(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_and_si256(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_and_si128(x0, y0);
		const __m128i z1 = _mm_and_si128(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}
	WEAVER_STRONG_INLINE __m256i _mmWEAVER_andnot_si256(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_andnot_si256(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_andnot_si128(x0, y0);
		const __m128i z1 = _mm_andnot_si128(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}
	WEAVER_STRONG_INLINE __m256i _mmWEAVER_or_si256(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_or_si256(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_or_si128(x0, y0);
		const __m128i z1 = _mm_or_si128(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}
	WEAVER_STRONG_INLINE __m256i _mmWEAVER_xor_si256(__m256i x, __m256i y)
	{
#	ifdef WEAVER_VECTORIZE_AVX2
		return _mm256_xor_si256(x, y);
#	else
		const __m128i x0 = _mm256_extractf128_si256(x, 0);
		const __m128i x1 = _mm256_extractf128_si256(x, 1);

		const __m128i y0 = _mm256_extractf128_si256(y, 0);
		const __m128i y1 = _mm256_extractf128_si256(y, 1);

		const __m128i z0 = _mm_xor_si128(x0, y0);
		const __m128i z1 = _mm_xor_si128(x1, y1);

		return _mm256_set_m128i(z1, z0);
#	endif
	}

	WEAVER_STRONG_INLINE float _mmWEAVER_hmin_ps(__m256 v)
	{
		const __m256 hilo = _mm256_permute2f128_ps(v, v, 0x81);
		__m256 redux = _mm256_min_ps(v, hilo);
		redux = _mm256_min_ps(redux, _mm256_shuffle_ps(redux, redux, 0x0e));
		redux = _mm256_min_ps(redux, _mm256_shuffle_ps(redux, redux, 0x01));

		return _mm_cvtss_f32(_mm256_castps256_ps128(redux));
	}

	WEAVER_STRONG_INLINE float _mmWEAVER_hmax_ps(__m256 v)
	{
		const __m256 hilo = _mm256_permute2f128_ps(v, v, 0x81);
		__m256 redux = _mm256_max_ps(v, hilo);
		redux = _mm256_max_ps(redux, _mm256_shuffle_ps(redux, redux, 0x0e));
		redux = _mm256_max_ps(redux, _mm256_shuffle_ps(redux, redux, 0x01));

		return _mm_cvtss_f32(_mm256_castps256_ps128(redux));
	}

	WEAVER_STRONG_INLINE float _mmWEAVER_dp_ps(__m256 a, __m256 b)
	{
		using F32 = union
		{
			__m256 x;
			float a[8];
		};

		const __m256 dp = _mm256_dp_ps(a, b, 0xff);
		return F32{ dp }.a[0] + F32{ dp }.a[4];
	}

	WEAVER_STRONG_INLINE float extract_f32(__m256 v, int i) noexcept
	{
		using F32 = union
		{
			__m256 x;
			float a[8];
		};

		return F32{ v }.a[i];
	}

	WEAVER_STRONG_INLINE int extract_s32(__m256i v, int i) noexcept
	{
		using U32 = union
		{
			__m256i x;
			int32_t a[8];
		};

		return U32{ v }.a[i];
	}
}
#endif
