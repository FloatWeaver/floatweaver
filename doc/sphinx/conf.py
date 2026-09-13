# Sphinx configuration for FloatWeaver.
#
# The Doxygen XML this depends on is produced by the `docs` CMake target
# (see cmake/config.cmake's WEAVER_BUILD_DOCS option), which points
# WEAVER_DOXYGEN_XML_DIR at its own build directory. Running sphinx-build by
# hand instead falls back to the location `doxygen doc/Doxyfile` produces
# when the project is built the way CLAUDE.md documents (`-B build`).

import os

project = "FloatWeaver"
copyright = "2023, Basil Fierz"
author = "Basil Fierz"

extensions = ["breathe"]

doxygen_xml_dir = os.environ.get(
	"WEAVER_DOXYGEN_XML_DIR",
	os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "build", "doxygen", "xml")),
)
breathe_projects = {"FloatWeaver": doxygen_xml_dir}
breathe_default_project = "FloatWeaver"

html_theme = "alabaster"
