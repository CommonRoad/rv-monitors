import rvm
import pandas as pd
import matplotlib.pyplot as plt
from io import StringIO

formula = rvm.Parser.parse("G((a)S[1, inf](b))", "F4_1")
trace = rvm.create_trace(
    {"a", "b", "c"},
    [
        [],
        ["a"],
        ["a"],
        ["a"],
        [],
        ["a"],
        ["b"],
        ["a"],
        ["a"],
        ["a", "b"],
        ["a", "b"],
        ["a", "b"],
        ["a"],
        ["a", "b"],
        ["a", "b"],
        ["a", "b"],
        ["a"],
        ["a"],
        ["a", "b"],
        ["a"],
        ["a", "b"],
        ["a", "b"],
        ["b"],
        ["a", "b"],
        ["a"],
        ["a"],
        ["b"],
        ["b"],
        ["a", "b"],
        [],
        ["b"],
        ["a", "b"],
    ],
    [
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
        16,
        17,
        18,
        19,
        20,
        21,
        22,
        23,
        24,
        25,
        25,
        27,
        28,
        29,
        30,
        31,
    ],
)

mon = rvm.Hydra2([formula])
res = mon.evaluate(trace)["F4_1"]

# Parse CSV output
csv_data = trace.to_csv()
df = pd.read_csv(StringIO(csv_data), sep=";")

# Use first column as x-axis, others as y-axis
x_col = df.columns[0]
y_cols = df.columns[1:]

# Convert values: False->0, True->1, other->0.5 for y-columns
converted_df = df.copy()
for col in y_cols:
    converted_df[col] = converted_df[col].map(
        lambda x: 0
        if x is False or (isinstance(x, str) and x.lower() == "false")
        else 1
        if x is True or (isinstance(x, str) and x.lower() == "true")
        else 0.5
    )

# Create subplots for each y-column
fig, axes = plt.subplots(len(y_cols), 1, figsize=(8, 2 * len(y_cols)), sharex=True)
if len(y_cols) == 1:
    axes = [axes]
for ax, col in zip(axes, y_cols):
    ax.plot(converted_df[x_col].values, converted_df[col].values, drawstyle="steps-post")
    ax.set_title(col)
    ax.set_yticks([0, 0.5, 1])
    ax.set_yticklabels(["0", "0.5", "1"])
plt.xlabel(f"{x_col}")
plt.tight_layout()
plt.show()
