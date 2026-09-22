# libJSON
Just another JSon library

## Motivation
I wanted a lightweight, multiplatform, with no external dependencies library
(Not a header, but a library) capable of handle big JSON files. As a bonus track, 
I wanted to have similar sintax than the reference Java library...

And here we are, a C++17 library.

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
