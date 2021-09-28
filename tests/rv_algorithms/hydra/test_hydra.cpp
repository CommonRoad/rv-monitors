//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "test_hydra.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra/hydra.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"

TEST(TestHydra, testPreviousSingle) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}},
                           std::vector<size_t>{0, 0, 1, 4, 4, 5});
    Formula formula = Parser::parse("P(c)");

    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, true}, Verdict{1, true},
                                     Verdict{4, false}, Verdict{4, true}, Verdict{5, true}};

    auto monitor{Hydra(formula)};

    auto solution{monitor.evaluate(trace)};

    for (size_t i{0}; i < expSolution.size(); ++i)
        EXPECT_TRUE(solution[i] == expSolution[i]);
}
