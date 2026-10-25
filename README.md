# RV-Monitors

A `C++17` library of temporal logic runtime verification algorithms [[1]](#1), [[2]](#2) for **LTLf**, **PLTLf**, and **MTL** formulas on finite traces.
It comes with Python bindings (`rvm`).
RV-Monitors parses a formula, builds a monitor with one of several algorithms, and returns a timestamped verdict for each step of a trace.

| Algorithm | C++ / Python class | Supported formulas |
|-----------|--------------------|-------|
| Dynamic Programming | `DynamicProgrammingLTL` | future-only LTLf or past-safety PLTLf |
| Event Consuming | `EventConsumingLTL` | future-only LTLf or past-safety PLTLf |
| MTL Event Consuming | `MTLEventConsuming` | MTL (safety) |
| Multi-transition Finite State Machine (MT-FSM*) | `MtFsm` | future-only LTLf or past-safety PLTLf |
| Binary Transition Finite State Machine (BTT-FSM*) | `BttFsm` | future-only LTLf or past-safety PLTLf |
| Multi-Head Monitoring (Hydra) | `Hydra` | MTL (safety), based on [[2]](#2) |
| Extended Multi-Head Monitoring | `Hydra2` | first-order MTL (safety) over domains; predicate reordering and short-circuiting |

## Installation

### Python

Building from source requires Python 3.10–3.13, a C++17 compiler, CMake ≥ 3.24, and a Java runtime (used by ANTLR to generate the parser).

```bash
pip install git+https://github.com/CommonRoad/rv-monitors.git
# or, from a local checkout
pip install .
```

### C++ (CMake)

Add RV-Monitors to your project with `FetchContent`:

```cmake
include(FetchContent)
FetchContent_Declare(RVMonitors
    GIT_REPOSITORY https://github.com/CommonRoad/rv-monitors.git
    GIT_TAG <commit/tag>
)
FetchContent_MakeAvailable(RVMonitors)

target_link_libraries(<MyTarget> PUBLIC RVMonitors::rvmonitors)
```

See [C++ Usage](./docs/cpp.md) for details.

## Usage

### Python

```python
import rvm

formula = rvm.Parser.parse("G((a) S[1, inf] (b))", "phi")

# atomic propositions, the propositions that hold at each step, and the time stamps
trace = rvm.create_trace({"a", "b"}, [["b"], ["a"], ["a", "b"], []], [0, 1, 2, 3])

monitor = rvm.Hydra2([formula])
verdicts = monitor.evaluate(trace)["phi"]  # one rvm.Verdict per step
for verdict in verdicts:
    print(verdict.time_stamp, verdict.value)
```

### C++

```cpp
#include <monitors/parser/parser.h>
#include <monitors/rv_algorithms/hydra_2/hydra.h>
#include <monitors/utils/utils.h>

Formula formula = Parser::parse("G((a) S[1, inf] (b))", "phi");
auto trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a", "b"}, {}}, {0, 1, 2, 3});

Hydra2 monitor({formula});
auto verdicts = monitor.evaluate(trace).at("phi");
```

## Development

```bash
cmake -B build -S .
cmake --build build
ctest --test-dir build
```

```bash
pip install -e ".[dev,test]"
pytest tests/python/bindings
```

For the full development setup (pre-commit, formatting, static analysis), see [Development](./docs/development.md) and [CONTRIBUTING.md](./CONTRIBUTING.md).
Other docs:

- [Evaluation against Hydra/Vydra](./docs/vydra.md)
- [Generating the documentation](./docs/documentation.md)
- [Changelog](./CHANGELOG.md)

## Citation

If you use RV-Monitors in your research, please cite it as described in [`CITATION.cff`](./CITATION.cff).

## Contributors

RV-Monitors is developed by Sebastian Maierhofer.
We thank the following people for their contributions:
Florian Lercher, Tobias Markus, Valentin Merle, and Yannick Ballnath.

## License

RV-Monitors is released under the [BSD 3-Clause License](./LICENSE).
For the licenses of third-party components, see [`THIRD_PARTY_NOTICES.md`](./THIRD_PARTY_NOTICES.md) and [Licensing](./docs/licensing.md).

## References

<a id="1">[1]</a>
G. Roşu and K. Havelund. “Rewriting-based techniques for runtime verification.”
In: Automated Software Engineering 12.2 (2005), pp. 151–197.

<a id="2">[2]</a>
M. Raszyk, D. Basin, D. Traytel. “Multi-head Monitoring of Metric Dynamic Logic.”
In: Automated Technology for Verification and Analysis (ATVA) 2020, pp. 233–250.
