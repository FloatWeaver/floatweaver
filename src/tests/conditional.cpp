// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.

// FloatWeaver
#include <weaver/conditional.h>

#include "common.h"

template<typename T, int W>
void selectTest(
	const weaver::VectorScalar<bool, W>& t,
	const weaver::VectorScalar<T, W>& a,
	const weaver::VectorScalar<T, W>& b,
	const weaver::VectorScalar<T, W>& c)
{
	const auto selected = weaver::select(t, a, b);
	CHECK(weaver::all(selected == c));
}

TEST_CASE("Select floatN given boolN", "[operation][float]")
{
	WEAVER_SIMD_FLOATS;

	selectTest<float, 4>(true, 1.0f, 0.0f, 1.0f);
	selectTest<float, 8>(true, 1.0f, 0.0f, 1.0f);
	selectTest<float, 16>(true, 1.0f, 0.0f, 1.0f);

	selectTest<float, 4>(false, 1.0f, 0.0f, 0.0f);
	selectTest<float, 8>(false, 1.0f, 0.0f, 0.0f);
	selectTest<float, 16>(false, 1.0f, 0.0f, 0.0f);

	selectTest<float, 4>(float4{ 1 } / float4{ 0 } < 0, 1.0f, 0.0f, 0.0f);
	selectTest<float, 8>(float8{ 1 } / float8{ 0 } < 0, 1.0f, 0.0f, 0.0f);
	selectTest<float, 16>(float16{ 1 } / float16{ 0 } < 0, 1.0f, 0.0f, 0.0f);
}

TEST_CASE("Select intN given boolN", "[operation][int]")
{
	selectTest<int, 4>(true, 1, 0, 1);
	selectTest<int, 8>(true, 1, 0, 1);
	selectTest<int, 16>(true, 1, 0, 1);

	selectTest<int, 4>(false, 1, 0, 0);
	selectTest<int, 8>(false, 1, 0, 0);
	selectTest<int, 16>(false, 1, 0, 0);
}

template<typename T, int W>
void cswapTest(
	const weaver::VectorScalar<bool, W>& t,
	const weaver::VectorScalar<T, W>& a,
	const weaver::VectorScalar<T, W>& b)
{
    auto a_tmp = a;
    auto b_tmp = b;

	weaver::cswap(t, a_tmp, b_tmp);
	for (int i = 0; i < W; i++)
	{
		if (t[i])
		{
			CHECK(b[i] == a_tmp[i]);
			CHECK(a[i] == b_tmp[i]);
		}
		else
		{
			CHECK(a[i] == a_tmp[i]);
			CHECK(b[i] == b_tmp[i]);
		}
	}
}

TEST_CASE("Conditional Swap floatN given boolN", "[operation][float]")
{
	WEAVER_SIMD_FLOATS;

	cswapTest<float, 4>(true, 1.0f, 0.0f);
	cswapTest<float, 8>(true, 1.0f, 0.0f);
	cswapTest<float, 16>(true, 1.0f, 0.0f);
    
	cswapTest<float, 4>(false, 1.0f, 0.0f);
	cswapTest<float, 8>(false, 1.0f, 0.0f);
	cswapTest<float, 16>(false, 1.0f, 0.0f);

	cswapTest<float, 4>(float4{ 1 } / float4{ 0 } < 0, 1.0f, 0.0f);
	cswapTest<float, 8>(float8{ 1 } / float8{ 0 } < 0, 1.0f, 0.0f);
	cswapTest<float, 16>(float16{ 1 } / float16{ 0 } < 0, 1.0f, 0.0f);
}

template<typename T, int W>
void cnswapTest(
	const weaver::VectorScalar<bool, W>& t,
	const weaver::VectorScalar<T, W>& a,
	const weaver::VectorScalar<T, W>& b)
{
	auto a_tmp = a;
	auto b_tmp = b;

	weaver::cnswap(t, a_tmp, b_tmp);
	for (int i = 0; i < W; i++)
	{
		if (t[i])
		{
			CHECK(b[i] == a_tmp[i]);
			CHECK(a[i] == -b_tmp[i]);
		}
		else
		{
			CHECK(a[i] == a_tmp[i]);
			CHECK(b[i] == b_tmp[i]);
		}
	}
}

TEST_CASE("Conditional Negative Swap floatN given boolN", "[operation][float]")
{
	WEAVER_SIMD_FLOATS;

	cnswapTest<float, 4>(true, 1.0f, 0.0f);
	cnswapTest<float, 8>(true, 1.0f, 0.0f);
	cnswapTest<float, 16>(true, 1.0f, 0.0f);

	cnswapTest<float, 4>(false, 1.0f, 0.0f);
	cnswapTest<float, 8>(false, 1.0f, 0.0f);
	cnswapTest<float, 16>(false, 1.0f, 0.0f);

	cnswapTest<float, 4>(float4{ 1 } / float4{ 0 } < 0, 1.0f, 0.0f);
	cnswapTest<float, 8>(float8{ 1 } / float8{ 0 } < 0, 1.0f, 0.0f);
	cnswapTest<float, 16>(float16{ 1 } / float16{ 0 } < 0, 1.0f, 0.0f);
}
