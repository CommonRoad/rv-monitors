# Formula and log generators

`gen_fmla` and `gen_log` are Linux x86-64 builds of the random formula and log generators from
[Hydra](https://bitbucket.org/krle/hydra) (`evaluation/gen_fmla.cpp`, `evaluation/gen_log.cpp`).
`scripts/run_eval.py` and the CI evaluation job use them.

They are redistributed under the BSD 3-Clause License, Copyright (c) 2019 Martin Raszyk, ETH Zurich
(see [`THIRD_PARTY_NOTICES.md`](../../THIRD_PARTY_NOTICES.md)).

To rebuild them on another platform, clone Hydra and run `make gen_fmla gen_log` in its `evaluation` directory.
