# make test optional. Useful also when no network connection is available
option(
        MONITORS_BUILD_TESTS
        "Build tests based on the Google Test Framework" ON
)

# do nothing if nothing should be done
if (NOT MONITORS_BUILD_TESTS)
    return()
endif ()

# includes the gtest modules
include(monitors_googletest)

set(RV_MONITOR_TEST_SRC_FILES
        all_tests.cpp
        rv_algorithms/btt_fsm/test_btt_fsm_evaluation_future.cpp
        rv_algorithms/btt_fsm/test_btt_fsm_evaluation_past.cpp
        rv_algorithms/dynamic_programming/test_dynamic_programming_ltl.cpp
        rv_algorithms/event_consuming/test_event_consuming_future.cpp
        rv_algorithms/event_consuming/test_event_consuming_past.cpp
        rv_algorithms/mtl_event_consuming/test_mtl_event_consuming.cpp
        rv_algorithms/mt_fsm/test_mt_fsm_evaluation_future.cpp
        rv_algorithms/mt_fsm/test_mt_fsm_evaluation_past.cpp
        rv_algorithms/hydra_2/hydra_test_utils.cpp
        rv_algorithms/hydra_2/test_hydra.cpp
        rv_algorithms/hydra_2/test_hydra_creation.cpp
        rv_algorithms/hydra_2/hydra_monitors/test_constant_monitor.cpp
        rv_algorithms/hydra_2/hydra_monitors/test_or_monitor.cpp
        rv_algorithms/hydra_2/hydra_monitors/test_and_monitor.cpp
        rv_algorithms/hydra_2/hydra_monitors/test_negation_monitor.cpp
        rv_algorithms/hydra_2/hydra_monitors/test_atomic_monitor.cpp
        rv_algorithms/hydra_2/hydra_monitors/test_predicate_monitor.cpp
        rv_algorithms/hydra_2/hydra_monitors/test_next_monitor.cpp
        rv_algorithms/hydra_2/hydra_monitors/test_previous_monitor.cpp
        rv_algorithms/hydra_2/hydra_monitors/test_since_monitor.cpp
        rv_algorithms/hydra_2/hydra_monitors/test_until_monitor.cpp
        rv_algorithms/test_rv_monitors.cpp
        parser/test_parser.cpp
        formula/test_formula.cpp
        qmc/test_qmc.cpp
        rewriting/test_basic_rewrite_visitor.cpp
        rewriting/test_rewrite_visitor.cpp
        utils/test_lazy_bool.cpp
        utils/generators/formula_generator/test_formula_generator.cpp
        utils/generators/formula_generator/test_formula_generator.cpp
        utils/generators/trace_generator/test_trace_generator.cpp
        utils/generators/trace_generator/test_trace_generator.cpp
        utils/generators/computation_time_generator/test_computation_time_generator.cpp
        utils/generators/computation_time_generator/test_computation_time_generator.cpp

        )


add_executable(rvmonitors_test ${RV_MONITOR_TEST_SRC_FILES})
target_compile_definitions(rvmonitors_test PRIVATE)
target_link_libraries(
        rvmonitors_test
        PUBLIC
        rvmonitors
        gmock # gmock includes the gtest target, so we don't need it here.
)

add_executable(rvmonitors_performance_eval performance/eval_performance.cpp)
target_compile_definitions(rvmonitors_performance_eval PRIVATE)
target_link_libraries(
        rvmonitors_performance_eval
        PUBLIC
        rvmonitors
)


include(GoogleTest)
# queries the compiled executable for tests, this requires the executable to be runnable. if you are
# cross compiling, make sure to properly set CMAKE_CROSSCOMPILING_EMULATOR.
gtest_discover_tests(
        rvmonitors_test TEST_PREFIX "testMonitors/"
        # increase the discovery timeout for `runTests --gtest_list_tests`
        DISCOVERY_TIMEOUT 60
)


# Code coverage
if (CMAKE_BUILD_TYPE MATCHES Debug)
    if (RVMONITORS_BUILD_CODE_COVERAGE)
        add_executable(rvmonitors_test_coverage ${RV_MONITOR_SRC_FILES} ${RV_MONITOR_TEST_SRC_FILES})
        target_include_directories(rvmonitors_test_coverage PUBLIC "../src/")
        target_link_libraries(rvmonitors_test_coverage PUBLIC
                rvmonitors
                antlr4cpp
                gtest
                )
        include(CodeCoverage)
        append_coverage_compiler_flags()
        setup_target_for_coverage_gcovr_html(
                NAME rvmonitors_coverage                                          # New target name
                EXECUTABLE rvmonitors_test_coverage                               # Executable in PROJECT_BINARY_DIR
                DEPENDENCIES rvmonitors_test_coverage                             # Dependencies to build first
                BASE_DIRECTORY "../"                                              # Base directory for report
                #  (defaults to PROJECT_SOURCE_DIR)
                EXCLUDE                                                           # Patterns to exclude (can be relative
                "ci/"                                                             #  to BASE_DIRECTORY, with CMake 3.4+)
                "docs/"
                "external/"
                "src/parser/gen/"
                "build/"
                "cmake/"
                "cmake-build-debug/"
                "cmake-build-debug-coverage/"
        )
    endif()
endif()
