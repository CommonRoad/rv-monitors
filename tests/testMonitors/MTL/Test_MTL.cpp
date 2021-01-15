//
// Created by Sebastian Maierhofer on 15.01.21.
//

#include "Test_MTL.h"
#include <MTL/MTLEventConsuming.h>
#include "Parser/Parser.h"
#include "Trace/Trace.h"
#include "Utils/Utils.h"

//TEST(TestMTL, violated1) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {
//            {"b"},
//            {"b"},
//            {"b"},
//            {"a"},
//            {"b"},
//            {"b"},
//            {"b"}});
//    Formula formula = Parser::parse("G(a -> F(b & O(c)))");
//
//    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
//}
//
//TEST(TestMTL, satisfied1) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {
//            {"b"},
//            {"b"},
//            {"b"},
//            {"a"},
//            {"b"},
//            {"c"},
//            {"b"}});
//    Formula formula = Parser::parse("G(a -> F(b & O(c)))");
//
//    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
//}
//
//TEST(TestMTL, satisfied2) {
//    Trace trace = Utils::createTrace({"a", "b"}, {
//            {"b"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"b"}});
//    Formula formula = Parser::parse("G(a -> F(b)");
//
//    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
//}
//
//TEST(TestMTL, violated2) {
//    Trace trace = Utils::createTrace({"a", "b"}, {
//            {"b"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"}});
//    Formula formula = Parser::parse("G(a -> F(b)");
//
//    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
//}