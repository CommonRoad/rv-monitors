////
//// Created by Sebastian Maierhofer on 15.01.21.
////
//
//#include "Test_MTL.h"
//#include <MTL/MTLEventConsuming.h>
//#include "Parser/Parser.h"
//#include "Trace/Trace.h"
//#include "Utils/Utils.h"
//
////TEST(TestMTL, violated1) {
////    Trace trace = Utils::createTrace({"a", "b", "c"}, {
////            {"b"},
////            {"b"},
////            {"b"},
////            {"a"},
////            {"b"},
////            {"b"},
////            {"b"}});
////    Formula formula = Parser::parse("G(a -> F(b & O(c)))");
////
////    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
////}
////
////TEST(TestMTL, satisfied1) {
////    Trace trace = Utils::createTrace({"a", "b", "c"}, {
////            {"b"},
////            {"b"},
////            {"b"},
////            {"a"},
////            {"b"},
////            {"c"},
////            {"b"}});
////    Formula formula = Parser::parse("G(a -> F(b & O(c)))");
////
////    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
////}
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
//    Formula formula = Parser::parse("G(a -> F(b))");
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
//    Formula formula = Parser::parse("G(a -> F(b))");
//
//    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
//}
//
//TEST(TestMTL, satisfied3) {
//    Trace trace = Utils::createTrace({"a", "b"}, {
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"}});
//    Formula formula = Parser::parse("G(a)");
//
//    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
//}
//
//TEST(TestMTL, violated3) {
//    Trace trace = Utils::createTrace({"a", "b"}, {
//            {"b"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"}});
//    Formula formula = Parser::parse("G(a)");
//
//    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
//}
//
//TEST(TestMTL, violated4) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {
//            {"c"},
//            {"c"},
//            {"a"},
//            {"b"},
//            {"a"},
//            {"a"},
//            {"a"}});
//    Formula formula = Parser::parse("G(a -> O(b))");
//
//    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
//}
//
//TEST(TestMTL, satisfied4) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {
//            {"b"},
//            {"c"},
//            {"a"},
//            {"b"},
//            {"b"},
//            {"a"},
//            {"b"}});
//    Formula formula = Parser::parse("G(a -> O(b))");
//
//    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
//}
//
//TEST(TestMTL, satisfied5) {
//    Trace trace = Utils::createTrace({"a"}, {
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"}});
//    Formula formula = Parser::parse("O(a)");
//
//    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
//}
//
//TEST(TestMTL, violated5) {
//    Trace trace = Utils::createTrace({"a"}, {
//            {},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"}});
//    Formula formula = Parser::parse("O(a)");
//
//    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
//}
//
//TEST(TestMTL, satisfied6) {
//    Trace trace = Utils::createTrace({"a", "b"}, {
//            {"a"},
//            {"a"},
//            {"a"},
//            {"a"}});
//    Formula formula = Parser::parse("H(a)");
//
//    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
//}
//
//TEST(TestMTL, violated6) {
//    Trace trace = Utils::createTrace({"a", "b"}, {
//            {"b"},
//            {"a"}});
//    Formula formula = Parser::parse("H(a)");
//
//    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
//}
//
//TEST(TestMTL, satisfied7) {
//    Trace trace = Utils::createTrace({"a", "b"}, {
//            {"b"},
//            {"a"},
//            {"a"}});
//    Formula formula = Parser::parse("G(aSb)");
//
//    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
//}
//
////TEST(TestMTL, violated7) {
////    Trace trace = Utils::createTrace({"a", "b", "c"}, {
////            {"b"},
////            {"c"}});
////    Formula formula = Parser::parse("G[0,1](aS[0,1]b)");
////
////    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
////}
