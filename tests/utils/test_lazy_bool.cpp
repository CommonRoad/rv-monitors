#include "test_lazy_bool.h"
#include "monitors/trace/event_value.h"

TEST(TestLazyBool, testFixedValue) {
    EventValue<bool> bool1(true);
    EXPECT_TRUE(static_cast<bool>(bool1));
    EventValue<bool> bool2(false);
    EXPECT_FALSE(static_cast<bool>(bool2));
    EventValue<bool> bool3;
    EXPECT_FALSE(static_cast<bool>(bool3));
}

TEST(TestLazyBool, testLazyEvaluation) {
    std::function<bool()> bool1 = []() { return true; };
    std::function<bool()> bool2 = []() { return false; };
    EventValue<bool> lazyBoolTrue(bool1);
    EXPECT_TRUE(static_cast<bool>(lazyBoolTrue));
    EventValue<bool> lazyBoolFalse(bool2);
    EXPECT_FALSE(static_cast<bool>(lazyBoolFalse));
}
