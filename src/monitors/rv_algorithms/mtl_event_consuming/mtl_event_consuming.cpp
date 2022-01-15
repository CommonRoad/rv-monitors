//
// Created by Sebastian Maierhofer and Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "mtl_event_consuming.h"
#include "../../rewriting/mtl_rewrite_visitor.h"
#include "mtl_event_consuming_visitor.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/unary/event_operator.h"
#include "monitors/rewriting/rewrite_visitor.h"
#include <iostream>
#include <memory>
#include <utility>

MTLEventConsuming::MTLEventConsuming(const Formula &formula) : MonitorInterface((formula)) {}

std::map<std::string, std::vector<Verdict>> MTLEventConsuming::evaluate(Trace &trace) {
    std::map<std::string, std::vector<Verdict>> result;
  // Empty trace
  if (trace.getTrace().empty()) {
      std::cout << "Trace is empty" << std::endl;
      result[formulas.at(0).getId()] = {Verdict{false}};
      return result;
  }
  // Copy formula before working on it
  auto formulaCopy = formulas.at(0).deepCopy();
  std::cout<<"Original Formula: "<<formulaCopy.to_string()<<std::endl;
  MtlRewriteVisitor rw;
  MTLEventConsumingVisitor ev;
  // Rewrite
  rw.rewrite(formulaCopy);
  for (size_t i = 0; i < trace.getLength(); ++i) {
      Formula rArrayFormula = static_cast<Formula>(nullptr);
      Formula dArrayFormula = static_cast<Formula>(nullptr);
      std::cout<<"MTL["<<i<<"] Formula to Work: "<<formulaCopy.to_string()<<std::endl;
      std::cout<<"--- I'm Consuming an Event ---- \n "<<std::endl;
      ev.consumeEvent(formulaCopy, trace, i);
      std::cout<<"----I'm Rewriting --------"<<std::endl;
      rw.rewrite(formulaCopy);
      std::cout<<"\n Formula after Iteration: "<<formulaCopy.to_string()<<"\n"<<std::endl;
      if (formulaCopy.to_string() == "false") {
          break;
      }
      if (formulaCopy.to_string() == "true") {
          break;
      }
  }
  result[formulas.at(0).getId()] = {Verdict{formulaCopy.to_string() == "true"}};
  return result;


}
std::string MTLEventConsuming::to_string() const { return "TODO"; }