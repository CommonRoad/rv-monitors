## Development Setup

### Requirements

- A C++17 compiler (CI tests GCC 10–14 and Clang 15–19)
- CMake ≥ 3.20 (≥ 3.24 for the Python package)
- Java ≥ 11, which ANTLR needs to generate the formula and log parsers
- Python 3.10–3.13 for the Python bindings

CMake fetches the C++ dependencies (ANTLR 4 runtime, spdlog, GoogleTest) with `FetchContent`, unless
it finds suitable system packages.

### Building the C++ library and tests

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build
```

Tests are built automatically when RV-Monitors is the top-level CMake project.

Useful CMake options:

| Option | Default | Description |
|--------|---------|-------------|
| `RVMonitors_BUILD_TESTS` | `ON` (top-level) | Build the GoogleTest suite |
| `RVMonitors_BUILD_SHARED_LIBS` | `ON` | Build shared instead of static libraries |
| `RVMonitors_BUILD_CODE_COVERAGE` | `OFF` | Instrument the tests for coverage (`rvmonitors_coverage` target) |
| `RVMonitors_SYSTEM_PACKAGES` | `ON` | Prefer system packages for dependencies |
| `RV_MONITOR_BUILD_PYTHON_BINDINGS` | `OFF` | Build the nanobind module (scikit-build sets this) |

### Python package

Install the package in editable mode. It is rebuilt automatically on import after C++ changes:

```bash
pip install -e ".[dev,test]"
pytest tests/python/bindings
```

### Code style

- C++: `clang-format` (`.clang-format`) and `clang-tidy` (`.clang-tidy`)
- Python: `ruff` (line length 120)

Both run as pre-commit hooks:

```bash
pip install pre-commit
pre-commit install
pre-commit run --all-files
```
