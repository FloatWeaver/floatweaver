# FloatWeaver

FloatWeaver is a C++17 library for writing CUDA-style SIMD code that runs on the CPU. You write ordinary, scalar-looking code against vector types such as `weaver::float8`, and it compiles down to SSE, AVX, AVX-512 or NEON instructions, or to a portable scalar fallback.

The vector types also work as the scalar type of an Eigen matrix, so `Eigen::Matrix<weaver::float8, 3, 1>` is eight 3D vectors processed in lockstep.

```cpp
#include <weaver/vector_types.h>

weaver::float8 x{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
weaver::float8 y{ 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };

weaver::float8 z = x + y;   // one vector add per lane group
```

```cpp
#include <Eigen/Geometry>
#include <weaver/eigen.h>

Eigen::Matrix<weaver::float8, 3, 1> a{ weaver::float8{1.0f}, weaver::float8{2.0f}, weaver::float8{3.0f} };
Eigen::Matrix<weaver::float8, 3, 1> b{ weaver::float8{4.0f}, weaver::float8{5.0f}, weaver::float8{6.0f} };

Eigen::Matrix<weaver::float8, 3, 1> c = a.cross(b);   // eight cross products at once
```

Branches are expressed with masked, branchless primitives (`select`, `cswap`, `cnswap`) instead of `if`, the same way divergent code is handled on a GPU.

## Features

- `float`, `int` and `bool` vectors at widths 4, 8 and 16 (`float4`, `int8`, `bool16`, ...).
- Arithmetic, comparison and reduction operators, plus `<cmath>`-style free functions (`sqrt`, `exp`, `log`, `min`, `max`, `sgn`, ...).
- Load, store, gather, scatter and interleave helpers.
- Eigen integration via `NumTraits` and `numext` specializations.
- Vectors wider than the hardware register are split across several registers automatically, so `float16` works on SSE and `float8` works on NEON.

## Supported backends

The instruction set is chosen at configure time with the `WEAVER_VECTORIZE` CMake cache variable. It is not autodetected.

| `WEAVER_VECTORIZE` | Target |
| --- | --- |
| `Generic` | Portable scalar fallback |
| `SSE 4.2` (default) | x86 / x86-64 |
| `AVX`, `AVX 2` | x86-64 |
| `AVX 512` | x86-64 |
| `NEON` | ARM / AArch64 |

CI builds and tests the library with GCC, Clang and MSVC on x86-64 and ARM64, and with Emscripten.

## Building

You need CMake 3.21 or newer and a C++17 compiler. The dependencies are [Eigen](https://eigen.tuxfamily.org) 3.4 and, for the tests, [Catch2](https://github.com/catchorg/Catch2) 3.

**With Conan 2:**

```bash
conan install --build=missing -of build .
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_TOOLCHAIN_FILE=build/build/Release/generators/conan_toolchain.cmake \
      -DWEAVER_VECTORIZE="AVX 2"
cmake --build build --config Release
```

On multi-config generators (for example Visual Studio), the toolchain file is in `build/build/generators` instead.

**Without Conan:** if Eigen or Catch2 aren't found as packages, CMake falls back to the git submodules under `external/`:

```bash
git submodule update --init
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

**Running the tests:**

```bash
ctest --test-dir build --build-config Release
```

Tests are only built when FloatWeaver is the top-level project. To use it from another CMake project, add it with `add_subdirectory` and link against the `weaver` target.

## Documentation

The documentation is built with Doxygen, Breathe and Sphinx. It includes how-to guides that show the assembly each example compiles to. Build it locally with:

```bash
cmake -S . -B build -DWEAVER_BUILD_DOCS=ON   # plus the options above
cmake --build build --target docs
```

The code examples in the docs live in [`doc/snippets`](doc/snippets) and are compiled and run as part of the test suite, so they stay in sync with the API.

## Origin

FloatWeaver started as the SIMD module of the [Visual Computing Library (VCL)](https://github.com/bfierz/vcl). Basil Fierz began VCL in 2014 to collect the algorithms and data structures from a PhD on visual simulation.

VCL grew into a large library covering many topics, but its SIMD types are useful on their own. FloatWeaver reimplements that module from scratch, along with parts of the math built on it, as a small standalone library. It targets C++17 instead of VCL's C++14 and uses a CMake build with Conan-managed dependencies. The core vector types and the Eigen adapter are complete. Examples, benchmarks and broader test coverage are still being brought over.

## Acknowledgements

The trigonometric, logarithm and power functions build on existing SIMD math libraries. The SSE and NEON versions come from [Julien Pommier's sse_mathfun](http://gruntthepeon.free.fr/ssemath/), the AVX version is by Giovanni Garberoglio, and the AVX-512 version comes from [JishinMaster's simd_utils](https://github.com/JishinMaster/simd_utils).

## License

FloatWeaver is released under the [MIT License](LICENSE).
