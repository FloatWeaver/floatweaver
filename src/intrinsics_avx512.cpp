// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#include <weaver/intrinsics_avx512.h>

#if defined WEAVER_VECTORIZE_AVX512
WEAVER_BEGIN_EXTERNAL_HEADERS
#	if defined WEAVER_VECTORIZE_AVX2 && !defined __AVX2__
#		define __AVX2__
#	endif
#	include "detail/avx512_mathfun.h"
WEAVER_END_EXTERNAL_HEADERS

// FloatWeaver
#	include <weaver/bool16_avx512.h>
#	include <weaver/float16_avx512.h>

namespace weaver {
#	if !defined(WEAVER_COMPILER_MSVC)
	__m512 _mm512_sin_ps(__m512 v)
	{
		return sin512_ps(v);
	}

	__m512 _mm512_cos_ps(__m512 v)
	{
		return cos512_ps(v);
	}

	__m512 _mm512_log_ps(__m512 v)
	{
		return log512_ps(v);
	}

	__m512 _mm512_exp_ps(__m512 v)
	{
		return exp512_ps(v);
	}

	// Handbook of Mathematical Functions
	// M. Abramowitz and I.A. Stegun, Ed.
	__m512 _mm512_acos_ps(__m512 v)
	{
		float16 x{ v };

		// Absolute error <= 6.7e-5
		float16 negate = select(x < 0, float16{ 1 }, float16{ 0 });

		x = x.abs();

		float16 ret = -0.0187293f;
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
	__m512 _mm512_asin_ps(__m512 v)
	{
		float16 x{ v };

		float16 negate = select(x < 0, float16{ 1 }, float16{ 0 });

		x = abs(x);
		float16 ret = -0.0187293f;
		ret *= x;
		ret += 0.0742610f;
		ret *= x;
		ret -= 0.2121144f;
		ret *= x;
		ret += 1.5707288f;
		ret = 3.14159265358979f * 0.5f - sqrt(1.0f - x) * ret;
		return (ret - 2.0f * negate * ret).get(0);
	}

	__m512 _mm512_atan2_ps(__m512 in_y, __m512 in_x)
	{
		float16 t0, t1, t3, t4;

		float16 x{ in_x };
		float16 y{ in_y };

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

	__m512 _mm512_pow_ps(__m512 x, __m512 y)
	{
		return _mm512_exp_ps(_mm512_mul_ps(_mm512_log_ps(x), y));
	}
#	endif
}
#endif // WEAVER_VECTORIZE_AVX
