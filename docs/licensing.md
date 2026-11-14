## Licensing

RV-Monitors is licensed under the [BSD 3-Clause License](https://opensource.org/license/bsd-3-clause).
Up to version 0.5 it was distributed under CC BY-NC-SA 4.0.
That license was replaced because Creative Commons licenses are not intended for software, and because the non-commercial clause prevented use in industrial projects.

### Dependency evaluation

Before the license was chosen, every dependency was checked for license terms that would restrict the choice:

| Dependency | Usage | License | Effect on RV-Monitors |
|------------|-------|---------|-----------------------|
| ANTLR 4 C++ runtime and generated parsers | linked, part of wheels | BSD-3-Clause | notice must be retained |
| spdlog / {fmt} | linked | MIT | notice must be retained |
| nanobind | linked into the Python extension | BSD-3-Clause | notice must be retained |
| GoogleTest | test suite only | BSD-3-Clause | none |
| `CodeCoverage.cmake` (Lars Bilke) | vendored CMake module | BSD-3-Clause | header retained |
| commonroad-cmake | optional CMake helpers fetched at configure time | none published | not redistributed |
| Python `eval` extras (matplotlib, pandas, seaborn, …) | optional, not bundled | permissive | none |

No dependency is under a copyleft license (GPL/LGPL), so none of them forces a particular license on RV-Monitors.
BSD-3-Clause matches the licenses of the main dependencies and of other CommonRoad tools.
All notices that must be retained are collected in [`THIRD_PARTY_NOTICES.md`](https://github.com/CommonRoad/rv-monitors/blob/develop/THIRD_PARTY_NOTICES.md).
