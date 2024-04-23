import rvm

import pytest

class DomainCollectionFixture:
    def __init__(self):
        self.dom1 = rvm.DomainStorage("testDomain1");
        self.dom2 = rvm.DomainStorage("testDomain2", ["a", "b", "c", "d"])
        self.domC1 = rvm.DomainCollection(self.dom1)
        self.domC2 = rvm.DomainCollection([self.dom1, self.dom2])

@pytest.fixture
def dc():
    return DomainCollectionFixture()

def test_init(dc):
    assert len(dc.domC1) == 1
    assert len(dc.domC2) == 2

def test_add_domain(dc):
    dc.domC1.add_domain(dc.dom2)

    assert len(dc.domC1) == 2

def test_remove_domain(dc):
    dc.domC1.remove_domain("testDomain1")
    assert len(dc.domC1) == 0

    testdom2 = dc.domC2.get_domain("testDomain2")
    assert testdom2.name == "testDomain2"
    # EXPECT_THROW(domC1.getDomain("testDomain1"), std::runtime_error);

