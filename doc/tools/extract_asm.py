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

# The CET landing pad gcc/clang emit as the first instruction of every
# function on Linux (-fcf-protection, on by default) -- carries no
# information about what the function computes, so it's dropped rather than
# shown as if it were part of the example.
_CET_LANDING_PAD_RE = re.compile(r"^endbr(32|64)$")

# Inter-function alignment padding: objdump keeps printing instruction-shaped
# lines after a function's last `ret`/`jmp` right up to the next label,
# because the assembler pads the gap between functions with nops so the next
# symbol starts on an aligned address. None of this executes as part of the
# function -- it's a side effect of *where* the linker put the next symbol,
# not of what this one does -- so it's stripped from the end of the listing.
# Safe to do unconditionally: real function bodies don't end in a run of nops,
# so this only ever matches genuine trailing padding.
_PADDING_RE = re.compile(r"^(data16\s+)*(cs\s+)?(nop\b.*|xchg\s+ax,ax)$")


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

	if insns and _CET_LANDING_PAD_RE.match(insns[0]):
		insns.pop(0)
	while insns and _PADDING_RE.match(insns[-1]):
		insns.pop()

	return insns


def main() -> int:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument("--objdump", required=True, help="path to the objdump executable")
	parser.add_argument(
		"--object",
		required=True,
		help="';'-joined object file(s) to search -- CMake's $<TARGET_OBJECTS:...> "
		"is passed straight through as a single argument with its list "
		"separator intact, not split into multiple argv entries. "
		"weaver_doc_asm is one shared OBJECT library, so this expands to one "
		"file per doc/snippets/*_asm.cpp; only one of them will actually "
		"define --symbol",
	)
	parser.add_argument("--symbol", required=True, help="plain (undecorated) function name to extract")
	parser.add_argument("--output", required=True, help="where to write the extracted instructions")
	parser.add_argument(
		"--params",
		default="",
		help="optional 'reg = param, reg = param, ...' note on where the function's "
		"arguments arrive, written as a leading comment. Not derived from the "
		"disassembly -- the System V AMD64 calling convention (what "
		"WEAVER_BUILD_DOCS always targets: gcc/clang on Linux/macOS) is "
		"deterministic given a signature, but reliably re-deriving it in Python "
		"would mean reimplementing its parameter-classification rules; instead "
		"this is filled in by hand next to the symbol in WEAVER_ASM_SYMBOLS, "
		"the same place a new example's *_asm.cpp already gets added, and "
		"checked against the real disassembly when it's added.",
	)
	args = parser.parse_args()

	for obj in args.object.split(";"):
		result = subprocess.run(
			[args.objdump, "-d", "--no-show-raw-insn", "-Mintel", "-C", obj],
			capture_output=True,
			text=True,
		)
		if result.returncode != 0:
			print(f"error: {args.objdump} failed on {obj}:\n{result.stderr}", file=sys.stderr)
			return 1

		insns = extract(result.stdout, args.symbol)
		if insns:
			lines = ([f"; {args.params}"] if args.params else []) + insns
			with open(args.output, "w") as f:
				f.write("\n".join(lines) + "\n")
			return 0

	print(
		f"error: no disassembly found for symbol '{args.symbol}' in any of "
		f"{args.object.replace(';', ', ')} -- was it inlined away, or renamed?",
		file=sys.stderr,
	)
	return 1


if __name__ == "__main__":
	sys.exit(main())
