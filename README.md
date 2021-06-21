# RV-Monitors
A `C++17` Implementation of four runtime verification monitor synthesis algorithms [[1]](#1) for LTLf and PLTLf formulas on finite traces:
- Dynamic Programming
- Event Consuming
- Multi-transition Finite State Machine (MT-FSM*)
- Binary Transition Finite State Machine (BTT-FSM*)

## Build and Compile

Tested with 
- CMake 3.18.3
- Clang compiler 10.0.1
- GCC 10.2.0

### Dependencies

#### Bundled Dependencies:
- [ANTLR4](https://www.antlr.org/) is used for parsing formulas. For this, the following files from [here](https://www.antlr.org/download/index.html) are used:
    - `antlr4-cpp-runtime-4.8-source.zip`
    - `antlr-4.7-complete.jar`

- [gtest](https://github.com/google/googletest) version 1.10.0 is used for tests.

## Build
1. Make a build folder and change into it:
```bash
mkdir build-debug && cd build-debug
```
**NB:** You can in theory use `build` as a name
for the build folder, however keep in mind that Python's setuptools will always use the `build` folder for
their build process.
This normally shouldn't cause any issues since the file names
used by setuptools don't clash with any names CMake uses currently, but you should still consider using separate folders just in case.

Generate the build files with `cmake`.
Consider specifying an installation prefix where you want to install the RV-Monitors using `-DCMAKE_INSTALL_PREFIX`.
The recommended approach is to use a user-writable folder as the installation prefix.
It is totally fine to use the same installation prefix for all C++ libraries.

```bash
cmake \
-DCMAKE_INSTALL_PREFIX=/path/to/install/prefix \
-DCMAKE_BUILD_TYPE=Release ..
```

Afterward build with make,
```bash
make -j4
```
where you can replace `-j4` in case more/less threads are available for the build.

## Documentation
Add the `-DBUILD_DOXYGEN=ON` to the cmake command above.
Afterward, the documentation can be generated with
```bash
cd build-debug
make doc_doxygen
```

## References
<a id="1">[1]</a> 
G. Roşu and K. Havelund. “Rewriting-based techniques for runtime verification.”
In: Automated Software Engineering 12.2 (2005), pp. 151–197.
