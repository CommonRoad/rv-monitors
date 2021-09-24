#include "hydra.h"
//#include "visitors.h"
#include "../../rewriting/mtl_rewrite_visitor.h"
#include <iostream>

std::vector<Verdict> Hydra::evaluate(Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return {Verdict{0, false}};
    }
    // Copy formula before working on it
    auto formulaCopy = formula.deepCopy();
    std::vector<Verdict> result;
    result.reserve(trace.getLength());

    MtlRewriteVisitor rw;
    //  MonitorVisitor mv;
    // fmla->accept(mv);
    rw.rewrite(formulaCopy);
    //  Monitor *mon = mv.get_mon();
    //  TimePoint tp;

    for (size_t i{0}; i < trace.getLength(); ++i) {
        //       BooleanVerdict v = mon->step();
        //       tp.update(v.ts);
        //        if (v.b == TRUE)
        //            printf("%d\n", tp.tp);
        //        else if (v.b != UNRESOLVED)
        //            printf("%d:%d %s\n", tp.ts, tp.off, (v.b == FALSE ? "false" : "true"));
    }
}
std::string Hydra::to_string() const { "TODO"; }
