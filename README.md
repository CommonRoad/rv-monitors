# RV-Monitors
A `C++17` Implementation of five runtime verification monitor synthesis algorithms [[1]](#1) for LTLf and PLTLf formulas on finite traces:
- Dynamic Programming
- Event Consuming
- MTL Event Consuming
- Multi-transition Finite State Machine (MT-FSM*)
- Binary Transition Finite State Machine (BTT-FSM*)

## Build and Compile

Tested with 
- CMake 3.18.3
- Clang compiler 10.0.1
- GCC 10.2.0

### Dependencies
These dependencies should be available as a system package.
- cmake > 3.16
- Boost
- Eigen3
- spdlog
- OpenMP
- Doxygen (for building the documentation)
- Graphviz (for building the documentation)

#### Debian/Ubuntu
You require at least Ubuntu 20.04.
On Debian/Ubuntu, you'll most likely need to install the following packages:  
`build-essential git pkg-config wget libspdlog-dev`

For building the documentation, you'll need to install `doxygen` and `graphviz`.

For test coverage, you'll need to install `gcovr`.

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

## Evaluation against Hydra/Vydra

The Continuous Integration (CI) pipeline for this repository includes an automated
evaluation step which checks the Hydra monitor implementation against the
[verified reference implementation Vydra](https://bitbucket.org/krle/hydra) by the authors of the Hydra paper [[2]](#2).
The evaluation job needs to be triggered manually in Gitlab.
If the evaluation fails for some formulas
(i.e. the Hydra monitor in RV-Monitors does not arrive at the same results as either Vydra or Hydra),
follow the instructions provided in the job output in order to locally test the failing formula.

The evaluation executable is called `rvmonitors_eval` and takes two arguments,
the formula file and the log file. Example invocation: `path/to/build/directory/rvmonitors_eval $FORMULA_FILE $LOG_FILE`.

## References
<a id="1">[1]</a> 
G. Roşu and K. Havelund. “Rewriting-based techniques for runtime verification.”
In: Automated Software Engineering 12.2 (2005), pp. 151–197.

<a id="2">[2]</a>
M. Raszyk, D. Basin, D. Traytel. “Multi-head Monitoring of Metric Dynamic Logic.”
In: Automated Technology for Verification and Analysis (ATVA) 2020, pp. 233–250.
