set(LIBRARY_NAME rvmonitors)

set(RV_MONITOR_SRC_FILES
        monitors/expression_tree/expression.cpp
        monitors/expression_tree/interval.cpp
        monitors/expression_tree/temporal_operator.cpp
        monitors/expression_tree/interval.cpp
        monitors/expression_tree/atomic/atomic.cpp
        monitors/expression_tree/atomic/constant.cpp
        monitors/expression_tree/atomic/predicate.cpp
        monitors/expression_tree/binary/binary.cpp
        monitors/expression_tree/binary/bool/equivalence.cpp
        monitors/expression_tree/binary/bool/implication.cpp
        monitors/expression_tree/binary/temporal/ltlf/until.cpp
        monitors/expression_tree/binary/temporal/ltlf/release.cpp
        monitors/expression_tree/binary/temporal/ptlf/since.cpp
        monitors/expression_tree/nary/bool/and.cpp
        monitors/expression_tree/nary/bool/or.cpp
        monitors/expression_tree/nary/bool/xor.cpp
        monitors/expression_tree/unary/deriving_operator.cpp
        monitors/expression_tree/unary/event_operator.cpp
        monitors/expression_tree/unary/resolving_operator.cpp
        monitors/expression_tree/unary/unary.cpp
        monitors/expression_tree/unary/bool/not.cpp
        monitors/expression_tree/unary/temporal/ltlf/future.cpp
        monitors/expression_tree/unary/temporal/ltlf/globally.cpp
        monitors/expression_tree/unary/temporal/ltlf/next_strong.cpp
        monitors/expression_tree/unary/temporal/ltlf/next_weak.cpp
        monitors/expression_tree/unary/temporal/ptlf/historically.cpp
        monitors/expression_tree/unary/temporal/ptlf/once.cpp
        monitors/expression_tree/unary/temporal/ptlf/previously.cpp
        monitors/formula/formula.cpp
        monitors/formula/nff.cpp
        monitors/parser/parser.cpp
        monitors/parser/gen/FormulaBaseListener.cpp
        monitors/parser/gen/FormulaBaseVisitor.cpp
        monitors/parser/gen/FormulaLexer.cpp
        monitors/parser/gen/FormulaListener.cpp
        monitors/parser/gen/FormulaParser.cpp
        monitors/parser/gen/FormulaVisitor.cpp
        monitors/qmc/prime_term.cpp
        monitors/qmc/qmc.cpp
        monitors/qmc/qmc_table.cpp
        monitors/rewriting/basic_rewrite_visitor.cpp
        monitors/rewriting/expression_visitor.cpp
        monitors/rewriting/rewrite_visitor.cpp
        monitors/rv_algorithms/rv_monitor.cpp
        monitors/rv_algorithms/btt_fsm/btt_fsm.cpp
        monitors/rv_algorithms/btt_fsm/btt_fsm_creation.cpp
        monitors/rv_algorithms/btt_fsm/btt_fsm_evaluation.cpp
        monitors/rv_algorithms/btt_fsm/decision.cpp
        monitors/rv_algorithms/btt_fsm/state_btt_fsm.cpp
        monitors/rv_algorithms/dynamic_programming/dynamic.cpp
        monitors/rv_algorithms/dynamic_programming/dynamic_future.cpp
        monitors/rv_algorithms/dynamic_programming/dynamic_past.cpp
        monitors/rv_algorithms/event_consuming/event_consuming.cpp
        monitors/rv_algorithms/event_consuming/event_consuming_visitor.cpp
        monitors/rv_algorithms/mt_fsm/mt_fsm.cpp
        monitors/rv_algorithms/mt_fsm/mt_fsm_evaluation.cpp
        monitors/rv_algorithms/mt_fsm/mt_fsm_creation.cpp
        monitors/rv_algorithms/mt_fsm/multi_transition.cpp
        monitors/rv_algorithms/mt_fsm/state_mt_fsm.cpp
        monitors/rv_algorithms/mtl/mtl_event_consuming.cpp
        monitors/rv_algorithms/mtl/mtl_deriving_future_visitor.cpp
        monitors/rv_algorithms/mtl/mtl_event_consuming_visitor.cpp
        monitors/rv_algorithms/mtl/mtl_resolving_past_visitor.cpp
        monitors/rv_algorithms/mtl/mtl_rewrite_visitor.cpp
        monitors/trace/event.cpp
        monitors/trace/event_generator.cpp
        monitors/utils/lazy_bool.cpp
        monitors/utils/execution_statistic.cpp
        monitors/utils/utils.cpp
        )


# create library of this project
add_library(${LIBRARY_NAME} SHARED ${RV_MONITOR_SRC_FILES})

target_include_directories(${LIBRARY_NAME}
        PUBLIC
        "."
        )

target_link_libraries(${LIBRARY_NAME}
        antlr4cpp
        )