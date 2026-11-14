#include "test_lazy_bool.h"
#include "monitors/trace/event_value.h"

#include <cstring>
#include <new>

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

// The value of a default-constructed EventValue was uninitialized. The storage is filled with garbage before the
// construction so that an uninitialized value does not appear as zero by chance.
TEST(TestLazyBool, testDefaultValueIsInitialized) {
    alignas(EventValue<int>) unsigned char intStorage[sizeof(EventValue<int>)];
    std::memset(intStorage, 0xFF, sizeof(intStorage));
    auto *intValue{new (intStorage) EventValue<int>()};
    EXPECT_EQ(intValue->get(), 0);
    EXPECT_EQ(EventValue<int>(*intValue).get(), 0);
    intValue->~EventValue<int>();

    alignas(EventValue<bool>) unsigned char boolStorage[sizeof(EventValue<bool>)];
    std::memset(boolStorage, 0xFF, sizeof(boolStorage));
    auto *boolValue{new (boolStorage) EventValue<bool>()};
    EXPECT_EQ(boolValue->to_string(), "false");
    boolValue->~EventValue<bool>();
}
