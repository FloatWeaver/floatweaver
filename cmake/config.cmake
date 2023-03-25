
# Enable Visual Studio solution folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(${CMAKE_CXX_COMPILER_ID} MATCHES "Intel")
	set(WEAVER_COMPILER_ICC ON)
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
	set(WEAVER_COMPILER_GNU ON)
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
	set(WEAVER_COMPILER_CLANG ON)
elseif(MSVC)
	set(WEAVER_COMPILER_MSVC ON)
else()
	set(WEAVER_COMPILER_UNKNOWN ON)
endif()
message(STATUS "Detected compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(WEAVER_ADDRESS_SIZE "64")
else()
	set(WEAVER_ADDRESS_SIZE "32")
endif()
message(STATUS "Compiling for ${WEAVER_ADDRESS_SIZE}bit machine")

set(WEAVER_CXX_STANDARD "17" CACHE STRING "C++ standard")
set_property(CACHE WEAVER_CXX_STANDARD PROPERTY STRINGS "14" "17" "20" "23")
message(STATUS "Using C++${WEAVER_CXX_STANDARD}")

set(WEAVER_VECTORIZE "SSE 4.2" CACHE STRING "Vectorization instruction set")
set_property(CACHE WEAVER_VECTORIZE PROPERTY STRINGS "Generic" "SSE 4.2" "AVX" "AVX 2" "AVX 512" "NEON")
message(STATUS "Compiling for ${WEAVER_VECTORIZE}")

# Configure the compiler options a target
function(weaver_configure_target tgt)

	message(STATUS "Configure target: ${tgt}")

	set_target_properties(${tgt} PROPERTIES
		CXX_STANDARD ${WEAVER_CXX_STANDARD}
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
	)
	#target_compile_features(weaver PUBLIC cxx_std_${WEAVER_CXX_STANDARD})

	if(WEAVER_VECTORIZE STREQUAL "SSE 4.2")
		target_compile_definitions(${tgt} PUBLIC WEAVER_VECTORIZE_SSE4_2)
	elseif(WEAVER_VECTORIZE STREQUAL "AVX")
		target_compile_definitions(${tgt} PUBLIC WEAVER_VECTORIZE_AVX)
	elseif(WEAVER_VECTORIZE STREQUAL "AVX 2")
		target_compile_definitions(${tgt} PUBLIC WEAVER_VECTORIZE_AVX2)
	elseif(WEAVER_VECTORIZE STREQUAL "AVX 512")
		target_compile_definitions(${tgt} PUBLIC WEAVER_VECTORIZE_AVX512)
	elseif(WEAVER_VECTORIZE STREQUAL "NEON")
		target_compile_definitions(${tgt} PUBLIC WEAVER_VECTORIZE_NEON)
	endif()
	
	if(WEAVER_COMPILER_MSVC)
		# Disable buffer security checks
		target_compile_options(${tgt} PRIVATE "$<$<CONFIG:RELEASE>:/GS->")
		# * Enable level 4 warnings
		target_compile_options(${tgt} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:/W4>")
		# * Exceptions
		target_compile_options(${tgt} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:/EHsc>")
		# * RTTI
		target_compile_options(${tgt} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:/GR>")
		# * Don't be permissive
		if (MSVC_VERSION GREATER 1900)
			target_compile_options(${tgt} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:/permissive->")
		endif()

		# Make AVX available
		if(WEAVER_VECTORIZE_AVX512)
			target_compile_options(${tgt} PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:/arch:AVX512>")
		elseif(WEAVER_VECTORIZE_AVX2)
			target_compile_options(${tgt} PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:/arch:AVX2>")
		elseif(WEAVER_VECTORIZE_AVX)
			target_compile_options(${tgt} PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:/arch:AVX>")
		elseif(WEAVER_VECTORIZE_SSE4_2 AND WEAVER_ADDRESS_SIZE EQUAL "32")
			# All x64 bit machine come with SSE2, thus it's defined as default
			target_compile_options(${tgt} PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:/arch:SSE2>")
		elseif(WEAVER_VECTORIZE_NEON)
			target_compile_options(${tgt} PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:/arch:VFPv4>")
		endif()

	endif(WEAVER_COMPILER_MSVC)

endfunction()

# Add files to target
# 'files' are supposed to be defined relative to where the target is defined
function(weaver_target_sources tgt prefix)

	if(NOT "${prefix}" STREQUAL "")
		string(REPLACE "." "\\." prefix ${prefix})
	endif()

	# Configure the VS project file filters
	foreach(file ${ARGN})
		get_filename_component(dir_ "${file}" DIRECTORY)
		if(NOT "${dir_}" STREQUAL "" AND NOT "${prefix}" STREQUAL "")
			# Remove the prefix and an optional '/'
			string(REGEX REPLACE "^${prefix}/?" "" dir_ ${dir_})
			# Replace the path separator with filter separators
			if("${dir_}" STREQUAL "")
				set(dir_ "\\\\")
			else()
				string(REPLACE "/" "\\\\" dir_ ${dir_})
			endif()
		else()
			set(dir_ "\\\\")
		endif()
		source_group(${dir_} FILES ${file})
	endforeach()

	# Add the files to the target
	target_sources(${tgt} PRIVATE ${ARGN})
endfunction()
