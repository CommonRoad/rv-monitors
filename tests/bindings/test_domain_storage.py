import rvm


def test_domain_storage_1():
    dom1 = rvm.DomainStorage("testDomain1")

    assert "testDomain1" == dom1.name
    assert len(dom1) == 0

    assert list(iter(dom1)) == list(dom1.values())


def test_domain_storage_2():
    dom2 = rvm.DomainStorage("testDomain2", ["a", "b", "c", "d"])

    assert "testDomain2" == dom2.name
    assert len(dom2) == 4
    values = list(dom2.values())
    assert len(values) == 4
    assert values[0] == "a"
    assert values[3] == "d"

    assert list(iter(dom2)) == values
    assert len(list(iter(dom2))) == 4


def test_domain_storage_3():
    def createDomain1(step_number: int):
        if step_number == 0:
            return ["a", "b"]
        elif step_number == 1:
            return ["c", "d", "e"]
        else:
            return ["f"]

    dom3 = rvm.DomainStorage("testDomain3", createDomain1)

    assert "testDomain3" == dom3.name
    assert len(dom3) == 0
    assert len(list(dom3.values())) == 0
    assert list(iter(dom3)) == list(dom3.values())

    dom3.update_dynamic_domain(0)
    assert len(dom3) == 2
    values = list(dom3.values())
    assert len(values) == 2
    assert values[0] == "a"
    assert values[1] == "b"
    assert list(iter(dom3)) == values

    dom3.reset()
    dom3.update_dynamic_domain(1)
    assert len(dom3) == 3
    values = list(dom3.values())
    assert len(values) == 3
    assert values[0] == "c"
    assert values[1] == "d"
    assert values[2] == "e"
    assert list(iter(dom3)) == values

    dom3.reset()
    dom3.update_dynamic_domain(2)
    assert len(dom3) == 1
    values = list(dom3.values())
    assert len(values) == 1
    assert values[0] == "f"
    assert list(iter(dom3)) == values
