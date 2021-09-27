//
// Created by Sebastian Maierhofer on 15.01.21.
//

#include "test_hydra.h"
#include "monitors/parser/parser.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"
#include "monitors/rv_algorithms/hydra/hydra.h"

TEST(TestHydra, violated1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b",  "c"}, {"b", "c"}, {"a", "b", "c"}});
    Formula formula = Parser::parse("P(c)");

    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, false}, Verdict{1, false}, Verdict{4, false}, Verdict{4, false}, Verdict{5, false}};

    auto monitor{Hydra(formula)};

    auto solution{monitor.evaluate(trace)};
    for(const auto &sol : solution)
        std::cout << sol.ts << " - " << sol.b << '\n';
}
