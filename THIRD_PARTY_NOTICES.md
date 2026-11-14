# Third-Party Notices

RV-Monitors is distributed under the BSD 3-Clause License (see [`LICENSE.txt`](LICENSE.txt)).
It uses or redistributes the third-party components listed below.
Each component remains under its own license.

## Components redistributed in builds (libraries and Python wheels)

| Component | Version | License | Copyright |
|-----------|---------|---------|-----------|
| [ANTLR 4 C++ runtime](https://github.com/antlr/antlr4) (and the parser code generated from `Formula.g4`/`Log.g4`) | 4.13.1 | BSD-3-Clause | Copyright (c) 2012-2022 The ANTLR Project |
| [spdlog](https://github.com/gabime/spdlog) | ≥ 1.8.0 | MIT | Copyright (c) 2016 Gabi Melman |
| [{fmt}](https://github.com/fmtlib/fmt) (bundled with spdlog) | — | MIT | Copyright (c) 2012-present Victor Zverovich and {fmt} contributors |
| [nanobind](https://github.com/wjakob/nanobind) (Python bindings only) | 2.2 | BSD-3-Clause | Copyright (c) 2022 Wenzel Jakob |

## Components included in this repository

| Component | Location | License | Copyright |
|-----------|----------|---------|-----------|
| `CodeCoverage.cmake` from [bilke/cmake-modules](https://github.com/bilke/cmake-modules) | `cmake/external/CodeCoverage.cmake` | BSD-3-Clause | Copyright (c) 2012-2017 Lars Bilke |

## Build- and test-time only (not redistributed)

| Component | License |
|-----------|---------|
| [GoogleTest](https://github.com/google/googletest) | BSD-3-Clause |
| [commonroad-cmake](https://gitlab.lrz.de/tum-cps/commonroad-cmake) (optional CMake helper modules, fetched at configure time) | no license file published |

---

## License texts

### MIT License (spdlog, {fmt})

```
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

### BSD 3-Clause License (ANTLR 4, nanobind, CodeCoverage.cmake)

```
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
