//
// Created by Sebastian Maierhofer and Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "mtl_event_consuming_visitor.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/expression_tree/binary/bool/equivalence.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/binary/temporal/ltlf/release.h"
#include "monitors/expression_tree/binary/temporal/ltlf/until.h"
#include "monitors/expression_tree/binary/temporal/ptlf/since.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/deriving_operator.h"
#include "monitors/expression_tree/unary/event_operator.h"
#include "monitors/expression_tree/unary/resolving_operator.h"
#include "monitors/expression_tree/unary/temporal/ltlf/future.h"
#include "monitors/expression_tree/unary/temporal/ltlf/globally.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_strong.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_weak.h"
#include "monitors/expression_tree/unary/temporal/ptlf/historically.h"
#include "monitors/expression_tree/unary/temporal/ptlf/once.h"
#include "monitors/expression_tree/unary/temporal/ptlf/previously.h"
#include "monitors/parser/parser.h"
#include <iostream>

void MTLEventConsumingVisitor::consumeEvent(Formula &formula, Trace &trace, size_t index) {
    std::cout<<index<<" - Formula Before: "<<formula.getExpr()->to_string()<<std::endl;
    std::shared_ptr<EventOperator> eventOperator = std::make_shared<EventOperator>(formula.getExpr(), trace, index);
    formula.setExpr(eventOperator->accept(*this, trace, index));

    std::cout<<index<<" - Formula After: "<<formula.getExpr()->to_string()<<std::endl;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitAtomic(Atomic &op, Trace &trace, size_t index) {
    std::cout<<"Visit Atomic: "<<op.to_string()<<std::endl;
    return std::make_shared<Atomic>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitPredicate(Predicate &op, Trace &trace, size_t index) {
    std::cout<<"Visit Predicate: "<<op.to_string()<<std::endl;
    return std::make_shared<Predicate>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitConstant(Constant &op, Trace &trace, size_t index) {
    std::cout<<"Visit Constant: "<<op.to_string()<<std::endl;
    return std::make_shared<Constant>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitAnd(And &op, Trace &trace, size_t index) {
    std::cout<<"Visit AND: "<<op.to_string()<<std::endl;
    visitExpression(op, trace, index);
    std::cout<<"After Visiting AND: "<<op.to_string()<<std::endl;
    return std::make_shared<And>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitOr(Or &op, Trace &trace, size_t index) {
    std::cout<<"Visit OR: "<<op.to_string()<<std::endl;
    visitExpression(op, trace, index);
    std::cout<<"After Visiting OR: "<<op.to_string()<<std::endl;
    return std::make_shared<Or>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitXor(Xor &op, Trace &trace, size_t index) {
    std::cout<<"Visit XOR: "<<op.to_string()<<std::endl;
    visitExpression(op, trace, index);
    std::cout<<"After Visiting XOR: "<<op.to_string()<<std::endl;
    return std::make_shared<Xor>(op);

}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitImplication(Implication &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Implication>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitEquivalence(Equivalence &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Equivalence>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitNot(Not &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Not>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitFuture(Future &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Future>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitGlobally(Globally &op, Trace &trace, size_t index) {
    // visitExpression(op, trace, index);
    visitExpression(op, trace, index);
    return std::make_shared<Globally>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitHistorically(Historically &op, Trace &trace, size_t index) {
    //visitExpression(op, trace, index);
    visitExpression(op, trace, index);
    return std::make_shared<Historically>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitNextStrong(NextStrong &op, Trace &trace, size_t index) {
    std::cout<<"Visit Next Strong: "<<op.to_string()<<std::endl;
    visitExpression(op, trace, index);
    return std::make_shared<NextStrong>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitNextWeak(NextWeak &op, Trace &trace, size_t index) {
    std::cout<<"Visit Next Weak: "<<op.to_string()<<std::endl;
    visitExpression(op, trace, index);
    return std::make_shared<NextWeak>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitOnce(Once &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Once>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitPreviously(Previously &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Previously>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitRelease(Release &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Release>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitUntil(Until &op, Trace &trace, size_t index) {
    std::cout<<"Visit Until: "<<op.to_string()<<std::endl;
    visitExpression(op, trace, index);
    return std::make_shared<Until>(op.getLeft(), op.getRight(), op.getInterval());
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitSince(Since &op, Trace &trace, size_t index) {
    std::cout<<"Visit Since: "<<op.to_string()<<std::endl;
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Since>(op.getLeft(), op.getRight(), op.getInterval());
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitDerivingOperator(DerivingOperator &op, Trace &trace,
                                                                            size_t index) {
    std::cout<<"visit Deriving Operator: "<<op.to_string()<<std::endl;
    auto ret = std::make_shared<DerivingOperator>(op);
    return ret;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitResolvingOperator(ResolvingOperator &op, Trace &trace,
                                                                             size_t index) {
    return std::make_shared<ResolvingOperator>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitEventOperator(EventOperator &op, Trace &trace,
                                                                         size_t index) {

    std::cout<<"["<<index<<"] --- Visit Event Operator: "<<op.getLeft()->to_string()<<std::endl;

    std::shared_ptr<Expression> ret;
    // Apply eventMap to Atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(op.getLeft())) {
        std::cout << index << ": VisitEventOperator [ATOMIC].getLeft(): " << op.getLeft()->to_string() << std::endl;
        ret = std::make_shared<Constant>(trace.getEvent(index).getValue(*atomic));
    }
    // Constant
    else if (auto constant = std::dynamic_pointer_cast<Constant>(op.getLeft())) {
        std::cout<<index<<": VisitEventOperator [CONSTANT].getLeft(): "<<op.getLeft()->to_string()<<std::endl;
        ret = std::make_shared<Constant>(constant->getValue());
        std::cout<<"After visiting Constant: "<<ret->to_string()<<std::endl;
    }
    // Predicate
    else if (auto predicate = std::dynamic_pointer_cast<Predicate>(op.getLeft())) {
        std::cout<<index<<": VisitEventOperator [PREDICATE].getLeft(): "<<op.getLeft()->to_string()<<std::endl;
        ret = std::make_shared<Constant>(trace.getEvent(index).getValue(*predicate));
    }
    // Deriving //TODO Might delete this
    else if (auto deriving = std::dynamic_pointer_cast<DerivingOperator>(op.getLeft())){
        std::cout<<index<<": VisitEventOperator [Deriving].getLeft(): "<<op.getLeft()->to_string()<<std::endl;
        ret = std::make_shared<EventOperator>(deriving->getLeft(), trace, index);
        // ret = std::make_shared<DerivingOperator>(deriving->getLeft(), trace, index);
    }

    // And
    else if (auto andExpr = std::dynamic_pointer_cast<And>(op.getLeft())) {
        std::cout<<index<<": VisitEventOperator [AND].getLeft(): "<<op.getLeft()->to_string()<<std::endl;
        And::SetType_T newExprSet;
        for (const auto &itr : *andExpr) {
            auto derivingFutureExpr = std::make_shared<EventOperator>(itr, trace, index);
            std::cout<<"Deriving Future Expr: "<<derivingFutureExpr->to_string()<<std::endl;
            newExprSet.emplace(derivingFutureExpr);
        }
        ret = std::make_shared<And>(newExprSet);
        std::cout<<index<<": After VisitEventOperator [AND] ret: "<<ret->to_string()<<std::endl;
    }
    // Or
    else if (auto orExpr = std::dynamic_pointer_cast<Or>(op.getLeft())) {
        std::cout<<index<<": VisitEventOperator [OR].getLeft(): "<<op.getLeft()->to_string()<<std::endl;
        Or::SetType_T newExprSet;
        for (const auto &itr : *orExpr) {
            auto derivingExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(derivingExpr);
        }
        ret = std::make_shared<Or>(newExprSet);
    }
    // Xor
    else if (auto xorExpr = std::dynamic_pointer_cast<Xor>(op.getLeft())) {
        std::cout<<index<<": VisitEventOperator [XOR].getLeft(): "<<op.getLeft()->to_string()<<std::endl;
        Xor::SetType_T newExprSet;
        for (const auto &itr : *xorExpr) {
            auto derivingExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(derivingExpr);
        }
        ret = std::make_shared<Xor>(newExprSet);

    }
    // Weak Next
    else if (auto nextWeak = std::dynamic_pointer_cast<NextWeak>(op.getLeft())) {
        std::cout<<index<<": VisitEventOperator [NEXT WEAK].getLeft(): "<<op.getLeft()->to_string()<<std::endl;
        if (trace.getEvent(index).isTerminal() or (not(static_cast<unsigned long>(nextWeak->getInterval()->getStart()) + index <= index + 1 and
                                                       static_cast<unsigned long>(nextWeak->getInterval()->getEnd()) + index >= index + 1))) {
            ret = std::make_shared<Constant>(true);
        }
        else {
            ret = nextWeak->getLeft();
            // ret = std::make_shared<EventOperator>(nextStrong->getLeft(), trace, index + 1);
        }

        /*
        if (!trace.getEvent(index).isTerminal()) { // TODO check if within interval
            ret = nextWeak->getLeft();
        } else {
            ret = std::make_shared<Constant>(true);
        }
         */
    }
    // Strong Next
    else if (auto nextStrong = std::dynamic_pointer_cast<NextStrong>(op.getLeft())) {
        // THe search has to continue also for other indexes
        std::cout<<index<<": VisitEventOperator [NEXT STRONG].getLeft(): "<<op.getLeft()->to_string()<<std::endl;
        std::cout<<"Start: "<<nextStrong->getInterval()->getStart()<<" - End: "<<nextStrong->getInterval()->getEnd()<<std::endl;

        if (trace.getEvent(index).isTerminal() /* or
             (not(static_cast<unsigned long>(nextStrong->getInterval()->getStart()) + index <= index + 1 and
                 static_cast<unsigned long>(nextStrong->getInterval()->getEnd()) + index >= index + 1)
             ) */ ) {
         ret = std::make_shared<Constant>(false);
        }
        else {
            if (nextStrong->getInterval()->containsZero()){
                /// do some actions
                Or::SetType_T orSet{};
                auto strongExpr = std::make_shared<EventOperator>(nextStrong->getLeft(), trace, index); //The Expression in StrongNext will be evaluated
                orSet.emplace(strongExpr);
                nextStrong->reduceInterval();
                orSet.emplace(nextStrong);
                ret = std::make_shared<Or>(orSet);
            }
            else
                ret = std::make_shared<Constant>(false);
        }

    }
    // Until
    else if (auto until = std::dynamic_pointer_cast<Until>(op.getLeft())) {
        //TODO Needs to be done
//        bool stop_checking = false; // no need for this rethink it any other way
//        std::cout<<"contains zero: "<<until->getInterval()->containsZero()<<std::endl;
//        if (!until->getInterval()->getContainsZero(index, stop_checking)){ // find another way for this
//            ret = std::make_shared<Constant>(false);
//        }
//        else {
            // stop_checking = true;
            // }

        std::cout<<index<<": VisitEventOperator [UNTIL].getLeft(): "<<op.getLeft()->to_string()<<std::endl;

        if (!until->getInterval()->containsZero()){
            ret = std::make_shared<Constant>(false);
        }
        else{
//            Or::SetType_T bigOrSet{};
            And::SetType_T andSet{};
            if (trace.getEvent(index).isTerminal()){
                auto terminalEval = std::make_shared<EventOperator>(until->getRight(), trace, index);
//                andSet.emplace(terminalEval);
                ret = terminalEval;
            }
            else {
                size_t trace_length_remaining = trace.getLength() - index; // maybe + 1 ?
                auto reps = static_cast<size_t>(until->getInterval()->getEnd()) + 1;
                auto min = (trace_length_remaining < reps) ? trace_length_remaining : reps;
                std::cout<<"Min [UNTIL] : "<<min<<std::endl;
                Or::SetType_T orSet{};
                for (size_t i=0; i<min; ++i){ //check reduction
                    And::SetType_T exprAndSet{};
                    size_t j=0;
                    while (j<min-1-i){ // could also state this relative to i
                        auto evalExpr1 = std::make_shared<EventOperator>(until->getLeft(), trace, index /* + j */);
                        auto evalExpr1sol = evalExpr1->accept(*this, trace, index+j);
                        exprAndSet.emplace(evalExpr1sol);
                        j = j + 1;
                    }
                    auto evalExpr2 = std::make_shared<EventOperator>(until->getRight(), trace,  index /* + j */);
                    auto evalExpr2sol = evalExpr2->accept(*this, trace, index+j);
                    exprAndSet.emplace(evalExpr2sol);
                    auto exprAnd = std::make_shared<And>(exprAndSet);
                    std::cout<<i<<": ands->: "<<exprAnd->to_string()<<std::endl;
                    orSet.emplace(std::make_shared<And>(exprAndSet));
                }
                andSet.emplace(std::make_shared<Or>(orSet));
//                bigOrSet.emplace(std::make_shared<Or>(orSet));
                andSet.emplace(until);
//                bigOrSet.emplace(until);
                ret = std::make_shared<And>(andSet);
//                ret = std::make_shared<Or>(bigOrSet);
            }
        }


//        ret = std::make_shared<Or>(orSet);

        std::cout<<index<<": After VisitEventOperator [UNTIL] ret: "<<ret->to_string()<<std::endl;


    }
    // Since
    else if (auto since = std::dynamic_pointer_cast<Since>(op.getLeft())) {
        //TODO Needs to be done - Check wihin interval
        std::cout<<index<<": VisitEventOperator [SINCE].getLeft(): "<<op.getLeft()->to_string()<<std::endl;

        if (!since->getInterval()->containsZero()){
            ret = std::make_shared<Constant>(false);
        }
        else{
            And::SetType_T andSet{};
            size_t trace_length_remaining = index + 1;
            auto reps = static_cast<size_t>(since->getInterval()->getEnd()) + 1;
            auto min = (reps < trace_length_remaining) ? reps : trace_length_remaining;
            std::cout<<"Min [SINCE] : "<<min<<std::endl;
            Or::SetType_T orSet{};
            for (size_t i=0; i<min; ++i){ //check reduction
                And::SetType_T exprAndSet{};
                size_t j=0;
                while (j<min-1-i){ // could also state this relative to i
                    auto evalExpr1 = std::make_shared<EventOperator>(since->getLeft(), trace, index /* - j */);
                    auto evalExpr1sol = evalExpr1->accept(*this, trace, index - j);
                    exprAndSet.emplace(evalExpr1sol);
                    j = j + 1;
                }
                auto evalExpr2 = std::make_shared<EventOperator>(since->getRight(), trace, index /* - j */);
                auto evalExpr2sol = evalExpr2->accept(*this, trace, index - j);
                exprAndSet.emplace(evalExpr2sol);
                auto exprAnd = std::make_shared<And>(exprAndSet);
                std::cout<<i<<": ands->: "<<exprAnd->to_string()<<std::endl;
                orSet.emplace(std::make_shared<And>(exprAndSet));
            }
            if (!trace.getEvent(index).isTerminal()) {
                andSet.emplace(std::make_shared<Or>(orSet));
                andSet.emplace(since);
                ret = std::make_shared<And>(andSet);
            }
            else {
                ret = std::make_shared<Or>(orSet);
            }
        }

        std::cout<<index<<" - After Event Operator [SINCE]: "<<ret->to_string()<<std::endl;

        //auto resolvingExpr = ResolvingOperator(since, trace, index);
        //ret = std::make_shared<DerivingOperator>(
           // DerivingOperator(resolvingExpr.accept(*this, trace, index), trace, index));
    }
    // Globally
    else if (auto globally = std::dynamic_pointer_cast<Globally>(op.getLeft())) {
        std::cout<<index<<"Visit Event Operator [GLOBALLY].getLeft():  "<<op.getLeft()->to_string()<<std::endl;
        // not(static_cast<unsigned long>(nextStrong->getInterval()->getStart()) + index <= index + 1 and static_cast<unsigned long>(nextStrong->getInterval()->getEnd()) + index >= index + 1))

        if (!trace.getEvent(index).isTerminal()) {
            std::cout<<"Globally get left: "<<globally->getLeft()->to_string()<<std::endl;
            And::SetType_T newSet;
            auto eventOperatorExpr = std::make_shared<EventOperator>(globally->getLeft(), trace, index);
            std::cout<<"Event Op Expr: "<<eventOperatorExpr->to_string()<<std::endl;
            newSet.emplace(eventOperatorExpr);
            globally->reduceInterval();
            newSet.emplace(globally);
            ret = std::make_shared<And>(newSet);
        }
        else {
            ret = std::make_shared<EventOperator>(globally->getLeft(), trace, index);
            std::cout<<"ret="<<ret->to_string()<<std::endl;
        }
    }
    // Future
    else if (auto future = std::dynamic_pointer_cast<Future>(op.getLeft())) {
        std::cout<<index<<"Visit Event Operator [FUTURE].getLeft():  "<<op.getLeft()->to_string()<<std::endl;

//        if (!trace.getEvent(index).isTerminal() and future->getInterval()->getEnd() != 0) {
//            Or::SetType_T newSet;
//            auto eventOperatorExpr = std::make_shared<EventOperator>(future->getLeft(), trace, index);
//            newSet.emplace(eventOperatorExpr);
//            future->reduceInterval();
//            newSet.emplace(future);
//            ret = std::make_shared<Or>(newSet);
//        }
//        else {
//            ret = std::make_shared<EventOperator>(future->getLeft(), trace, index);
//        }

        if (!future->getInterval()->containsZero()){
            ret = std::make_shared<Constant>(false);
        }
        else {
            And::SetType_T andSet{};
            if (trace.getEvent(index).isTerminal()){
                auto terminalEval = std::make_shared<EventOperator>(future->getLeft(), trace, index);
                ret = terminalEval;
            }
            else {
                size_t trace_length_remaining = trace.getLength() - index;
                auto reps = static_cast<size_t>(future->getInterval()->getEnd()) + 1;
                auto min = (trace_length_remaining < reps) ? trace_length_remaining : reps;
                std::cout << "Min [Future] : " << min << std::endl;
                Or::SetType_T orSet{};
                for (size_t i = 0; i < min; ++i) {
                    auto evalExpr = std::make_shared<EventOperator>(future->getLeft(), trace, index);
                    auto evalExprSol = evalExpr->accept(*this, trace, index + i);
                    orSet.emplace(evalExprSol);
                }
                andSet.emplace(std::make_shared<Or>(orSet));
                andSet.emplace(future);
                ret = std::make_shared<And>(andSet);
            }
        }
    }
    // Previously
    else if (auto previously = std::dynamic_pointer_cast<Previously>(op.getLeft())) {
        std::cout<<index<<"Visit Event Operator [PREVIOUSLY].getLeft():  "<<op.getLeft()->to_string()<<std::endl;
//        if (index > 0) {
//            auto eventOperatorExpr = std::make_shared<EventOperator>(previously->getLeft(), trace, index - 1);
//            ret = eventOperatorExpr->accept(*this, trace, index - 1);
//        }
//        else {
//            ret = std::make_shared<Constant>(false);
//        }
        if (!previously->getInterval()->containsZero()){
            ret = std::make_shared<Constant>(false);
        }
        else {
            And::SetType_T andSet{};

            size_t trace_length_remaining = index;
            auto reps = static_cast<size_t>(previously->getInterval()->getEnd());
            auto min = (trace_length_remaining < reps) ? trace_length_remaining : reps;
            std::cout << "Min [Previously] : " << min << std::endl;
            Or::SetType_T orSet{};
            for (size_t i = 0; i < min; ++i) {
                auto evalExpr = std::make_shared<EventOperator>(future->getLeft(), trace, index);
                auto evalExprSol = evalExpr->accept(*this, trace, index -1- i);
                orSet.emplace(evalExprSol);
            }
            andSet.emplace(std::make_shared<Or>(orSet));
            andSet.emplace(previously);
            ret = std::make_shared<And>(andSet);

        }
    }
    // Once
    else if (auto once = std::dynamic_pointer_cast<Once>(op.getLeft())) {
        std::cout<<index<<"Visit Event Operator [ONCE].getLeft():  "<<op.getLeft()->to_string()<<std::endl;
        if (once->getInterval()->getEnd() != 0 and index > 0) {
            if (once->getInterval()->getStart() > 0) {
                once->reduceInterval();
                auto his = std::make_shared<EventOperator>(once, trace, index - 1); // evtl. once or newOnce ???
                ret = his->accept(*this, trace, index - 1);
            }
            else {
                Or::SetType_T newSet;
                auto eventOperatorExpr = std::make_shared<EventOperator>(once->getLeft(), trace, index);
                newSet.emplace(eventOperatorExpr);
                Once newOnce = *once;
                newOnce.reduceInterval();
                auto his = std::make_shared<EventOperator>(std::make_shared<Once>(newOnce), trace,
                                                           index - 1); // evtl. once or newOnce ???
                auto his_resolved = his->accept(*this, trace, index - 1);
                newSet.emplace(his_resolved);
                ret = std::make_shared<Or>(newSet);
            }
        }
        else {
            if (once->getInterval()->getStart() > 0)
                ret = std::make_shared<Constant>(false);
            else
                ret = std::make_shared<EventOperator>(once->getLeft(), trace, index);
        }
    }
    // Historically
    else if (auto historically = std::dynamic_pointer_cast<Historically>(op.getLeft())) {
        std::cout<<index<<"Visit Event Operator [HISTORICALLY].getLeft():  "<<op.getLeft()->to_string()<<std::endl;
        if (historically->getInterval()->getEnd() != 0 and index > 0) {
            if (historically->getInterval()->getStart() > 0) {
                historically->reduceInterval();
                auto eventOperatorExpr =
                    std::make_shared<EventOperator>(std::make_shared<Historically>(historically), trace, index - 1);
                ret = eventOperatorExpr->accept(*this, trace, index - 1);
            }
            else {
                And::SetType_T newSet;
                Historically newHistorically = *historically;
                newHistorically.reduceInterval();
                auto eventOperatorExpr =
                    std::make_shared<EventOperator>(std::make_shared<Historically>(newHistorically), trace, index - 1);
                auto sol = eventOperatorExpr->accept(*this, trace, index - 1);
                newSet.emplace(sol);
                newSet.emplace(std::make_shared<EventOperator>(historically->getLeft(), trace, index));
                ret = std::make_shared<And>(newSet);
            }
        }
        else {
            if (historically->getInterval()->getStart() > 0)
                ret = std::make_shared<Constant>(false);
            else
                ret = std::make_shared<EventOperator>(historically->getLeft(), trace, index);
        }
    }

//    else if (auto eventOp = std::dynamic_pointer_cast<EventOperator>(op.getLeft())) {
//        std::cout<<"TADA: "<<eventOp->to_string()<<std::endl;
//        ret = eventOp->accept(*this, trace, index);
//    }

    return ret->accept(*this, trace, index); //PROBLEM ORIGIN
}