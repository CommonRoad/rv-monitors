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
        monitors/expression_tree/binary/temporal/until.cpp
        monitors/expression_tree/binary/temporal/release.cpp
        monitors/expression_tree/binary/temporal/since.cpp
        monitors/expression_tree/nary/bool/and.cpp
        monitors/expression_tree/nary/bool/or.cpp
        monitors/expression_tree/nary/bool/xor.cpp
        monitors/expression_tree/unary/deriving_operator.cpp
        monitors/expression_tree/unary/event_operator.cpp
        monitors/expression_tree/unary/resolving_operator.cpp
        monitors/expression_tree/unary/unary.cpp
        monitors/expression_tree/unary/bool/not.cpp
        monitors/expression_tree/unary/temporal/future.cpp
        monitors/expression_tree/unary/temporal/globally.cpp
        monitors/expression_tree/unary/temporal/next_strong.cpp
        monitors/expression_tree/unary/temporal/next_weak.cpp
        monitors/expression_tree/unary/temporal/historically.cpp
        monitors/expression_tree/unary/temporal/once.cpp
        monitors/expression_tree/unary/temporal/previously.cpp
        monitors/formula/formula.cpp
        monitors/formula/supported_formula_type.cpp
        monitors/formula/nff.cpp
        monitors/parser/parser.cpp
        monitors/parser/visitor.cpp
        monitors/parser/log_visitor.cpp
        monitors/qmc/prime_term.cpp
        monitors/qmc/qmc.cpp
        monitors/qmc/qmc_table.cpp
        monitors/rewriting/basic_rewrite_visitor.cpp
        monitors/rewriting/expression_visitor.cpp
        monitors/rewriting/rewrite_visitor.cpp
        monitors/rewriting/mtl_rewrite_visitor.cpp
        monitors/rewriting/hydra_rewrite_visitor.cpp
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
        monitors/rv_algorithms/mtl_event_consuming/mtl_event_consuming_visitor.cpp
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
        monitors/rv_algorithms/hydra/hydra.cpp
        monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.cpp
        monitors/rv_algorithms/hydra/hydra_monitors/previous_monitor.cpp
        monitors/rv_algorithms/hydra/hydra_creation_visitor.cpp
        monitors/rv_algorithms/hydra/hydra_monitors/predicate_monitor.cpp
        monitors/rv_algorithms/hydra/hydra_monitors/constant_monitor.cpp
        monitors/rv_algorithms/hydra/hydra_monitors/atomic_monitor.cpp
        monitors/rv_algorithms/hydra/hydra_monitors/next_monitor.cpp
        monitors/rv_algorithms/hydra/hydra_monitors/since_monitor.cpp
        monitors/rv_algorithms/hydra/hydra_monitors/until_monitor.cpp
        monitors/rv_algorithms/hydra/hydra_monitors/or_monitor.cpp
        monitors/rv_algorithms/hydra/hydra_monitors/and_monitor.cpp
        monitors/rv_algorithms/hydra/hydra_monitors/negation_monitor.cpp
        monitors/trace/event.cpp
        monitors/trace/event_generator.cpp
        monitors/utils/lazy_bool.cpp
        monitors/utils/execution_statistic.cpp
        monitors/utils/utils.cpp
        monitors/utils/generators/formula_generator/formula_generator.cpp
        monitors/utils/generators/trace_generator/trace_generator.cpp
        monitors/utils/generators/computation_time_generator/computation_time_generator.cpp
        monitors/utils/interval.cpp monitors/utils/interval.cpp
        )

set(RV_MONITOR_HDR_FILES
        monitors/expression_tree/expression.h
        monitors/expression_tree/interval.h
        monitors/expression_tree/temporal_operator.h
        monitors/expression_tree/interval.h
        monitors/expression_tree/atomic/atomic.h
        monitors/expression_tree/atomic/constant.h
        monitors/expression_tree/atomic/predicate.h
        monitors/expression_tree/binary/binary.h
        monitors/expression_tree/binary/bool/equivalence.h
        monitors/expression_tree/binary/bool/implication.h
        monitors/expression_tree/binary/temporal/until.h
        monitors/expression_tree/binary/temporal/release.h
        monitors/expression_tree/binary/temporal/since.h
        monitors/expression_tree/nary/nary_interface.h
        monitors/expression_tree/nary/nary.h
        monitors/expression_tree/nary/bool/and.h
        monitors/expression_tree/nary/bool/or.h
        monitors/expression_tree/nary/bool/xor.h
        monitors/expression_tree/unary/deriving_operator.h
        monitors/expression_tree/unary/event_operator.h
        monitors/expression_tree/unary/resolving_operator.h
        monitors/expression_tree/unary/unary.h
        monitors/expression_tree/unary/bool/not.h
        monitors/expression_tree/unary/temporal/future.h
        monitors/expression_tree/unary/temporal/globally.h
        monitors/expression_tree/unary/temporal/next_strong.h
        monitors/expression_tree/unary/temporal/next_weak.h
        monitors/expression_tree/unary/temporal/historically.h
        monitors/expression_tree/unary/temporal/once.h
        monitors/expression_tree/unary/temporal/previously.h
        monitors/formula/formula.h
        monitors/formula/supported_formula_type.h
        monitors/formula/nff.h
        monitors/parser/parser.h
        monitors/parser/visitor.h
        monitors/parser/log_visitor.h
        monitors/qmc/prime_term.h
        monitors/qmc/qmc.h
        monitors/qmc/qmc_table.h
        monitors/rewriting/basic_rewrite_visitor.h
        monitors/rewriting/expression_visitor.h
        monitors/rewriting/rewrite_visitor.h
        monitors/rewriting/mtl_rewrite_visitor.h
        monitors/rewriting/hydra_rewrite_visitor.h
        monitors/rv_algorithms/monitor_interface.h
        monitors/rv_algorithms/rv_monitor.h
        monitors/rv_algorithms/btt_fsm/btt_fsm.h
        monitors/rv_algorithms/btt_fsm/btt_fsm_utils_creation.h
        monitors/rv_algorithms/btt_fsm/decision.h
        monitors/rv_algorithms/btt_fsm/node.h
        monitors/rv_algorithms/btt_fsm/state_btt_fsm.h
        monitors/rv_algorithms/dynamic_programming/dynamic.h
        monitors/rv_algorithms/ltl_event_consuming/event_consuming.h
        monitors/rv_algorithms/ltl_event_consuming/event_consuming_visitor.h
        monitors/rv_algorithms/mt_fsm/mt_fsm.h
        monitors/rv_algorithms/mt_fsm/multi_transition.h
        monitors/rv_algorithms/mt_fsm/state_mt_fsm.h
        monitors/rv_algorithms/mtl_event_consuming/mtl_event_consuming.h
        monitors/rv_algorithms/mtl_event_consuming/mtl_event_consuming_visitor.h
        monitors/rv_algorithms/hydra_2/hydra.h
        monitors/rv_algorithms/hydra_2/hydra_monitors/hydra_monitor.h
        monitors/rv_algorithms/hydra_2/hydra_monitors/previous_monitor.h
        monitors/rv_algorithms/hydra_2/hydra_creation_visitor.h
        monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h
        monitors/rv_algorithms/hydra_2/hydra_monitors/constant_monitor.h
        monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h
        monitors/rv_algorithms/hydra_2/hydra_monitors/next_monitor.h
        monitors/rv_algorithms/hydra_2/hydra_monitors/since_monitor.h
        monitors/rv_algorithms/hydra_2/hydra_monitors/until_monitor.h
        monitors/rv_algorithms/hydra_2/hydra_monitors/or_monitor.h
        monitors/rv_algorithms/hydra_2/hydra_monitors/and_monitor.h
        monitors/rv_algorithms/hydra_2/hydra_monitors/negation_monitor.h
        monitors/rv_algorithms/hydra/hydra.h
        monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h
        monitors/rv_algorithms/hydra/hydra_monitors/previous_monitor.h
        monitors/rv_algorithms/hydra/hydra_creation_visitor.h
        monitors/rv_algorithms/hydra/hydra_monitors/predicate_monitor.h
        monitors/rv_algorithms/hydra/hydra_monitors/constant_monitor.h
        monitors/rv_algorithms/hydra/hydra_monitors/atomic_monitor.h
        monitors/rv_algorithms/hydra/hydra_monitors/next_monitor.h
        monitors/rv_algorithms/hydra/hydra_monitors/since_monitor.h
        monitors/rv_algorithms/hydra/hydra_monitors/until_monitor.h
        monitors/rv_algorithms/hydra/hydra_monitors/or_monitor.h
        monitors/rv_algorithms/hydra/hydra_monitors/and_monitor.h
        monitors/rv_algorithms/hydra/hydra_monitors/negation_monitor.h
        monitors/symbols/symbols.h
        monitors/trace/event.h
        monitors/trace/event_generator.h
        monitors/trace/trace.h
        monitors/utils/lazy_bool.h
        monitors/utils/execution_statistic.h
        monitors/utils/utils.h
        monitors/utils/generators/formula_generator/formula_generator.h
        monitors/utils/generators/trace_generator/trace_generator.h
        monitors/utils/generators/computation_time_generator/computation_time_generator.h
        monitors/utils/interval.h
        )

include(./rvmonitors_parser.cmake)

# create library of this project
add_library(rvmonitors_core)
target_sources(rvmonitors_core PRIVATE ${RV_MONITOR_SRC_FILES})

target_link_libraries(rvmonitors_core PUBLIC rvmonitors_parser)


list(TRANSFORM RV_MONITOR_HDR_FILES PREPEND ${PROJECT_SOURCE_DIR}/include/)

target_sources(${LIBRARY_NAME}_core PRIVATE ${RV_MONITOR_HDR_FILES})

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.23.0)
    target_sources(${LIBRARY_NAME}_core
            INTERFACE
            FILE_SET rvmonitors_core_headers
            TYPE HEADERS
            BASE_DIRS ${PROJECT_SOURCE_DIR}/include
            FILES ${RV_MONITOR_HDR_FILES})
endif()

target_compile_features(${LIBRARY_NAME}_core
        PUBLIC cxx_lambdas cxx_auto_type
        PRIVATE cxx_lambdas cxx_auto_type)

set_property(TARGET ${LIBRARY_NAME}_core PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(${LIBRARY_NAME}_core
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        )

target_precompile_headers(${LIBRARY_NAME}_core
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/utils/lazy_bool.h>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/utils/utils.h>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/trace/trace.h>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/trace/event.h>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/rv_algorithms/rv_monitor.h>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/rv_algorithms/monitor_interface.h>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/rv_algorithms/mtl_event_consuming/mtl_event_consuming.h>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/rv_algorithms/mt_fsm/mt_fsm.h>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/rv_algorithms/ltl_event_consuming/event_consuming.h>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/rv_algorithms/hydra_2/hydra.h>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/rv_algorithms/hydra/hydra.h>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/rv_algorithms/dynamic_programming/dynamic.h>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/rv_algorithms/btt_fsm/btt_fsm.h>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitors/formula/formula.h>
        )

# Create environment model library
add_library(${LIBRARY_NAME} INTERFACE)

target_link_libraries(${LIBRARY_NAME}
        INTERFACE
        ${LIBRARY_NAME}_core
        )

add_library(RVMonitors::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

add_executable(rvmonitors_eval monitors/evaluation/evaluation.cpp)
target_link_libraries(${LIBRARY_NAME}_eval
        PUBLIC
        ${LIBRARY_NAME}
        )

# this adds many warnings to the build. They usually help to find some bugs
# TODO: Check whether each warning flag is actually supported by the compiler before adding it
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    foreach(target IN ITEMS ${LIBRARY_NAME} ${LIBRARY_NAME}_core)
        target_compile_options(${LIBRARY_NAME}_core PRIVATE -Wall -Wextra -Wconversion
                -pedantic -Wfatal-errors -Wno-unused-parameter)
    endforeach()
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    foreach(target IN ITEMS ${LIBRARY_NAME} ${LIBRARY_NAME}_core)
        target_compile_options(${LIBRARY_NAME}_core PRIVATE /W3 /WX)
    endforeach()
endif()


