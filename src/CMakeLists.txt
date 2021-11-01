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
        monitors/rv_algorithms/monitor_interface.cpp
        monitors/rv_algorithms/rv_monitor.cpp
        monitors/rv_algorithms/btt_fsm/btt_fsm.cpp
        monitors/rv_algorithms/btt_fsm/btt_fsm_utils_creation.cpp
        monitors/rv_algorithms/btt_fsm/decision.cpp
        monitors/rv_algorithms/btt_fsm/state_btt_fsm.cpp
        monitors/rv_algorithms/dynamic_programming/dynamic.cpp
        monitors/rv_algorithms/ltl_event_consuming/event_consuming.cpp
        monitors/rv_algorithms/ltl_event_consuming/event_consuming_visitor.cpp
        monitors/rv_algorithms/mt_fsm/mt_fsm.cpp
        monitors/rv_algorithms/mt_fsm/multi_transition.cpp
        monitors/rv_algorithms/mt_fsm/state_mt_fsm.cpp
        monitors/rv_algorithms/mtl_event_consuming/mtl_event_consuming.cpp
        monitors/rv_algorithms/mtl_event_consuming/mtl_deriving_future_visitor.cpp
        monitors/rv_algorithms/mtl_event_consuming/mtl_event_consuming_visitor.cpp
        monitors/rv_algorithms/mtl_event_consuming/mtl_resolving_past_visitor.cpp
        monitors/rewriting/mtl_rewrite_visitor.cpp
        monitors/rv_algorithms/hydra_2/hydra.cpp
        monitors/rv_algorithms/hydra_2/hydra_monitors/hydra_monitor.cpp
        monitors/rv_algorithms/hydra_2/hydra_monitors/previous_monitor.cpp
        monitors/rv_algorithms/hydra_2/hydra_creation_visitor.cpp
        monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.cpp
        monitors/rv_algorithms/hydra_2/hydra_monitors/constant_monitor.cpp
        monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.cpp
        monitors/rv_algorithms/hydra_2/hydra_monitors/next_monitor.cpp
        monitors/rv_algorithms/hydra_2/hydra_monitors/since_monitor.cpp
        monitors/rv_algorithms/hydra_2/hydra_monitors/until_monitor.cpp
        monitors/rv_algorithms/hydra_2/hydra_monitors/or_monitor.cpp
        monitors/rv_algorithms/hydra_2/hydra_monitors/and_monitor.cpp
        monitors/rv_algorithms/hydra_2/hydra_monitors/negation_monitor.cpp
        monitors/trace/event.cpp
        monitors/trace/event_generator.cpp
        monitors/utils/lazy_bool.cpp
        monitors/utils/execution_statistic.cpp
        monitors/utils/utils.cpp
        )


# create library of this project
add_library(${LIBRARY_NAME} SHARED ${RV_MONITOR_SRC_FILES})

target_compile_features(${LIBRARY_NAME}
        PUBLIC cxx_lambdas cxx_auto_type
        PRIVATE cxx_lambdas cxx_auto_type)

set_property(TARGET ${LIBRARY_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(${LIBRARY_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(${LIBRARY_NAME}
        antlr4cpp
        )

# this adds many warnings to the build. They usually help to find some bugs
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(${LIBRARY_NAME}  PRIVATE -Wall -Wextra -Wconversion
            -pedantic -Wfatal-errors)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${LIBRARY_NAME}  PRIVATE /W3 /WX)
    target_compile_options(HW01Main PRIVATE /W3 /WX)
endif()
