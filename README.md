# libJSON
Just another JSon library

## Motivation
This project was born as a way to try out C++17 and `shared_ptr`-based tree
structures, with no real intention of production use. From there it grew into
a lightweight, multiplatform, dependency-free library (not a header, but a
library) capable of handling big JSON files. As a bonus track, I wanted to
have similar syntax to the reference Java library...

And here we are, a C++17 library.

## How does it compare?

Since this is a learning project, it made sense to see where it actually
stands against established C++ JSON libraries. [`benchmarks/`](benchmarks/)
compares write (build a DOM + serialize) and read (parse + walk the DOM)
performance against:

- [nlohmann/json](https://github.com/nlohmann/json)
- [RapidJSON](https://github.com/Tencent/rapidjson)
- [simdjson](https://github.com/simdjson/simdjson) (read-only in this
  comparison — it has no DOM-building/writer API)
- [glaze](https://github.com/stephenberry/glaze), compared through its
  generic DOM type (`glz::json_t`), not its much faster reflection-over-structs
  mode, to keep the comparison DOM-to-DOM (see
  [`benchmarks/README.md`](benchmarks/README.md) for why)

Three dataset sizes are used (random-walk OHLC minute candles grouped by
hour): 10 minutes (small), 1 day (medium), 1 week (large). Minimum time over
30 rounds, in microseconds:

**Write** (build DOM + serialize)

| Dataset | libJSON | nlohmann/json | RapidJSON | glaze (json_t) |
|---|---:|---:|---:|---:|
| Small (10 candles)      | 34.0     | 21.0     | 4.6     | 8.0     |
| Medium (1 440 candles)  | 4 388.1  | 3 042.3  | 1 238.4 | 1 106.5 |
| Large (10 080 candles)  | 32 568.2 | 21 225.0 | 6 273.6 | 9 714.9 |

**Read** (parse + walk DOM)

| Dataset | libJSON | nlohmann/json | RapidJSON | glaze (json_t) | simdjson |
|---|---:|---:|---:|---:|---:|
| Small (10 candles)      | 21.1     | 26.4     | 3.9     | 7.0     | 3.3     |
| Medium (1 440 candles)  | 2 632.2  | 3 737.0  | 459.8   | 1 077.3 | 376.3   |
| Large (10 080 candles)  | 19 772.3 | 27 254.9 | 3 321.3 | 9 866.3 | 2 820.1 |

`libJSON`'s containers are `std::vector<std::pair<std::string, PtrJSONBase>>`
for objects and `std::vector<PtrJSONBase>` for arrays — no raw `new`/`delete`
anywhere, same as glaze's criterion. Small, linearly-scanned vectors beat a
hash map for the handful of keys a typical JSON object has, and a vector
beats a linked list for both iteration and (real, O(1)) random access. That
alone makes `libJSON` **read faster than nlohmann/json at every size**
tested. Writing is still the slowest of the four: every node is still its
own heap allocation (one `shared_ptr` per value, via `make_shared`), which
is exactly the C++17/`shared_ptr` experiment this project set out to run.

### Which one would I actually pick?

If I had to pick one of these for real use, I'd lean towards **glaze**,
mainly for memory-safety reasons: its JSON read/write path is built entirely
on standard containers (`std::vector`, `std::string`, `std::map`,
`std::variant`), and the few raw `new` calls that exist anywhere in the
library live outside the JSON hot path and are wrapped in `std::shared_ptr`
with a custom deleter (RAII, not a bare owning pointer) — the actual
serialization code has no manual memory management at all. It's also backed
by a dedicated [`fuzzing/`](https://github.com/stephenberry/glaze/tree/master/fuzzing)
suite wired to OSS-Fuzz that specifically targets roundtrip bugs.

That said, I'm genuinely impressed by **RapidJSON**'s craftsmanship: 11+
years of battle-testing at Tencent scale, a real `unittest`/`perftest` suite,
and a CI that runs under Valgrind. Its speed comes precisely from a
hand-written arena allocator (`RAPIDJSON_NEW`/`RAPIDJSON_DELETE`,
`malloc`/`realloc`/`free`), and it does it carefully and consistently. But
for my own criterion — memory safety first — I still prefer the fact that
glaze's code has no `new` in it at all over a library that manages memory by
hand, however well it does so.

## Requirements
The repo brings two build targets:
- The library itself has no dependency.
- The tester target uses [GoogleTest](https://github.com/google/googletest), fetched via Conan.

## Authors
- Ignacio Pomar Ballestero



## Building
Requires CMake (>= 3.21) and [Conan](https://conan.io/) (2.x).

```sh
conan profile detect --force   # only needed once
conan install . --output-folder=build --build=missing -s compiler.cppstd=17
cmake --preset conan-release
cmake --build --preset conan-release
ctest --preset conan-release   # or run build/build/Release/test/libJSONTester directly
```

Pass `-o shared=True` to `conan install` to build libJSON as a shared library.

The library is also packaged as a Conan recipe, so it can be built, tested and
installed into the local Conan cache with:

```sh
conan create . -s compiler.cppstd=17
```

## License
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.

In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
