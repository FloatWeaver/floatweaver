# Sphinx configuration for FloatWeaver.
#
# The Doxygen XML this depends on is produced by the `docs` CMake target
# (see cmake/config.cmake's WEAVER_BUILD_DOCS option), which points
# WEAVER_DOXYGEN_XML_DIR at its own build directory. Running sphinx-build by
# hand instead falls back to the location `doxygen doc/Doxyfile` produces
# when the project is built the way CLAUDE.md documents (`-B build`).

import os

from docutils.parsers.rst import Directive
from docutils import nodes

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


class WeaverAsmDirective(Directive):
	"""``.. weaver-asm:: <symbol>`` -- embeds the disassembly WEAVER_BUILD_DOCS's
	extract_asm.py extracted for that symbol (see doc/tools/extract_asm.py and
	the weaver_doc_asm target in the root CMakeLists.txt). Mirrors how
	WEAVER_DOXYGEN_XML_DIR feeds Breathe: same env-var handoff from CMake,
	same "missing -> warning, -W turns it into a build failure" contract as
	literalinclude/doxygenfunction elsewhere in these docs.
	"""

	required_arguments = 1
	has_content = False

	def run(self):
		symbol = self.arguments[0]
		asm_dir = os.environ.get("WEAVER_ASM_DIR")
		if not asm_dir:
			self.state_machine.reporter.warning(
				"weaver-asm: WEAVER_ASM_DIR is not set -- build docs via the "
				"CMake 'docs' target (WEAVER_BUILD_DOCS=ON) to generate "
				"assembly listings.",
				line=self.lineno,
			)
			return []

		path = os.path.join(asm_dir, symbol + ".txt")
		if not os.path.isfile(path):
			self.state_machine.reporter.warning(
				f"weaver-asm: no extracted assembly found for '{symbol}' at {path}",
				line=self.lineno,
			)
			return []

		with open(path) as f:
			text = f.read().rstrip("\n")

		node = nodes.literal_block(text, text)
		node["language"] = "nasm"
		return [node]


def setup(app):
	app.add_directive("weaver-asm", WeaverAsmDirective)
