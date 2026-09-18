How-to guides
=============

Each guide below is pulled directly from a compiled, executed Catch2 test
(tagged ``[doc]`` in ``doc/snippets/``), so it can't drift out of sync with
the API it demonstrates -- if a guide stops matching reality, the docs build
fails.

Add two float8 vectors
-----------------------

``weaver::float8`` behaves like a scalar ``float`` for the usual arithmetic
operators, but computes on 8 lanes at once:

.. literalinclude:: ../snippets/float8_snippets.cpp
   :language: cpp
   :start-after: //! [Add float8]
   :end-before: //! [Add float8]

That single ``+`` isn't sugar over a loop -- it's one SIMD instruction. The
function below is compiled and disassembled by the docs build itself
(``doc/snippets/float8_asm.cpp``); its operands arrive as parameters rather
than literals so the compiler can't fold the addition away at compile time:

.. literalinclude:: ../snippets/float8_asm.cpp
   :language: cpp
   :start-after: //! [Add float8 asm]
   :end-before: //! [Add float8 asm]

Which the configured toolchain (here: whatever ``WEAVER_VECTORIZE`` this docs
build used) turns into:

.. weaver-asm:: add_float8

The exact instructions depend on the compiler and target ISA -- an AVX build
emits a single ``vaddps`` on 8-wide ``ymm`` registers, while an SSE build
splits the same 8 lanes across two 4-wide ``xmm`` registers instead. Either
way, this is the real output of the currently configured toolchain, not a
hand-picked example.

Pick a value without branching
-------------------------------

FloatWeaver code has no real ``if``: an ``if`` would have to pick one branch
for the whole vector, but different lanes can want different answers. Instead
you compute both possibilities and use ``select`` to keep, per lane,
whichever one the (also per-lane) condition asked for -- the branchless
equivalent of ``a[i] < b[i] ? b[i] : a[i]``:

.. literalinclude:: ../snippets/select_snippets.cpp
   :language: cpp
   :start-after: //! [Select branchless max]
   :end-before: //! [Select branchless max]

Same isolate-and-disassemble treatment as above
(``doc/snippets/select_asm.cpp``):

.. literalinclude:: ../snippets/select_asm.cpp
   :language: cpp
   :start-after: //! [Select branchless max asm]
   :end-before: //! [Select branchless max asm]

.. weaver-asm:: select_max

Two instructions, no branch: a compare that produces a per-lane mask, then a
blend that picks lanes from ``a`` or ``b`` according to that mask -- every
lane gets computed, and the "wrong" one is simply discarded rather than
skipped.
