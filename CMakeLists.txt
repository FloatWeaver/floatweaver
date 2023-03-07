cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0048 NEW)

project(FloatWeaver VERSION 0.1.0.0 LANGUAGES CXX)

set(WEAVER_CXX_STANDARD "17" CACHE STRING "C++ standard")
set_property(CACHE WEAVER_CXX_STANDARD PROPERTY STRINGS "17" "20" "23")
message(STATUS "Using C++${WEAVER_CXX_STANDARD}")

add_library(weaver STATIC "")
target_compile_features(weaver PUBLIC cxx_std_${CPPPAL_CXX_STANDARD})
target_include_directories(weaver PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(weaver PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/src/intrinsics_avx.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/intrinsics_avx512.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/intrinsics_neon.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/intrinsics_sse.h"
)
