import rvm

def test_function1():
    f1 = rvm.Function(lambda time: time == 1, "dummy1")

    assert f1(1) == True
    assert f1(2) == False

def test_function2():
    f2 = rvm.Function(lambda time, value1: time == 1 and value1 == "test", "dummy2")

    assert f2(1, "test") == True
    assert f2(1, "foo") == False

def test_function_name_discovery():
    def dummy2(time, value1):
        return time == 1 and value1 == "test"
    f2 = rvm.Function(dummy2)

    assert f2.name == "dummy2"