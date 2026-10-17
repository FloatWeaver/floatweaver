# AGENTS.md

This file provides guidance to AI coding agents working with code in this repository.

## Project background

FloatWeaver is a header-mostly C++17 library for writing CUDA-style SIMD code that runs on the CPU: you write scalar-looking code against `VectorScalar<T, Width>` types and it compiles down to SSE/AVX/AVX512/NEON instructions (or a portable scalar fallback), with Eigen integration so `Eigen::Matrix<weaver::float4, 3, 1>` etc. work out of the box.

**Status**: the core SIMD types (bool/float/int × 4/8/16, ref/SSE/AVX/AVX512/NEON backends) and the Eigen adapter are implemented and have baseline Catch2 tests. Not yet done: examples, broader test coverage, user-facing documentation, and benchmarks — treat these as open work, not as things that don't belong here. There is also an open, undecided question about whether some standalone high-speed SVD tooling should live in this repo or a separate one under the same org — don't assume either answer without asking.

## Build & test commands

Dependencies (Eigen3, Catch2) are managed via Conan 2; do a full from-scratch build with:

```bash
conan install --build=missing -of build .
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=build/build/Release/generators/conan_toolchain.cmake
cmake --build build --config Release
```

(On Windows/multi-config generators the generators dir is `build/build/generators` instead — see `.github/workflows/build.yml` for the exact matrix.)

Run the whole test suite:

```bash
ctest --test-dir build --build-config Release
```

Run a single test / tag with Catch2 directly (faster than going through ctest), e.g. only float tests, or one named test case:

```bash
build/weaver_tests "[float]"
build/weaver_tests "Sqrt FloatN"
```

The vectorization backend used for the build is a CMake cache variable, not autodetected:

```bash
cmake -S . -B build -DWEAVER_VECTORIZE="AVX 2"   # one of: Generic, SSE 4.2, AVX, AVX 2, AVX 512, NEON
```

This sets `WEAVER_VECTORIZE_*` compile definitions (see `cmake/config.cmake`) which in turn drive which per-ISA headers get pulled in (see Architecture below). When adding a new SIMD feature, it needs to be exercised under more than one backend to be considered validated — CI (`.github/workflows/build.yml`) builds gcc/clang/MSVC/emscripten across the matrix, but locally only one backend is active per configure.

## Architecture

**File naming = one axis per file.** Each SIMD type is split as `{category}{width}_{isa}.h`, e.g. `float8_avx.h`, `bool16_avx512.h`, `int4_neon.h`, plus a `*_ref.h` scalar-fallback variant for every width. There's no single "generic float vector" header — instead `include/weaver/eigen.h` (and equally `vector_types.h`'s consumers) `#if`-chains on `WEAVER_VECTORIZE_AVX512` / `_AVX` / `_SSE` / `_NEON` to pick exactly one full set of backend headers, falling back to `*_ref.h` for Generic builds. When adding a new op, it must be implemented consistently across all five backends (ref/SSE/AVX/AVX512/NEON) for a given width — a partial implementation will only surface as a build failure on the backends you didn't touch.

**Two-layer register model** (`include/weaver/vector_types.h`):
- `SimdRegister<Scalar, SimdExt>` is the thin, per-ISA-and-scalar-type wrapper around the actual intrinsic type (`__m128`, `float32x4_t`, `__mmask16`, ...). It only knows how to `set`/`get` lanes.
- `VectorScalarBase<Scalar, Width, SimdExt>` stacks `NrRegs = Width / NrValuesPerReg` of those registers to build wider logical vectors from narrower physical registers (e.g. a `float16` on SSE hardware is 4 `__m128` registers). `VectorScalar<Scalar, Width>` (defined per-ISA in the `{type}{width}_{isa}.h` files) is the public-facing type built on top of this.

**Operator codegen via macros**, not templates, because SIMD intrinsics for +, -, cmp, reductions etc. don't share a signature: `common.h` defines `WEAVER_SIMD_UNARY_OP` / `_BINARY_OP` / `_ASSIGN_OP` / `_COMP_OP` / `_QUERY_OP` / `_UNARY_REDUCTION_OP` / `_BINARY_REDUCTION_OP`, each expanding through `WEAVER_SIMD_P1_N` / `WEAVER_SIMD_P2_N` helper macros that unroll across the `NrRegs` registers for a given width `N` (4/8/16). Each per-width-per-ISA header instantiates these macros with the intrinsic function name for that op. `WEAVER_SIMD_VECTORSCALAR_SETUP(ISA)` wires up the constructors and type aliases (`Self`, `Bool`, `RegType`) each `VectorScalar` specialization needs.

**Free functions vs. members**: elementwise math (`sqrt`, `exp`, `log`, `min`, `max`, `select`, `equal`, `sgn`, ...) is exposed as free functions in `namespace weaver` (`vector_types.h`, bottom) that mostly just forward to a member of the same name — this mirrors `<cmath>`/Eigen calling conventions and is what makes generic code (templated on `Scalar`/`Width`) work uniformly across scalar and vector types.

**Eigen integration** (`include/weaver/eigen.h`): specializes `Eigen::NumTraits<weaver::floatN>` and the relevant `Eigen::numext` functions so `weaver::floatN`/`int N`/`boolN` can be used as the `Scalar` of an `Eigen::Matrix`. `select`/`cswap`/`cnswap` (`conditional.h`) are the masked-branchless primitives this whole model relies on instead of real control flow — any "CUDA-style" branch in user code should compile down to one of these rather than an `if`.

**`intrinsics_*.h`/`.cpp`** wrap raw platform intrinsics not otherwise exposed cleanly (lane extraction, mathfun-derived transcendentals in `src/detail/*_mathfun.h`, sourced from Julien Pommier / Giovanni Garberoglio / JishinMaster implementations). **`memory_*.h`** covers load/store/gather/scatter/interleave, which is why those get their own test files (`load.cpp`, `store.cpp`, `gather.cpp`, `scatter.cpp`, `interleave.cpp`) rather than living inside `float.cpp`/`int.cpp`.

**Tests** (`src/tests/`) use Catch2 v3. `src/tests/common.h` provides `WEAVER_SIMD_FLOATS` / `_INTS` / `_BOOLS` macros that inject a standard set of ascending/descending fixture vectors (`f4_asc`, `f8_desc`, ...) at all three widths into a test case — reuse these instead of hand-rolling fixtures. Tag tests with `[float]`/`[int]`/`[bool]`/`[operation]` etc. matching the existing convention so `-t "[tag]"` filtering keeps working.
