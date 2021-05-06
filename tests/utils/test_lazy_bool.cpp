#include "test_lazy_bool.h"
#include "monitors/utils/lazy_bool.h"

TEST(TestLazyBool, testFixedValue) {
    lazy_bool t(true);
    EXPECT_TRUE(static_cast<bool>(t));
    lazy_bool f(false);
    EXPECT_FALSE(static_cast<bool>(f));
    lazy_bool d;
    EXPECT_FALSE(static_cast<bool>(d));
}

TEST(TestLazyBool, testLazyEvaluation) {
    std::function<bool()> t = []() { return true; };
    std::function<bool()> f = []() { return false; };
    lazy_bool lazyBoolTrue(t);
    EXPECT_TRUE(static_cast<bool>(lazyBoolTrue));
    lazy_bool lazyBoolFalse(f);
    EXPECT_FALSE(static_cast<bool>(lazyBoolFalse));
}