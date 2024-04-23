import pytest

import rvm

@pytest.mark.parametrize("monitor", [rvm.Hydra, rvm.Hydra2])
def test_F4_1(monitor):
    formula = rvm.Parser.parse("G((a)S[1, inf](b))", "F4_1")

    trace = rvm.create_trace(
        {"a", "b", "c"}, 
            [[],         ["a"],      ["a"], ["a"],      [],         ["a"],      ["b"], ["a"], ["a"],      ["a", "b"],
             ["a", "b"], ["a", "b"], ["a"], ["a", "b"], ["a", "b"], ["a", "b"], ["a"], ["a"], ["a", "b"], ["a"],

             ["a", "b"], ["a", "b"], ["b"], ["a", "b"], ["a"],      ["a"],      ["b"], ["b"], ["a", "b"], [],
             ["b"],      ["a", "b"]],
            [0,  0,  0,  0,  4,  4,  4,  4,  8,  8,  8,  8,  9,  9,  9,  9,
             10, 10, 10, 10, 13, 13, 13, 13, 15, 15, 15, 15, 16, 16, 16, 16]
        )

    mon = monitor([formula])
    res = mon.evaluate(trace)["F4_1"]

    expected = [
        rvm.Verdict(0, False), rvm.Verdict(0, False), rvm.Verdict(0, False),  rvm.Verdict(0, False),  rvm.Verdict(4, False), rvm.Verdict(4, False),  rvm.Verdict(4, False),  rvm.Verdict(4, False),
        rvm.Verdict(8, True),  rvm.Verdict(8, True),  rvm.Verdict(8, True),   rvm.Verdict(8, True),   rvm.Verdict(9, True),  rvm.Verdict(9, True),   rvm.Verdict(9, True),   rvm.Verdict(9, True),
        rvm.Verdict(10, True), rvm.Verdict(10, True), rvm.Verdict(10, True),  rvm.Verdict(10, True),  rvm.Verdict(13, True), rvm.Verdict(13, True),  rvm.Verdict(13, False), rvm.Verdict(13, False),
        rvm.Verdict(15, True), rvm.Verdict(15, True), rvm.Verdict(15, False), rvm.Verdict(15, False), rvm.Verdict(16, True), rvm.Verdict(16, False), rvm.Verdict(16, False), rvm.Verdict(16, False)
    ]

    for v_actual, v_expected in zip(res, expected):
        assert v_actual == v_expected

@pytest.mark.parametrize("monitor", [rvm.EventConsumingLTL, rvm.MTLEventConsuming, rvm.DynamicProgrammingLTL, rvm.BttFsm, rvm.MtFsm])
def test_G1(monitor):
    formula = rvm.Parser.parse("G(b -> (X(a)))", "G1")

    trace = rvm.create_trace({"a", "b", "c"}, [["b"], ["a"], ["b"]])

    mon = monitor([formula])
    res = mon.evaluate(trace)[formula.id]

    assert len(res) == 1
    assert res[0] == False

@pytest.mark.parametrize("monitor", [rvm.Hydra, rvm.Hydra2])
def test_G1_hydra(monitor):
    formula = rvm.Parser.parse("G(b -> (X(a)))", "G1")

    trace = rvm.create_trace({"a", "b", "c"}, [["b"], ["a"], ["b"]])

    mon = monitor([formula])
    res = mon.evaluate(trace)[formula.id]

    assert len(res) == 3
    assert res[0].value == True
    assert res[1].value == True
    assert res[2] == rvm.Verdict(2, False)

def make_event_list(size):
    return [rvm.Event(i, i == size - 1) for i in range(size)]

def one_higher(time_step: int, fst_val: int, snd_val: int):
    return fst_val > time_step or snd_val > time_step

def test_universal_monitor():
    formula = rvm.Parser.parse("A x in D0, y in D1. one_higher(x, y)", "R1")

    dom1 = rvm.DomainStorage("D0", [1,2,3,4,5,6])
    dom2 = rvm.DomainStorage("D1", [3,4,5])

    event_list = make_event_list(5)
    trace = rvm.Trace(event_list, [rvm.Function(one_higher)])

    mon = rvm.Hydra2([formula], domains=[dom1, dom2])
    res = mon.evaluate(trace)["R1"]

    assert res[0].value == True
    assert res[1].value == True
    assert res[2].value == True
    assert res[3].value == False
    assert res[4].value == False

def test_universal_monitor_dynamic_domain():
    formula = rvm.Parser.parse("A x in D0, y in D1. one_higher(x, y)", "R1")

    def createDomain2(step_number):
        return [1, 2, 3, 4, 5, 6]

    dom1 = rvm.DomainStorage("D0", createDomain2)
    dom2 = rvm.DomainStorage("D1", [3,4,5])

    event_list = make_event_list(5)
    trace = rvm.Trace(event_list, [rvm.Function(one_higher)])

    mon = rvm.Hydra2([formula], domains=[dom1, dom2])
    res = mon.evaluate(trace)["R1"]

    assert res[0].value == True
    assert res[1].value == True
    assert res[2].value == True
    assert res[3].value == False
    assert res[4].value == False
