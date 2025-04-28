import time
from typing import List, Tuple

import pandas as pd
from matplotlib import pyplot as plt
import seaborn as sns
from evaluation.run_eval import gen_log
import rvm
import matplotlib

from tests.bindings.test_bindings import make_event_list

matplotlib.use("TkAgg")

specs1 = [
    ("S1", "G((a('rand')) S[0, 4] (b('rand')))"),
    (
        "S2",
        "G((a('rand')) U[0, 4] (b('rand')))",
    ),
    ("S3", "G((a('rand')) U[0, 4] (b('rand')) & (a('rand')) S[0, 4] (b('rand')))"),
]

specs2 = [
    ("S1", "G(P[0, 4] (a('rand')))"),
    (
        "S2",
        "G(X[0, 4] (b('rand')))",
    ),
    ("S3", "G((c('rand')) U[0, 4] (d('rand')) & (e('rand')) S[0, 4] (f('rand')))"),
]

trace = []


# 0.00001ms
def a(time_step: int, rand: int) -> bool:
    time.sleep(float(0.00001 / 1000))
    return "a" in trace[time_step]


# 0.00005ms
def b(time_step: int, rand: int) -> bool:
    time.sleep(0.00005 / 1000)
    return "b" in trace[time_step]


# 0.00002ms
def c(time_step: int, rand: int) -> bool:
    time.sleep(0.00002 / 1000)
    return "c" in trace[time_step]


# 0.000001ms
def d(time_step: int, rand: int) -> bool:
    time.sleep(0.000001 / 1000)
    return "d" in trace[time_step]


# 0.00002ms
def e(time_step: int, rand: int) -> bool:
    time.sleep(0.00002 / 1000)
    return "e" in trace[time_step]


# 0.000001ms
def f(time_step: int, rand: int) -> bool:
    time.sleep(0.000001 / 1000)
    return "f" in trace[time_step]


def create_trace(length: int, num_atomics: int, seed: int) -> List[List[str]]:
    gen_log("./eval", length, 1, 10, seed, num_atomics)

    # Initialize the lists
    actions = []

    # Open the file and read the lines
    with open("./logs/eval.log", "r") as file:
        lines = file.readlines()

    # Process each line
    for line in lines:
        # Split the line into parts
        parts = line.strip().split()

        # The remaining parts are the actions
        action = []
        for part in parts[1:]:
            if part == "a0":
                action.append("a")
            elif part == "a1":
                action.append("b")
            elif part == "a2":
                action.append("c")
            elif part == "a3":
                action.append("d")
            elif part == "a4":
                action.append("e")
            elif part == "a5":
                action.append("f")
            else:
                raise RuntimeError("unknown atomic")

        actions.append(action)

    return actions


def evaluate(approach: str, mon: rvm.Hydra2, df: pd.DataFrame, trace_length: int):
    event_list = make_event_list(trace_length)
    tr = rvm.Trace(
        event_list,
        [
            rvm.Function(a),
            rvm.Function(b),
            rvm.Function(c),
            rvm.Function(d),
            rvm.Function(e),
            rvm.Function(f),
        ],
        rvm.ConstantCollection([rvm.ConstantValue("rand", 0)]),
    )

    elapsed_time = 0
    num_iters = 5
    for ite in range(num_iters):
        start = time.time()
        mon.evaluate(tr)
        elapsed_time += time.time() - start
    df.loc[len(df)] = [approach, trace_length, elapsed_time / num_iters]


def eval_and_fig(spec: List[Tuple[str, str]], name: str, num_atomics: int):
    trace_lengths = range(200, 5200, 200)
    x_axis_text = "Trace Length [x100]"

    df = pd.DataFrame(columns=["Approach", x_axis_text, "Comp. Time [s]"])
    formulas = [rvm.Parser.parse(form[1], form[0]) for form in spec]
    for trace_length in trace_lengths:
        print(trace_length)
        global trace
        trace = create_trace(trace_length, num_atomics, 2)
        evaluate(
            "Baseline",
            rvm.Hydra2(formulas, connectedEval=False, shortCircuiting=False),
            df,
            trace_length,
        )
        evaluate("Ours", rvm.Hydra2(formulas), df, trace_length)

        plt.figure(figsize=(10, 8))
        plt.rcParams["svg.fonttype"] = "none"
        line_plot = sns.lineplot(
            data=df,
            x=x_axis_text,
            y="Comp. Time [s]",
            hue="Approach",
            marker="o",
            markersize=10,
        )
        line_plot.set_xlabel(x_axis_text, labelpad=40)
        line_plot.set_ylabel("Computation Time [s]", labelpad=45)
        line_plot.set_ylim(bottom=0)
        plt.setp(line_plot.lines, linewidth=4)
        plt.tick_params(axis="x", which="major", pad=30)
        plt.xticks(trace_lengths, [str(int(i / 100)) for i in trace_lengths])
        plt.legend(fontsize=30)
        plt.savefig(f"./{name}.svg")


eval_and_fig(specs1, "running_example_predicate", 2)
eval_and_fig(specs2, "dissimilar_predicate", 6)
