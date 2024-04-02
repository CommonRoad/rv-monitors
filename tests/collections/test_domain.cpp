#include "test_domain.h"

std::vector<std::any> createDomain1(size_t stepNumber) {
    switch (stepNumber) {
    case 0:
        return {"a", "b"};
    case 1:
        return {"c", "d", "e"};
    default:
        return {"f"};
    }
}

void TestDomain::SetUp() {
    dom1 = DomainStorage("testDomain1");
    dom2 = DomainStorage("testDomain2", {"a", "b", "c", "d"},
                         [](const std::any &val) { return std::any_cast<const char *>(val); });
    dom3 = DomainStorage(
        "testDomain3", createDomain1, [](const std::any &val) { return std::any_cast<const char *>(val); },
        [](size_t, const std::any &) { return true; });
}

TEST_F(TestDomain, constructor) {
    EXPECT_EQ("testDomain1", dom1.getName());
    EXPECT_EQ(dom1.size(), 0);

    EXPECT_EQ("testDomain2", dom2.getName());
    EXPECT_EQ(dom2.size(), 4);
    EXPECT_EQ(dom2.getValues().size(), 4);
    EXPECT_EQ(std::any_cast<const char *>(dom2.getValues().at(0)), "a");
    EXPECT_EQ(std::any_cast<const char *>(dom2.getValues().at(3)), "d");

    EXPECT_EQ("testDomain3", dom3.getName());
    EXPECT_EQ(dom3.size(), 0);
    EXPECT_EQ(dom3.getValues().size(), 0);
}

TEST_F(TestDomain, addValues) {
    dom1.addValues({1.0}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); });
    EXPECT_EQ(dom1.size(), 1);
    EXPECT_EQ(std::any_cast<double>(dom1.getValues().at(0)), 1.0);

    dom2.addValues({"e", "f"}, [](const std::any &val) { return std::any_cast<const char *>(val); });
    EXPECT_EQ(dom2.size(), 2);
    EXPECT_EQ(std::any_cast<const char *>(dom2.getValues().at(0)), "e");
    EXPECT_EQ(std::any_cast<const char *>(dom2.getValues().at(1)), "f");
}

TEST_F(TestDomain, dynamicDomain) {
    dom3.updateDynamicDomain(0);
    EXPECT_EQ(dom3.size(), 2);
    EXPECT_EQ(dom3.getValues().size(), 2);
    EXPECT_EQ(std::any_cast<const char *>(dom3.getValues().at(0)), "a");
    EXPECT_EQ(std::any_cast<const char *>(dom3.getValues().at(1)), "b");

    dom3.reset();
    dom3.updateDynamicDomain(1);
    EXPECT_EQ(dom3.size(), 3);
    EXPECT_EQ(dom3.getValues().size(), 3);
    EXPECT_EQ(std::any_cast<const char *>(dom3.getValues().at(0)), "c");
    EXPECT_EQ(std::any_cast<const char *>(dom3.getValues().at(1)), "d");
    EXPECT_EQ(std::any_cast<const char *>(dom3.getValues().at(2)), "e");

    dom3.reset();
    dom3.updateDynamicDomain(2);
    EXPECT_EQ(dom3.size(), 1);
    EXPECT_EQ(dom3.getValues().size(), 1);
    EXPECT_EQ(std::any_cast<const char *>(dom3.getValues().at(0)), "f");
}

TEST_F(TestDomain, iterator) {
    std::vector<std::any>::iterator ptr;
    std::vector<std::string> expRes{"a", "b", "c", "d"};
    size_t idx{0};
    for (ptr = dom2.begin(); ptr < dom2.end(); ptr++) {
        EXPECT_EQ(std::any_cast<const char *>(*ptr), expRes.at(idx));
        ++idx;
    }
}