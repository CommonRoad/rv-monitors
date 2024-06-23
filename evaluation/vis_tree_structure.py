from typing import List, Dict
import networkx as nx
import matplotlib
import rvm

matplotlib.use("TkAgg")


def visualize_tree_from_dict(
    tree_dict: Dict[int, List[int]], root_nodes: List[int], labels: Dict[int, str]
) -> None:
    """
    Visualizes Hydra2 tree structure highlighting root and leaf nodes.

    :param tree_dict: Tree structure.
    :param root_nodes: List of root nodes.
    :param labels: Mapping of node IDs to logical representation.
    """
    node_size = 1500
    tree = nx.DiGraph(tree_dict)

    leaf_nodes = [node for node in tree.nodes() if tree.out_degree(node) == 0]
    pos = nx.drawing.layout.spiral_layout(tree)
    nx.draw(
        tree,
        pos,
        with_labels=True,
        labels=labels,
        node_size=node_size,
        node_color="lightblue",
        font_size=10,
    )
    nx.draw_networkx_nodes(
        tree, pos, nodelist=root_nodes, node_color="green", node_size=node_size
    )
    nx.draw_networkx_nodes(
        tree, pos, nodelist=leaf_nodes, node_color="red", node_size=node_size
    )

    matplotlib.pyplot.title("Tree Visualization")
    matplotlib.pyplot.axis("off")
    matplotlib.pyplot.show()


specs = [
    ("S1", "G((a) S[0, 4] (b))"),
    (
        "S2",
        "G((a) U[0, 4] (b))",
    ),
    ("S3", "G((a) U[0, 4] (b) & (a) S[0, 4] (b))"),
]
formulas = [rvm.Parser.parse(form[1], form[0]) for form in specs]
mon = rvm.Hydra2(formulas)
tree_vis = mon.to_tree()
visualize_tree_from_dict(tree_vis.tree, tree_vis.rootIDs, tree_vis.nodeTypes)
