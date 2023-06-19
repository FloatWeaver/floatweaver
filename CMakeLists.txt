cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0048 NEW)

project(FloatWeaver VERSION 0.1.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/external/eigen/cmake")
set(EIGEN3_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/external/eigen")
find_package(Eigen3 REQUIRED)

include(cmake/config.cmake)

add_library(weaver STATIC "")
weaver_configure_target(weaver)

target_include_directories(weaver PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(weaver PUBLIC Eigen3::Eigen)

set(INC
	include/weaver/config.h
	include/weaver/common.h
	include/weaver/conditional.h
	include/weaver/vectorscalar.h
	include/weaver/vector_types.h

	include/weaver/eigen.h

	include/weaver/bool4_ref.h
	include/weaver/bool8_ref.h
	include/weaver/bool16_ref.h
	include/weaver/float4_ref.h
	include/weaver/float8_ref.h
	include/weaver/float16_ref.h
	include/weaver/int4_ref.h
	include/weaver/int8_ref.h
	include/weaver/int16_ref.h
	include/weaver/common_ref.h

	include/weaver/bool4_sse.h
	include/weaver/bool8_sse.h
	include/weaver/bool16_sse.h
	include/weaver/float4_sse.h
	include/weaver/float8_sse.h
	include/weaver/float16_sse.h
	include/weaver/int4_sse.h
	include/weaver/int8_sse.h
	include/weaver/int16_sse.h

	include/weaver/bool8_avx.h
	include/weaver/bool16_avx.h
	include/weaver/float8_avx.h
	include/weaver/float16_avx.h
	include/weaver/int8_avx.h
	include/weaver/int16_avx.h

	include/weaver/bool16_avx512.h
	include/weaver/float16_avx512.h
	include/weaver/int16_avx512.h

	include/weaver/bool4_neon.h
	include/weaver/bool8_neon.h
	include/weaver/bool16_neon.h
	include/weaver/float4_neon.h
	include/weaver/float8_neon.h
	include/weaver/float16_neon.h
	include/weaver/int4_neon.h
	include/weaver/int8_neon.h
	include/weaver/int16_neon.h

	include/weaver/intrinsics_avx512.h
	include/weaver/intrinsics_avx.h
	include/weaver/intrinsics_sse.h
	include/weaver/intrinsics_neon.h

	include/weaver/memory.h
	include/weaver/memory_avx.h
	include/weaver/memory_sse.h
	include/weaver/memory_neon.h
)
set(SRC
	src/detail/avx_mathfun.h
	src/detail/avx512_mathfun.h
	src/detail/sse_mathfun.h
	src/detail/neon_mathfun.h
	src/intrinsics_avx.cpp
	src/intrinsics_avx512.cpp
	src/intrinsics_neon.cpp
	src/intrinsics_sse.cpp
)
weaver_target_sources(weaver "include" ${INC})
weaver_target_sources(weaver "." ${SRC})

# Only build tests when building as standalone project
if("^${CMAKE_SOURCE_DIR}$" STREQUAL "^${PROJECT_SOURCE_DIR}$")
	add_subdirectory(external/catch2 EXCLUDE_FROM_ALL)

	add_executable(weaver_tests "")
	set(WEAVER_TEST_SRC
		src/tests/bool.cpp
		src/tests/float.cpp
		src/tests/int.cpp

		src/tests/common.h

		src/tests/conditional.cpp
		src/tests/eigen_alignedbox.cpp
		src/tests/gather.cpp
		src/tests/interleave.cpp
		src/tests/load.cpp
		src/tests/scatter.cpp
		src/tests/simd.cpp
		src/tests/store.cpp
	)
	weaver_target_sources(weaver_tests "src" ${WEAVER_TEST_SRC})

	set_target_properties(weaver_tests PROPERTIES
		CXX_STANDARD ${WEAVER_CXX_STANDARD}
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
	)
	target_link_libraries(weaver_tests weaver Catch2::Catch2WithMain)
endif()
