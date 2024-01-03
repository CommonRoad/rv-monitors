#include "test_domain_collection.h"

void TestDomainCollection::SetUp() {
    dom1 = DomainStorage("testDomain1");
    dom2 = DomainStorage("testDomain2", {"a", "b", "c", "d"});
    domC1 = DomainCollection(dom1);
    domC2 = DomainCollection({dom1, dom2});
}

TEST_F(TestDomainCollection, constructor) {
    EXPECT_EQ(domC1.size(), 1);
    EXPECT_EQ(domC2.size(), 2);
}

TEST_F(TestDomainCollection, addDomain) {
    domC1.addDomain(dom2);
    EXPECT_EQ(domC1.size(), 2);
}

TEST_F(TestDomainCollection, removeDomain) {
    domC1.removeDomain("testDomain1");
    EXPECT_EQ(domC1.size(), 0);
    EXPECT_EQ(domC2.getDomain("testDomain2").getName(), "testDomain2");
    EXPECT_THROW(domC1.getDomain("testDomain1"), std::runtime_error);
}