# Changelog

## [Unreleased]

### Added

- `MonitorInterface::setEvaluationStartTimeStep` / `RVMonitor::verify(..., startTimeStep)` (Python: `set_evaluation_start_time_step`):
  `Hydra2` only evaluates, reports, and terminates early on events from this time step on. Earlier events (e.g., the
  history of a vehicle) stay in the trace as context for past operators. This also applies to `initial_step`, which now
  evaluates the first event at or after the start time step.

## [1.0.0]

First public release.

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
- Documentation on development, documentation generation, and licensing.

### Changed

- Relicensed from CC BY-NC-SA 4.0 to the BSD 3-Clause License (see `THIRD_PARTY_NOTICES.md` for third-party components).

[1.0.0]: https://github.com/CommonRoad/rv-monitors/releases/tag/1.0.0
