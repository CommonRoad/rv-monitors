#include "test_trace.h"

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"

TEST(TestTrace, traceClearance) {
    const std::shared_ptr<Trace> trace = Utils::createTrace(
        {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
         "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
         "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
         "in_single_lane('DALL_1000')"},
        {
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {},
        },
        std::vector<size_t>{0, 1, 2, 3});
    EXPECT_EQ(trace->getEvent(0).getEventMap().size(), 7);
    EXPECT_EQ(trace->getEvent(1).getEventMap().size(), 7);
    EXPECT_EQ(trace->getEvent(2).getEventMap().size(), 7);
    EXPECT_EQ(trace->getEvent(3).getEventMap().size(), 7);

    trace->cleanTrace({"1000'"}, 0);
    EXPECT_EQ(trace->getEvent(0).getEventMap().size(), 2);
    EXPECT_EQ(trace->getEvent(1).getEventMap().size(), 2);
    EXPECT_EQ(trace->getEvent(2).getEventMap().size(), 2);
    EXPECT_EQ(trace->getEvent(3).getEventMap().size(), 2);
}

TEST(TestTrace, toCSV) {
    const std::shared_ptr<Trace> trace = Utils::createTrace(
        {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
         "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
         "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
         "in_single_lane('DALL_1000')"},
        {
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {},
        },
        std::vector<size_t>{0, 1, 2, 3});
    const auto csv = trace->to_csv();
    EXPECT_EQ(csv.size(), 450);
}

TEST(TestTrace, traceRemoveAndExtend) {
    const std::shared_ptr<Trace> trace1 = Utils::createTrace(
        {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
         "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
         "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
         "in_single_lane('DALL_1000')"},
        {
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {},
        },
        std::vector<size_t>{0, 1, 2, 3});
    EXPECT_EQ(trace1->getLength(), 4);
    trace1->removeAndExtend(2, 3, 0);
    EXPECT_EQ(trace1->getLength(), 2);

    const std::shared_ptr<Trace> trace2 = Utils::createTrace(
        {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
         "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
         "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
         "in_single_lane('DALL_1000')"},
        {
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {},
        },
        std::vector<size_t>{0, 1, 2, 3});
    trace2->removeAndExtend(0, 3, 0);
    EXPECT_EQ(trace2->getLength(), 4);

    const std::shared_ptr<Trace> trace3 = Utils::createTrace(
        {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
         "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
         "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
         "in_single_lane('DALL_1000')"},
        {
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {"in_same_lane('DALL_1000', 'DALL_1007')", "in_same_lane('DALL_1000', 'DALL_1006')",
             "in_same_lane('DALL_1000', 'DALL_1005')", "lane_based_orientation_similar('DALL_1000', 'DALL_1007)",
             "lane_based_orientation_similar('DALL_1006', 'DALL_1007)", "in_single_lane('DALL_1001')",
             "in_single_lane('DALL_1000')"},
            {},
        },
        std::vector<size_t>{0, 1, 2, 3});
    trace3->removeAndExtend(2, 3, 2);
    EXPECT_EQ(trace3->getLength(), 4);
    EXPECT_EQ(trace3->getTrace().at(0).getTimeStep(), 2);
    EXPECT_EQ(trace3->getTrace().at(3).getTimeStep(), 5);
}

bool a2(size_t dummy) {
    switch (dummy) {
    case 0:
        return true;
    case 1:
        return false;
    case 2:
        return true;
    default:
        return false;
    }
}

std::tuple<size_t> aArgs2(const std::vector<DomainValue> &values) {
    return {std::any_cast<size_t>(values.at(0).getValue())};
}

TEST(TestTrace, traceMissingSteps) {
    const std::vector<std::shared_ptr<IFunction<bool>>> test{
        {std::make_shared<Function<bool, size_t>>(Function<bool, size_t>("a", {"size_t"}, a2, aArgs2))}};

    std::vector<Event> eventList;
    for (size_t timeStepIdx{0}; timeStepIdx <= 10; timeStepIdx += 4) {
        eventList.emplace_back(timeStepIdx, timeStepIdx == 10);
    }
    const auto trace = std::make_shared<Trace>(
        Trace(eventList, test,
              ConstantCollection({ConstantValue{"dummy", "dummy", [](const std::any &val) { return "dummy"; }}})));
    EXPECT_EQ(trace->getTrace().size(), 3);

    Formula formula = Parser::parse("a('dummy')");
    formula.setId("R1");
    auto mon{PredicateMonitor(formula, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(trace->getTrace().size(), 3);
    EXPECT_EQ(mon.concreteStep(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.concreteStep(0, 1).value, false);
    EXPECT_EQ(mon.step(0, 1).value, false);

    trace->insertEvents(2, 10, 4);
    EXPECT_EQ(trace->getTrace().size(), 6);
    mon.reset();
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.concreteStep(0, 1).value, true);
    EXPECT_EQ(mon.step(0, 1).value, true);
    EXPECT_EQ(mon.concreteStep(0, 2).value, false);
    EXPECT_EQ(mon.step(0, 2).value, false);

    trace->insertEvents(1, 10, 2);
    mon.reset();
    EXPECT_EQ(trace->getTrace().size(), 11);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.concreteStep(0, 1).value, false);
    EXPECT_EQ(mon.step(0, 1).value, false);
    EXPECT_EQ(mon.concreteStep(0, 2).value, true);
    EXPECT_EQ(mon.step(0, 2).value, true);
    EXPECT_EQ(mon.concreteStep(0, 3).value, false);
    EXPECT_EQ(mon.step(0, 3).value, false);
    EXPECT_EQ(mon.concreteStep(0, 4).value, false);
    EXPECT_EQ(mon.step(0, 4).value, false);
}
