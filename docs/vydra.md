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