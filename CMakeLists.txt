cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0048 NEW)

project(FloatWeaver VERSION 0.1.0.0 LANGUAGES CXX)

# Eigen3: use a Conan/system package if one is available; otherwise fall back
# to the "external/eigen" git submodule, so the project also builds with
# nothing more than `git submodule update --init`.
find_package(Eigen3 3.4 QUIET)
if(NOT TARGET Eigen3::Eigen)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/eigen/CMakeLists.txt")
		message(STATUS "Eigen3 package not found, building from external/eigen submodule")
		set(EIGEN3_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/eigen")
		list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
		find_package(Eigen3 3.4 MODULE REQUIRED)
	else()
		message(FATAL_ERROR
			"Eigen3 was not found, and external/eigen is not checked out. Either "
			"install Eigen3 (e.g. 'conan install .') or run "
			"'git submodule update --init external/eigen'.")
	endif()
endif()

include(cmake/config.cmake)

add_library(weaver STATIC "")
weaver_configure_target(weaver)

target_include_directories(weaver PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(weaver PUBLIC Eigen3::Eigen)

set(INC
	include/weaver/config.h
	include/weaver/common.h
	include/weaver/conditional.h
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
	# Catch2: use a Conan/system package if one is available; otherwise fall
	# back to the "external/catch2" git submodule.
	find_package(Catch2 3 QUIET)
	if(NOT TARGET Catch2::Catch2WithMain)
		if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/catch2/CMakeLists.txt")
			message(STATUS "Catch2 package not found, building from external/catch2 submodule")
			add_subdirectory(external/catch2 EXCLUDE_FROM_ALL)
			list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/catch2/extras")
		else()
			message(FATAL_ERROR
				"Catch2 was not found, and external/catch2 is not checked out. Either "
				"install Catch2 (e.g. 'conan install .') or run "
				"'git submodule update --init external/catch2'.")
		endif()
	endif()

	add_executable(weaver_tests "")
	set(WEAVER_TEST_SRC
		src/tests/bool.cpp
		src/tests/float.cpp
		src/tests/int.cpp

		src/tests/common.h
		src/tests/common_bool.h
		src/tests/common_float.h
		src/tests/common_int.h

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

	# Documentation snippets double as tests: every //! [tag] block in
	# doc/snippets is referenced verbatim from the Sphinx/Doxygen docs, so
	# compiling and running it here is what keeps an example from silently
	# drifting out of sync with the API it documents.
	set(WEAVER_DOC_SNIPPET_SRC
		doc/snippets/float8_snippets.cpp
		doc/snippets/float8_asm.cpp
	)
	weaver_target_sources(weaver_tests "doc" ${WEAVER_DOC_SNIPPET_SRC})

	set_target_properties(weaver_tests PROPERTIES
		CXX_STANDARD ${WEAVER_CXX_STANDARD}
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
	)
	target_link_libraries(weaver_tests weaver Catch2::Catch2WithMain)

	# catch_discover_tests() needs to run the freshly-built test executable to
	# enumerate its test cases. When cross-compiling that only works if CMake
	# knows how to execute foreign-architecture binaries, i.e. when
	# CMAKE_CROSSCOMPILING_EMULATOR is set (e.g. "qemu-arm -L ..." for a NEON
	# build cross-compiled for ARM/AArch64). Plain Emscripten builds don't set
	# it -- CI drives those through `node weaver_tests.js` directly instead.
	if(NOT CMAKE_CROSSCOMPILING OR (CMAKE_CROSSCOMPILING_EMULATOR AND NOT EMSCRIPTEN))
		include(CTest)
		include(Catch)
		catch_discover_tests(weaver_tests)
	endif()
endif()

# Doxygen -> Breathe -> Sphinx documentation, off by default so a plain
# `cmake --build` never needs Doxygen/Python/Sphinx installed.
if(WEAVER_BUILD_DOCS)
	find_package(Doxygen REQUIRED)
	find_package(Python3 COMPONENTS Interpreter REQUIRED)
	find_program(SPHINX_EXECUTABLE sphinx-build REQUIRED)
	# objdump ships with binutils, i.e. whatever gcc/clang toolchain is
	# already on hand on Linux/macOS -- WEAVER_BUILD_DOCS is scoped to that,
	# not to every CI backend/compiler this project builds against. The
	# extracted instructions naturally differ by compiler/OS; that's fine,
	# it's showing whatever the configured toolchain actually generated.
	find_program(WEAVER_OBJDUMP objdump REQUIRED)

	set(WEAVER_DOXYGEN_XML_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen/xml")
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in"
		"${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
		@ONLY
	)

	# doc/snippets/*_asm.cpp functions are compiled here in isolation (their
	# own OBJECT library, no LTO, no other call sites in the program) so
	# doc/tools/extract_asm.py can disassemble the actual vector instructions
	# they generate for the how-to guides, rather than just asserting
	# FloatWeaver is vectorized.
	add_library(weaver_doc_asm OBJECT doc/snippets/float8_asm.cpp)
	weaver_configure_target(weaver_doc_asm)
	target_link_libraries(weaver_doc_asm PRIVATE Eigen3::Eigen Catch2::Catch2WithMain)
	target_include_directories(weaver_doc_asm PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

	set(WEAVER_ASM_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc/asm")
	set(WEAVER_ASM_ADD_FLOAT8 "${WEAVER_ASM_DIR}/add_float8.txt")
	add_custom_command(
		OUTPUT "${WEAVER_ASM_ADD_FLOAT8}"
		COMMAND "${CMAKE_COMMAND}" -E make_directory "${WEAVER_ASM_DIR}"
		COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/doc/tools/extract_asm.py"
			--objdump "${WEAVER_OBJDUMP}"
			--object "$<TARGET_OBJECTS:weaver_doc_asm>"
			--symbol add_float8
			--output "${WEAVER_ASM_ADD_FLOAT8}"
		DEPENDS weaver_doc_asm "$<TARGET_OBJECTS:weaver_doc_asm>"
		COMMENT "Extracting assembly for add_float8"
		VERBATIM
	)
	add_custom_target(weaver_doc_asm_extract DEPENDS "${WEAVER_ASM_ADD_FLOAT8}")

	add_custom_target(docs
		COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
		COMMAND "${CMAKE_COMMAND}" -E env
			"WEAVER_DOXYGEN_XML_DIR=${WEAVER_DOXYGEN_XML_DIR}"
			"WEAVER_ASM_DIR=${WEAVER_ASM_DIR}"
			"${SPHINX_EXECUTABLE}" -b html -W
				"${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx"
				"${CMAKE_CURRENT_BINARY_DIR}/sphinx"
		DEPENDS weaver_doc_asm_extract
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		COMMENT "Building Doxygen XML and Sphinx HTML documentation"
		VERBATIM
	)
endif()
