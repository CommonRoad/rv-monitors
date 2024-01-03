#include "test_function.h"

bool dummyFunction(size_t time, const std::string &value1, double value2) {
    return time == 1 and value1 == "test" and value2 == 1.1;
}

std::tuple<size_t, const std::string, double> dummyArguments(const std::vector<DomainValue> &args) {
    size_t val1{std::any_cast<size_t>(args.at(0).getValue())};
    std::string val2{std::any_cast<std::string>(args.at(1).getValue())};
    double val3{std::any_cast<double>(args.at(2).getValue())};
    return {val1, val2, val3};
}

void TestFunction::SetUp() {
    testf = dummyFunction;
    test_f = std::make_shared<Function<bool, size_t, std::string, double>>(
        Function<bool, size_t, std::string, double>("testname", {"bool", "string", "double"}, testf, dummyArguments));
}

TEST_F(TestFunction, FunctionalExecution) {
    EXPECT_TRUE(test_f->evaluateFunction(1, "test", 1.1));
    EXPECT_FALSE(test_f->evaluateFunction(1, "false", 1.0));
}

TEST_F(TestFunction, GetName) { EXPECT_EQ("testname", test_f->getName()); }

TEST_F(TestFunction, GetFunctionWithoutArgs) {
    size_t val1_1{1};
    size_t val1_2{1};
    std::string val2_1{"test"};
    std::string val2_2{"false"};
    double val3_1{1.1};
    double val3_2{1.0};
    std::shared_ptr<IFunction<bool>> tmp{test_f};
    EXPECT_TRUE(
        tmp->getFunction({DomainValue("val1", val1_1), DomainValue("val2", val2_1), DomainValue("val3", val3_1)})());
    EXPECT_FALSE(
        tmp->getFunction({DomainValue("val1", val1_2), DomainValue("val2", val2_2), DomainValue("val3", val3_2)})());
}
