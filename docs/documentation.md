## Generating the Documentation

The documentation is built with [MkDocs](https://www.mkdocs.org/) and the Material theme.
[mkdoxy](https://github.com/JakubAndrysek/MkDoxy) generates the C++ API reference from the Doxygen comments in `src/` and `include/`.

Requirements: Doxygen (and optionally Graphviz for diagrams).

```bash
pip install ".[docs]"
mkdocs serve   # live preview at http://127.0.0.1:8000
mkdocs build   # static site in ./site
```

To get standalone Doxygen HTML instead, configure CMake with Doxygen installed and build the `doc_doxygen` target:

```bash
cmake -B build -S . -DRVMonitors_MODEL_BUILD_DOXYGEN=ON
cmake --build build --target doc_doxygen
```
