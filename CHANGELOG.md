# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - Unreleased

First public release.

### Changed

- Relicensed from CC BY-NC-SA 4.0 to the BSD 3-Clause License (see `THIRD_PARTY_NOTICES.md` for third-party components).

### Added

- Runtime verification algorithms for finite traces:
  - Dynamic Programming (`DynamicProgrammingLTL`)
  - Event Consuming (`EventConsumingLTL`), including past-time evaluation
  - MTL Event Consuming (`MTLEventConsuming`)
  - Multi-transition and binary-transition finite state machines (`MtFsm`, `BttFsm`)
  - Multi-head monitoring (`Hydra`) and extended multi-head monitoring for first-order MTL (`Hydra2`)
    with predicate reordering, short-circuiting, connected evaluation, and several formula evaluation modes
- ANTLR 4–based parsers for formulas and logs.
- Domains and functions for first-order formulas (`DomainStorage`, `DomainCollection`, `ConstantCollection`, `Function`).
- Python bindings (`rvm`) based on nanobind.
- `CONTRIBUTING.md`, `CITATION.cff`, and documentation on development, documentation generation, and licensing.

[1.0.0]: https://github.com/CommonRoad/rv-monitors/releases/tag/1.0.0
