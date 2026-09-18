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

Take a square root
-------------------

Not just ``+`` compiles down to a single instruction -- most of the
``<cmath>``-style free functions in ``namespace weaver`` do too:

.. literalinclude:: ../snippets/sqrt_snippets.cpp
   :language: cpp
   :start-after: //! [Square root float8]
   :end-before: //! [Square root float8]

.. literalinclude:: ../snippets/sqrt_asm.cpp
   :language: cpp
   :start-after: //! [Square root float8 asm]
   :end-before: //! [Square root float8 asm]

.. weaver-asm:: sqrt_float8

Approximate reciprocal and reciprocal square root
---------------------------------------------------

``rcp`` and ``rsqrt`` trade a couple of ULPs of accuracy for hardware
approximation instructions that are cheaper than a real divide or square
root -- useful when normalizing a vector or computing a rough inverse where
exactness doesn't matter:

.. literalinclude:: ../snippets/approx_snippets.cpp
   :language: cpp
   :start-after: //! [Approximate reciprocal float8]
   :end-before: //! [Approximate reciprocal float8]

.. literalinclude:: ../snippets/approx_asm.cpp
   :language: cpp
   :start-after: //! [Approximate reciprocal float8 asm]
   :end-before: //! [Approximate reciprocal float8 asm]

.. weaver-asm:: rcp_float8

.. literalinclude:: ../snippets/approx_snippets.cpp
   :language: cpp
   :start-after: //! [Approximate reciprocal square root float8]
   :end-before: //! [Approximate reciprocal square root float8]

.. literalinclude:: ../snippets/approx_asm.cpp
   :language: cpp
   :start-after: //! [Approximate reciprocal square root float8 asm]
   :end-before: //! [Approximate reciprocal square root float8 asm]

.. weaver-asm:: rsqrt_float8

Branchless sign
-----------------

``sgn`` is a real library function, not a contrived example -- it's what a
branchless sign-without-``abs`` costs in practice: a mask, an and, an or, and
an and:

.. literalinclude:: ../snippets/sgn_snippets.cpp
   :language: cpp
   :start-after: //! [Branchless sign float8]
   :end-before: //! [Branchless sign float8]

.. literalinclude:: ../snippets/sgn_asm.cpp
   :language: cpp
   :start-after: //! [Branchless sign float8 asm]
   :end-before: //! [Branchless sign float8 asm]

.. weaver-asm:: sgn_float8

Compare floats with a tolerance
----------------------------------

Exact float equality is rarely what you want. ``weaver::equal`` implements a
relative-tolerance compare per lane -- a realistic idiom, a few instructions
longer than a plain ``==``:

.. literalinclude:: ../snippets/tolerance_snippets.cpp
   :language: cpp
   :start-after: //! [Compare floats with a tolerance]
   :end-before: //! [Compare floats with a tolerance]

.. literalinclude:: ../snippets/tolerance_asm.cpp
   :language: cpp
   :start-after: //! [Compare floats with a tolerance asm]
   :end-before: //! [Compare floats with a tolerance asm]

.. weaver-asm:: equal_tol_float8

Check a condition across all lanes
-------------------------------------

Individual lanes can never branch, but FloatWeaver code is still allowed a
*real* ``if`` -- as long as it branches on a single reduced bool, not on a
per-lane result. ``any``/``all``/``none`` are how a per-lane comparison
becomes that bool:

.. literalinclude:: ../snippets/reduce_snippets.cpp
   :language: cpp
   :start-after: //! [Check a condition across all lanes]
   :end-before: //! [Check a condition across all lanes]

.. literalinclude:: ../snippets/reduce_asm.cpp
   :language: cpp
   :start-after: //! [Check a condition across all lanes asm]
   :end-before: //! [Check a condition across all lanes asm]

.. weaver-asm:: any_greater_float8

A compare, a ``movmsk`` to pull the per-lane mask bits into a scalar
register, then a plain integer test -- this is the one place in these guides
where a scalar, not a vector, comes out the other end.

Horizontal reduction: dot product
-------------------------------------

Every example so far keeps 8 independent results, one per lane.
``dot`` doesn't: it collapses all 8 lanes of both operands into a single
``float``, so the instruction shape looks different from the rest of this
guide -- a multiply-reduce followed by a lane-extraction step, not a
per-lane op:

.. literalinclude:: ../snippets/dot_snippets.cpp
   :language: cpp
   :start-after: //! [Dot product float8]
   :end-before: //! [Dot product float8]

.. literalinclude:: ../snippets/dot_asm.cpp
   :language: cpp
   :start-after: //! [Dot product float8 asm]
   :end-before: //! [Dot product float8 asm]

.. weaver-asm:: dot_float8

Using FloatWeaver with Eigen
-------------------------------

``weaver::float4``/``float8``/``float16`` work as the ``Scalar`` of an
``Eigen::Matrix`` directly (see ``include/weaver/eigen.h``) -- no separate
vector type, no adapter code at the call site. The guides below show that
this isn't just type-level plumbing: ordinary Eigen algorithms, written with
no knowledge of FloatWeaver, compile down to the same kind of vector
instructions as the ``weaver``-native examples above.

Eigen vector arithmetic
--------------------------

The baseline case: an ``Eigen::Matrix<weaver::float8, 3, 1>`` is a
``Vector3`` holding 8 points at once, and ``operator+`` works exactly as it
would for ``Eigen::Vector3f``:

.. literalinclude:: ../snippets/eigen_add_snippets.cpp
   :language: cpp
   :start-after: //! [Eigen vector add with float8]
   :end-before: //! [Eigen vector add with float8]

.. literalinclude:: ../snippets/eigen_add_asm.cpp
   :language: cpp
   :start-after: //! [Eigen vector add with float8 asm]
   :end-before: //! [Eigen vector add with float8 asm]

.. weaver-asm:: add_vec3_float8

Three ``vaddps``, one per vector component -- proof the ``NumTraits``/
``numext`` wiring costs nothing over calling ``weaver::float8``'s own
``operator+`` directly.

Squared norm and Eigen algorithms
-------------------------------------

``squaredNorm()`` was never written for ``weaver::float8`` -- Eigen builds it
generically out of ``operator*`` and ``operator+``, the same way it would for
any other ``Scalar``:

.. literalinclude:: ../snippets/eigen_norm_snippets.cpp
   :language: cpp
   :start-after: //! [Eigen squared norm with float8]
   :end-before: //! [Eigen squared norm with float8]

.. literalinclude:: ../snippets/eigen_norm_asm.cpp
   :language: cpp
   :start-after: //! [Eigen squared norm with float8 asm]
   :end-before: //! [Eigen squared norm with float8 asm]

.. weaver-asm:: squared_norm_vec3_float8

Cross product
----------------

``cross()`` is the classic shuffle-and-multiply pattern -- three multiplies
and three fused multiply-subtracts, still one screenful of assembly:

.. literalinclude:: ../snippets/eigen_cross_snippets.cpp
   :language: cpp
   :start-after: //! [Eigen cross product with float8]
   :end-before: //! [Eigen cross product with float8]

.. literalinclude:: ../snippets/eigen_cross_asm.cpp
   :language: cpp
   :start-after: //! [Eigen cross product with float8 asm]
   :end-before: //! [Eigen cross product with float8 asm]

.. weaver-asm:: cross_vec3_float8

Branchless select over an Eigen vector
-------------------------------------------

The same branchless pattern as "Pick a value without branching" above, but
applied to a whole vector-valued quantity at once -- e.g. choosing one of two
candidate velocities for 8 particles in a single call, one blend per
component:

.. literalinclude:: ../snippets/eigen_select_snippets.cpp
   :language: cpp
   :start-after: //! [Branchless select over an Eigen vector]
   :end-before: //! [Branchless select over an Eigen vector]

.. literalinclude:: ../snippets/eigen_select_asm.cpp
   :language: cpp
   :start-after: //! [Branchless select over an Eigen vector asm]
   :end-before: //! [Branchless select over an Eigen vector asm]

.. weaver-asm:: select_vec3_float8

Merge axis-aligned boxes
----------------------------

This is closer to why FloatWeaver exists than a toy example:
``Eigen::AlignedBox`` needs no changes at all to operate on 8 boxes
simultaneously. ``merged()`` takes the componentwise min of both boxes'
minimums and the max of their maximums:

.. literalinclude:: ../snippets/eigen_alignedbox_snippets.cpp
   :language: cpp
   :start-after: //! [Merge axis-aligned boxes with float8]
   :end-before: //! [Merge axis-aligned boxes with float8]

.. literalinclude:: ../snippets/eigen_alignedbox_asm.cpp
   :language: cpp
   :start-after: //! [Merge axis-aligned boxes with float8 asm]
   :end-before: //! [Merge axis-aligned boxes with float8 asm]

.. weaver-asm:: merge_box2_float8
