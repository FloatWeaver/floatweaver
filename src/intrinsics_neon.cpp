// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

#include <weaver/intrinsics_neon.h>

#if defined(WEAVER_VECTORIZE_NEON)
WEAVER_BEGIN_EXTERNAL_HEADERS
#	include "detail/neon_mathfun.h"
WEAVER_END_EXTERNAL_HEADERS

// FloatWeaver
#	include <weaver/vector_types.h>

namespace weaver {

	float32x4_t vsinq_f32(float32x4_t v)
	{
		return sin_ps(v);
	}

	float32x4_t vcosq_f32(float32x4_t v)
	{
		return cos_ps(v);
	}

	float32x4_t vlogq_f32(float32x4_t v)
	{
		return log_ps(v);
	}

	float32x4_t vexpq_f32(float32x4_t v)
	{
		return exp_ps(v);
	}

	float32x4_t vpowq_f32(float32x4_t x, float32x4_t y)
	{
		return vexpq_f32(vmulq_f32(vlogq_f32(x), y));
	}

	// Handbook of Mathematical Functions
	// M. Abramowitz and I.A. Stegun, Ed.
	float32x4_t vacosq_f32(float32x4_t v)
	{
		using float4 = VectorScalar<float, 4>;

		float4 x{ v };

		// Absolute error <= 6.7e-5
		float4 negate = select(x < 0, float4{ 1 }, float4{ 0 });

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
	float32x4_t vasinq_f32(float32x4_t v)
	{
		using float4 = VectorScalar<float, 4>;

		float4 x{ v };

		float4 negate = select(x < 0, float4{ 1 }, float4{ 0 });

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

	float32x4_t vatan2q_f32(float32x4_t in_y, float32x4_t in_x)
	{
		using float4 = VectorScalar<float, 4>;

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

	float32x4_t weaver_vsqrtq_f32(float32x4_t x)
	{
		const float4 v{ x };
		return select(v == 0.0f, 0.0f, v * rsqrt(v)).get(0);
	}
}
#endif
