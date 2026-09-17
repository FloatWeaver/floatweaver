#!/usr/bin/env python3
"""Extract one function's disassembly from an object file for the docs.

Used by the WEAVER_BUILD_DOCS CMake target: each doc/snippets/*_asm.cpp file
defines a small, non-inlined, extern-linkage function whose sole purpose is
to be compiled in isolation and disassembled here, so the how-to guides can
show the real vector instructions FloatWeaver generates instead of just
asserting that it's fast. See doc/snippets/float8_asm.cpp for why the
function takes its operands as parameters rather than literals.

Output varies by compiler/OS -- that's expected and left as-is, this only
extracts whatever the configured toolchain actually produced.
"""

import argparse
import re
import subprocess
import sys

# Matches the label objdump -C (demangle) prints before a function's
# disassembly, e.g. "0000000000000130 <add_float8(weaver::VectorScalar<float, 8>, ...)>:"
# Greedy `.+` (not `[^>]+`) because a demangled C++ name can itself contain
# ">" characters, e.g. "add_float8(weaver::VectorScalar<float, 8>, ...)".
_LABEL_RE = re.compile(r"^[0-9a-f]+\s+<(?P<name>.+)>:\s*$")

# Matches one instruction line, e.g. " 130:\tendbr64" or " 134:\tvaddps ymm0,ymm1,ymm0"
_INSN_RE = re.compile(r"^\s*[0-9a-f]+:\t(?P<insn>.*)$")


def extract(objdump_output: str, symbol: str) -> list[str]:
	lines = objdump_output.splitlines()
	prefix = symbol + "("
	insns = []
	in_target = False
	for line in lines:
		label = _LABEL_RE.match(line)
		if label:
			in_target = label.group("name").startswith(prefix)
			continue
		if not in_target:
			continue
		insn = _INSN_RE.match(line)
		if not insn:
			# Blank line / section header: the function's body has ended.
			break
		text = insn.group("insn").split("#", 1)[0].rstrip()
		if text:
			insns.append(text)
	return insns


def main() -> int:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument("--objdump", required=True, help="path to the objdump executable")
	parser.add_argument("--object", required=True, help="object file to disassemble")
	parser.add_argument("--symbol", required=True, help="plain (undecorated) function name to extract")
	parser.add_argument("--output", required=True, help="where to write the extracted instructions")
	args = parser.parse_args()

	result = subprocess.run(
		[args.objdump, "-d", "--no-show-raw-insn", "-Mintel", "-C", args.object],
		capture_output=True,
		text=True,
	)
	if result.returncode != 0:
		print(f"error: {args.objdump} failed:\n{result.stderr}", file=sys.stderr)
		return 1

	insns = extract(result.stdout, args.symbol)
	if not insns:
		print(
			f"error: no disassembly found for symbol '{args.symbol}' in {args.object} "
			"-- was it inlined away, or renamed?",
			file=sys.stderr,
		)
		return 1

	with open(args.output, "w") as f:
		f.write("\n".join(insns) + "\n")
	return 0


if __name__ == "__main__":
	sys.exit(main())
