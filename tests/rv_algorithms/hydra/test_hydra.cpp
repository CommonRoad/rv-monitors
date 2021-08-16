//
// Created by Sebastian Maierhofer on 15.01.21.
//

#include "test_hydra.h"
#include "monitors/parser/parser.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"

TEST(TestHydra, violated1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"a"}, {"b"}, {"b"}, {"b"}});
    Formula formula = Parser::parse("G(a)");

    // EXPECT_FALSE(Hydra::eventConsuming(formula, trace));
}
