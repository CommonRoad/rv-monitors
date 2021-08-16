#include "hydra.h"
#include "visitors.h"
#include <iostream>

struct TimePoint {
    timestamp ts;
    int tp{-1};
    int off;

    void update(timestamp new_ts) {
        if (new_ts > ts || tp == -1) {
            off = 0;
        } else {
            off++;
        }
        ts = new_ts;
        tp++;
    }
};

std::vector<BooleanVerdict> Hydra::evaluate(const Formula &formula, Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return {BooleanVerdict(-1, Boolean::FALSE)};
    }
    // Copy formula before working on it
    auto formulaCopy = formula.deepCopy();

    MonitorVisitor mv;
    // fmla->accept(mv);
    Monitor *mon = mv.get_mon();
    TimePoint tp;

    for (size_t i{0}; i < trace.getLength(); ++i) {
        BooleanVerdict v = mon->step();
        tp.update(v.ts);
        if (v.b == TRUE)
            printf("%d\n", tp.tp);
        else if (v.b != UNRESOLVED)
            printf("%d:%d %s\n", tp.ts, tp.off, (v.b == FALSE ? "false" : "true"));
    }
}
