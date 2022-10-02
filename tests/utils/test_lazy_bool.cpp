#include "test_lazy_bool.h"
#include "monitors/trace/lazy_function.h"

TEST(TestLazyBool, testFixedValue) {
    LazyFunction<bool> varT(true);
    EXPECT_TRUE(static_cast<bool>(varT));
    LazyFunction<bool> varF(false);
    EXPECT_FALSE(static_cast<bool>(varF));
    LazyFunction<bool> varD;
    EXPECT_FALSE(static_cast<bool>(varD));
}

TEST(TestLazyBool, testLazyEvaluation) {
    std::function<bool()> varT = []() { return true; };
    std::function<bool()> varF = []() { return false; };
    LazyFunction<bool> lazyBoolTrue(varT);
    EXPECT_TRUE(static_cast<bool>(lazyBoolTrue));
    LazyFunction<bool> lazyBoolFalse(varF);
    EXPECT_FALSE(static_cast<bool>(lazyBoolFalse));
}