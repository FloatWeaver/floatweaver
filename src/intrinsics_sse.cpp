// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#include <weaver/intrinsics_sse.h>

#if defined(WEAVER_VECTORIZE_SSE)
WEAVER_BEGIN_EXTERNAL_HEADERS
#	define USE_SSE2
#	include "detail/sse_mathfun.h"

WEAVER_END_EXTERNAL_HEADERS

#	if !defined(WEAVER_COMPILER_MSVC) || _MSC_VER < 1920
	__m128 _mm_sin_ps(__m128 v) noexcept
{
	return sin_ps(v);
}

__m128 _mm_cos_ps(__m128 v) noexcept
{
	return cos_ps(v);
}

__m128 _mm_log_ps(__m128 v) noexcept
{
	return log_ps(v);
}

__m128 _mm_exp_ps(__m128 v) noexcept
{
	return exp_ps(v);
}

__m128 _mm_pow_ps(__m128 x, __m128 y) noexcept
{
	return _mm_exp_ps(_mm_mul_ps(_mm_log_ps(x), y));
}

// Handbook of Mathematical Functions
// M. Abramowitz and I.A. Stegun, Ed.
__m128 _mm_acos_ps(__m128 v) noexcept
{
	float4 x{ v };

	// Absolute error <= 6.7e-5
	const float4 negate = select(x < 0, float4{ 1 }, float4{ 0 });

	x = x.abs();

	float4 ret = -0.0187293f;
	ret = ret * x;
	ret = ret + 0.0742610f;
	ret = ret * x;
	ret = ret - 0.2121144f;
	ret = ret * x;
	ret = ret + 1.5707288f;
	ret = ret * (1.0f - x).sqrt();
	ret = ret - 2.0f * negate * ret;
	return (negate * 3.14159265358979f + ret).get(0);
}

// Handbook of Mathematical Functions
// M. Abramowitz and I.A. Stegun, Ed.
__m128 _mm_asin_ps(__m128 v) noexcept
{
	float4 x{ v };

	const float4 negate = select(x < 0, float4{ 1 }, float4{ 0 });

	x = abs(x);
	float4 ret = -0.0187293f;
	ret *= x;
	ret += 0.0742610f;
	ret *= x;
	ret -= 0.2121144f;
	ret *= x;
	ret += 1.5707288f;
	ret = 3.14159265358979f * 0.5f - sqrt(1.0f - x) * ret;
	return (ret - 2.0f * negate * ret).get(0);
}

__m128 _mm_atan2_ps(__m128 in_y, __m128 in_x) noexcept
{
	float4 t0, t1, t3, t4;

	float4 x{ in_x };
	float4 y{ in_y };

	t3 = abs(x);
	t1 = abs(y);
	t0 = max(t3, t1);
	t1 = min(t3, t1);
	t3 = 1.0f / t0;
	t3 = t1 * t3;

	t4 = t3 * t3;
	t0 = -0.013480470f;
	t0 = t0 * t4 + 0.057477314f;
	t0 = t0 * t4 - 0.121239071f;
	t0 = t0 * t4 + 0.195635925f;
	t0 = t0 * t4 - 0.332994597f;
	t0 = t0 * t4 + 0.999995630f;
	t3 = t0 * t3;

	t3 = select(abs(y) > abs(x), 1.570796327f - t3, t3);
	t3 = select(x < 0, 3.141592654f - t3, t3);
	t3 = select(y < 0, -t3, t3);

	return t3.get(0);
}
#	endif

__m128 _mmWEAVER_floor_ps(__m128 x) noexcept
{
#	ifdef WEAVER_VECTORIZE_SSE4_1
	return _mm_floor_ps(x);
#	else
	// The following implementations are taken from:
	// http://dss.stephanierct.com/DevBlog/?p=8

	__m128i v0 = _mm_setzero_si128();
	__m128i v1 = _mm_cmpeq_epi32(v0, v0);
	__m128i ji = _mm_srli_epi32(v1, 25);
	__m128 j = _mm_castsi128_ps(_mm_slli_epi32(ji, 23)); //create vector 1.0f
	__m128i i = _mm_cvttps_epi32(x);
	__m128 fi = _mm_cvtepi32_ps(i);
	__m128 igx = _mm_cmpgt_ps(fi, x);
	j = _mm_and_ps(igx, j);
	return _mm_sub_ps(fi, j);
#	endif
}
#endif
