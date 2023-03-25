// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#pragma once

#include <weaver/config.h>

// C++ standard library
#include <cstdint>

#ifdef WEAVER_VECTORIZE_AVX512

#	include <weaver/intrinsics_avx.h>

#	define WEAVER_M512I_SIGNBIT _mm512_set1_epi32(int(0x80000000))
#	define WEAVER_M512I_ALLBITS _mm512_set1_epi32(int(0xffffffff))

namespace weaver {
	WEAVER_STRONG_INLINE __m512 _mm512_sgn_ps(__m512 v)
	{
		const __mmask16 is_eq_zero = _mm512_cmp_ps_mask(v, _mm512_setzero_ps(), _CMP_EQ_OQ);
		return _mm512_castsi512_ps(_mm512_mask_set1_epi32(
			_mm512_or_epi32(
				_mm512_and_epi32(_mm512_castps_si512(v), WEAVER_M512I_SIGNBIT),
				_mm512_castps_si512(_mm512_set1_ps(1.0f))),
			is_eq_zero, 0));
	}

#	if !defined(WEAVER_COMPILER_MSVC)
	__m512 _mm512_sin_ps(__m512 v);
	__m512 _mm512_cos_ps(__m512 v);
	__m512 _mm512_log_ps(__m512 v);
	__m512 _mm512_exp_ps(__m512 v);

	__m512 _mm512_acos_ps(__m512 v);
	__m512 _mm512_asin_ps(__m512 v);
	__m512 _mm512_atan2_ps(__m512 y, __m512 x);

	__m512 _mm512_pow_ps(__m512 x, __m512 y);
#	endif

	WEAVER_STRONG_INLINE __mmask16 _mm512_cmpeq_ps(__m512 a, __m512 b)
	{
		return _mm512_cmp_ps_mask(a, b, _CMP_EQ_OQ);
	}
	WEAVER_STRONG_INLINE __mmask16 _mm512_cmpneq_ps(__m512 a, __m512 b) { return _mm512_cmp_ps_mask(a, b, _CMP_NEQ_OQ); }
	WEAVER_STRONG_INLINE __mmask16 _mm512_cmplt_ps(__m512 a, __m512 b) { return _mm512_cmp_ps_mask(a, b, _CMP_LT_OQ); }
	WEAVER_STRONG_INLINE __mmask16 _mm512_cmple_ps(__m512 a, __m512 b) { return _mm512_cmp_ps_mask(a, b, _CMP_LE_OQ); }
	WEAVER_STRONG_INLINE __mmask16 _mm512_cmpgt_ps(__m512 a, __m512 b) { return _mm512_cmp_ps_mask(a, b, _CMP_GT_OQ); }
	WEAVER_STRONG_INLINE __mmask16 _mm512_cmpge_ps(__m512 a, __m512 b) { return _mm512_cmp_ps_mask(a, b, _CMP_GE_OQ); }

	WEAVER_STRONG_INLINE __mmask16 _mm512_isinf_ps(__m512 x)
	{
		const __m512 sign_mask = _mm512_set1_ps(-0.0);
		const __m512 inf = _mm512_set1_ps(std::numeric_limits<float>::infinity());

		x = _mm512_andnot_ps(sign_mask, x);
		return _mm512_cmpeq_ps(x, inf);
	}

	WEAVER_STRONG_INLINE __m512 _mmWEAVER_rsqrt_ps(__m512 v)
	{
		const __m512 nr = _mm512_rsqrt14_ps(v);
		const __m512 muls = _mm512_mul_ps(_mm512_mul_ps(nr, nr), v);
		const __m512 beta = _mm512_mul_ps(_mm512_set1_ps(0.5f), nr);
		const __m512 gamma = _mm512_sub_ps(_mm512_set1_ps(3.0f), muls);

		return _mm512_mul_ps(beta, gamma);
	}

	WEAVER_STRONG_INLINE __m512 _mmWEAVER_rcp_ps(__m512 v)
	{
		const __m512 nr = _mm512_rcp14_ps(v);
		const __m512 muls = _mm512_mul_ps(_mm512_mul_ps(nr, nr), v);
		const __m512 dbl = _mm512_add_ps(nr, nr);

		// Filter out zero input to ensure
		const __mmask16 is_neq_zero_mask = _mm512_cmpneq_ps(v, _mm512_setzero_ps());
		const __m512 result = _mm512_mask_sub_ps(dbl, is_neq_zero_mask, dbl, muls);

		return result;
	}

	WEAVER_STRONG_INLINE float _mmWEAVER_hmin_ps(__m512 v)
	{
		return _mm512_reduce_min_ps(v);
	}

	WEAVER_STRONG_INLINE float _mmWEAVER_hmax_ps(__m512 v)
	{
		return _mm512_reduce_max_ps(v);
	}

	WEAVER_STRONG_INLINE float _mmWEAVER_dp_ps(__m512 a, __m512 b)
	{
		return _mm512_reduce_add_ps(_mm512_mul_ps(a, b));
	}

	WEAVER_STRONG_INLINE float _mmWEAVER_extract_ps(__m512 v, int i)
	{
		typedef union
		{
			__m512 x;
			float a[16];
		} F32;

		return F32{ v }.a[i];
	}

	WEAVER_STRONG_INLINE int _mmWEAVER_extract_epi32(__m512i v, int i)
	{
		typedef union
		{
			__m512i x;
			int32_t a[16];
		} U32;

		return U32{ v }.a[i];
	}
}
#endif
