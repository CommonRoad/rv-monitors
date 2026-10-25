# Contributing to RV-Monitors

Thank you for your interest in contributing! Bug reports, feature requests, and merge/pull requests are welcome.

## Reporting issues

Please open an issue that includes:

- the formula(s) and a minimal trace that reproduce the problem,
- the monitor class you used (e.g. `Hydra2`),
- the expected and the actual verdicts,
- your platform, compiler, and Python version.

## Making changes

1. Set up the development environment as described in [docs/development.md](docs/development.md).
2. Install the pre-commit hooks: `pre-commit install`.
3. Create a feature branch from `develop` (e.g. `feature/<topic>` or `fix/<topic>`).
4. Add or update tests:
   - C++ tests in `tests/` (GoogleTest), run them with `ctest --test-dir build`
   - Python binding tests in `tests/python/bindings`, run them with `pytest tests/python/bindings`
5. Make sure that `pre-commit run --all-files` and all tests pass.
6. Open a merge/pull request against `develop` and describe the change and why it is needed.

## Code style

- C++17, formatted with `clang-format` and checked with `clang-tidy` (configurations in the repository root).
- Public C++ APIs are documented with Doxygen comments.
- Python is formatted and linted with `ruff` (line length 120).

## License

By contributing, you agree that your contributions are licensed under the [BSD 3-Clause License](LICENSE).
