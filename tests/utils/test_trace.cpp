#include "test_trace.h"

TEST(TestTrace, traceClearance) {
    std::shared_ptr<Trace> trace = Utils::createTrace(
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

TEST(TestTrace, traceRemoveAndExtend) {
    std::shared_ptr<Trace> trace1 = Utils::createTrace(
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

    std::shared_ptr<Trace> trace2 = Utils::createTrace(
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

    std::shared_ptr<Trace> trace3 = Utils::createTrace(
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
