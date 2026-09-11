// Copyright 2023 Basil Fierz
// 
// Use of this source code is governed by an MIT - style
// license that can be found in the LICENSE file or at
// https ://opensource.org/licenses/MIT.


// Check library configuration
#if defined __ICC || defined __ICL
#	define WEAVER_COMPILER_ICC
#	if (__INTEL_COMPILER < 1700)
#		warning "Minimum supported version is ICC 17. Good luck."
#	endif
#elif defined _MSC_VER && !defined __clang__
// Microsoft compiler versions
// MSVC++ 10.0  _MSC_VER == 1600 (Visual Studio 2010)
// MSVC++ 11.0  _MSC_VER == 1700 (Visual Studio 2012)
// MSVC++ 12.0  _MSC_VER == 1800 (Visual Studio 2013)
// MSVC++ 14.0  _MSC_VER == 1900 (Visual Studio 2015)
// MSVC++ 14.10 _MSC_VER == 1910 (Visual Studio 2017)
// MSVC++ 14.11 _MSC_VER == 1911 (Visual Studio 2017 Update 3)
// MSVC++ 14.12 _MSC_VER == 1912 (Visual Studio 2017 Update 5)
// MSVC++ 14.13 _MSC_VER == 1913 (Visual Studio 2017 Update 6)
// MSVC++ 14.14 _MSC_VER == 1914 (Visual Studio 2017 Update 7)
// MSVC++ 14.15 _MSC_VER == 1915 (Visual Studio 2017 Update 8)
// MSVC++ 14.16 _MSC_VER == 1916 (Visual Studio 2017 Update 9)
// MSVC++ 14.20 _MSC_VER == 1920 (Visual Studio 2019)
// MSVC++ 14.30 _MSC_VER == 1930 (Visual Studio 2022)
#	define WEAVER_COMPILER_MSVC
#	if (_MSC_VER < 1920)
#		warning "Minimum supported version is MSVC 2019. Good luck."
#	endif
#elif defined __clang__
#	define WEAVER_COMPILER_CLANG
#	if (__clang_major__ < 3) || (__clang_major__ == 3 && __clang_minor__ < 5)
#		warning "Minimum supported version is Clang 3.5. Good luck."
#	endif
#elif defined __GNUC__
#	define WEAVER_COMPILER_GNU
#	if (__GNUC__ < 5)
#		warning "Minimum supported version is GCC 5. Good luck."
#	endif
#endif

// Identify CPU instruction set
#if defined WEAVER_COMPILER_ICC

// The Intel compiler only supports Intel64 and x86 instruction sets
#	if defined _M_X64 || defined __x86_64 || defined __x86_64__
#		define WEAVER_ARCH_X64
#	else
#		define WEAVER_ARCH_X86
#	endif

#elif defined WEAVER_COMPILER_MSVC
#	if defined _M_IX86
#		define WEAVER_ARCH_X86
#	endif

#	if defined _M_X64
#		define WEAVER_ARCH_X64
#	endif

#	if defined _M_ARM
#		define WEAVER_ARCH_ARM
#	endif

#	if defined _M_ARM64
#		define WEAVER_ARCH_ARM64
#	endif

#elif defined WEAVER_COMPILER_GNU || defined WEAVER_COMPILER_CLANG
#	if defined __i386__ || defined __i686__
#		define WEAVER_ARCH_X86
#	endif

#	if defined __x86_64
#		define WEAVER_ARCH_X64
#	endif

#	if defined __arm__
#		define WEAVER_ARCH_ARM
#	endif

#	if defined __aarch64__
#		define WEAVER_ARCH_ARM64
#	endif

#	if defined __EMSCRIPTEN__
#		define WEAVER_ARCH_WEBASM
#	endif

#endif

#if (defined(WEAVER_ARCH_X86) || defined(WEAVER_ARCH_X64))

#	ifdef WEAVER_VECTORIZE_AVX512
#		ifndef WEAVER_VECTORIZE_AVX2
#			define WEAVER_VECTORIZE_AVX2
#		endif
#	endif
#	ifdef WEAVER_VECTORIZE_AVX2
#		ifndef WEAVER_VECTORIZE_AVX
#			define WEAVER_VECTORIZE_AVX
#		endif
#	endif
#	ifdef WEAVER_VECTORIZE_AVX
#		ifndef WEAVER_VECTORIZE_SSE4_2
#			define WEAVER_VECTORIZE_SSE4_2
#		endif
#	endif

#	ifdef WEAVER_VECTORIZE_SSE4_2
#		ifndef WEAVER_VECTORIZE_SSE4_1
#			define WEAVER_VECTORIZE_SSE4_1
#		endif
#	endif
#	ifdef WEAVER_VECTORIZE_SSE4_1
#		ifndef WEAVER_VECTORIZE_SSSE3
#			define WEAVER_VECTORIZE_SSSE3
#		endif
#	endif
#	ifdef WEAVER_VECTORIZE_SSSE3
#		ifndef WEAVER_VECTORIZE_SSE3
#			define WEAVER_VECTORIZE_SSE3
#		endif
#	endif
#	ifdef WEAVER_VECTORIZE_SSE3
#		ifndef WEAVER_VECTORIZE_SSE2
#			define WEAVER_VECTORIZE_SSE2
#		endif
#	endif
#	ifdef WEAVER_VECTORIZE_SSE2
#		ifndef WEAVER_VECTORIZE_SSE
#			define WEAVER_VECTORIZE_SSE
#		endif
#	endif
#	if defined WEAVER_VECTORIZE_AVX
extern "C"
{
#		include <immintrin.h>
}
#	elif defined(WEAVER_VECTORIZE_SSE)
extern "C"
{
#		ifdef WEAVER_COMPILER_MSVC
#			include <intrin.h>
#		else
#			include <x86intrin.h>
#		endif
}
#	endif

#elif (defined(WEAVER_ARCH_ARM) || defined(WEAVER_ARCH_ARM64)) && defined WEAVER_VECTORIZE_NEON
#	include <arm_neon.h>
#elif (defined(WEAVER_ARCH_WEBASM))
#	ifdef WEAVER_VECTORIZE_SSE4_2
#		ifndef WEAVER_VECTORIZE_SSE4_1
#			define WEAVER_VECTORIZE_SSE4_1
#		endif
#	endif
#	ifdef WEAVER_VECTORIZE_SSE4_1
#		ifndef WEAVER_VECTORIZE_SSSE3
#			define WEAVER_VECTORIZE_SSSE3
#		endif
#	endif
#	ifdef WEAVER_VECTORIZE_SSSE3
#		ifndef WEAVER_VECTORIZE_SSE3
#			define WEAVER_VECTORIZE_SSE3
#		endif
#	endif
#	ifdef WEAVER_VECTORIZE_SSE3
#		ifndef WEAVER_VECTORIZE_SSE2
#			define WEAVER_VECTORIZE_SSE2
#		endif
#	endif
#	ifdef WEAVER_VECTORIZE_SSE2
#		ifndef WEAVER_VECTORIZE_SSE
#			define WEAVER_VECTORIZE_SSE
#		endif
#	endif
#	ifdef WEAVER_VECTORIZE_SSE
#		include <nmmintrin.h>
#	endif
#endif

#if defined WEAVER_COMPILER_MSVC

#	define WEAVER_STRONG_INLINE __forceinline

#elif defined(WEAVER_COMPILER_GNU) || defined(WEAVER_COMPILER_CLANG)

#	define WEAVER_STRONG_INLINE inline

#endif

#if defined(WEAVER_COMPILER_MSVC)
#	if defined(WEAVER_CHECK_CORE_GUIDELINES) && (_MSC_VER >= 1910)
#		include <CppCoreCheck/Warnings.h>
#		define WEAVER_BEGIN_EXTERNAL_HEADERS \
			__pragma(warning(push, 1)) \
			__pragma(warning(disable: ALL_CPPCORECHECK_WARNINGS))
#	else

#		define WEAVER_BEGIN_EXTERNAL_HEADERS \
			__pragma(warning(push, 1))
#	endif
#	define WEAVER_END_EXTERNAL_HEADERS __pragma(warning(pop))
#elif defined(WEAVER_COMPILER_CLANG)
#	define WEAVER_BEGIN_EXTERNAL_HEADERS \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Weverything\"")
#	define WEAVER_END_EXTERNAL_HEADERS \
    _Pragma("clang diagnostic pop")
#else
#	define WEAVER_BEGIN_EXTERNAL_HEADERS
#	define WEAVER_END_EXTERNAL_HEADERS
#endif
