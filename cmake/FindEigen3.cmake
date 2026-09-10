# - Try to find Eigen3, without relying on an Eigen3Config.cmake being installed.
#
# Used as a fallback when Eigen3 isn't available as a package (Conan, system
# install): point EIGEN3_INCLUDE_DIR at a source checkout (e.g. the
# external/eigen git submodule) before calling find_package(Eigen3 MODULE),
# and this module will pick up the version from the headers directly.
#
# Once done this will define
#
#  EIGEN3_FOUND        - Eigen3 was found
#  EIGEN3_INCLUDE_DIR   - the Eigen3 include directory
#  EIGEN3_VERSION       - Eigen3 version
#  Eigen3::Eigen        - imported target, usable like the Conan/CMake package
#
# Reads hints about search locations from the EIGEN3_ROOT / EIGEN3_ROOT_DIR
# environment variables when EIGEN3_INCLUDE_DIR isn't already set.

if(NOT EIGEN3_INCLUDE_DIR)
	find_path(EIGEN3_INCLUDE_DIR NAMES signature_of_eigen3_matrix_library
		HINTS
		ENV EIGEN3_ROOT
		ENV EIGEN3_ROOT_DIR
		PATHS
		${CMAKE_INSTALL_PREFIX}/include
		PATH_SUFFIXES eigen3 eigen
	)
endif()

if(EIGEN3_INCLUDE_DIR AND EXISTS "${EIGEN3_INCLUDE_DIR}/Eigen/src/Core/util/Macros.h")
	file(READ "${EIGEN3_INCLUDE_DIR}/Eigen/src/Core/util/Macros.h" _eigen3_version_header)

	string(REGEX MATCH "define[ \t]+EIGEN_WORLD_VERSION[ \t]+([0-9]+)" _ "${_eigen3_version_header}")
	set(_eigen3_world_version "${CMAKE_MATCH_1}")
	string(REGEX MATCH "define[ \t]+EIGEN_MAJOR_VERSION[ \t]+([0-9]+)" _ "${_eigen3_version_header}")
	set(_eigen3_major_version "${CMAKE_MATCH_1}")
	string(REGEX MATCH "define[ \t]+EIGEN_MINOR_VERSION[ \t]+([0-9]+)" _ "${_eigen3_version_header}")
	set(_eigen3_minor_version "${CMAKE_MATCH_1}")

	set(EIGEN3_VERSION "${_eigen3_world_version}.${_eigen3_major_version}.${_eigen3_minor_version}")
	unset(_eigen3_version_header)
	unset(_eigen3_world_version)
	unset(_eigen3_major_version)
	unset(_eigen3_minor_version)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Eigen3
	REQUIRED_VARS EIGEN3_INCLUDE_DIR
	VERSION_VAR EIGEN3_VERSION
)

if(EIGEN3_FOUND AND NOT TARGET Eigen3::Eigen)
	add_library(Eigen3::Eigen INTERFACE IMPORTED)
	target_include_directories(Eigen3::Eigen INTERFACE "${EIGEN3_INCLUDE_DIR}")
endif()

mark_as_advanced(EIGEN3_INCLUDE_DIR)
