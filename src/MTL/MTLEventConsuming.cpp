#include <iostream>
#include <memory>
#include "MTLRewriteVisitor.h"
#include "MTLEventConsuming.h"
#include "MTLResolvingPastVisitor.h"
#include "MTLDerivingFutureVisitor.h"

bool MTLEventConsuming::eventConsuming(const Formula &formula, Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return false;
    }
    // Copy formula before working on it
    std::cout << "Initial formula: " << formula.to_string() << '\n';
    auto formulaCopy = formula.deepCopy();
    MTLRewriteVisitor rw;
    MTLResolvingPastVisitor rp;
    MTLDerivingFutureVisitor df;
    // Rewrite
    rw.rewrite(formulaCopy);
    std::cout << "Rewritten formula: " << formulaCopy.to_string() << '\n';
    for (int i = 0; i < trace.getLength(); ++i) {
        std::cout << '\n' << "Time step:" << i << '\n';
        rp.consumeEvent(formulaCopy, trace, i);
        std::cout << "Resolving past: " << formulaCopy.to_string() << '\n';
        rw.rewrite(formulaCopy);
        std::cout << "Rewriting: " << formulaCopy.to_string() << '\n';
        if (formulaCopy.to_string() == "false") {
//            std::cout << "Formula violated in event " << i << std::endl;
            break;
        }
        if (formulaCopy.to_string() == "true") {
            //std::cout << "Formula became true in event " << i << std::endl;
            break;
        }
        df.consumeEvent(formulaCopy, trace, i);
        std::cout << "Deriving future: " << formulaCopy.to_string() << '\n';
        rw.rewrite(formulaCopy);
        std::cout << "Rewriting: " << formulaCopy.to_string() << '\n';
        if (formulaCopy.to_string() == "false") {
//            std::cout << "Formula violated in event " << i << std::endl;
            break;
        }
        if (formulaCopy.to_string() == "true") {
            //std::cout << "Formula became true in event " << i << std::endl;
            break;
        }

    }
    return formulaCopy.to_string() == "true";
}
