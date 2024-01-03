import sys
import unittest
from pathlib import Path

sys.path.append(str(Path(__file__).parent.parent))

from evaluation.run_eval import run_experiment


class OriginalHydraEvaluationTest(unittest.TestCase):
    def test_eval(self):
        run_experiment('test_experiment')

        self.assertTrue((Path(__file__).parent.parent / "evaluation/figs/space_complexity.png").exists())
        self.assertTrue((Path(__file__).parent.parent / "evaluation/figs/time_complexity.png").exists())


if __name__ == '__main__':
    unittest.main()
