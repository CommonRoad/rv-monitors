set(RV_MONITOR_TEST_SRC_FILES
        all_tests.cpp
        rv_algorithms/btt_fsm/test_btt_fsm.cpp
        rv_algorithms/dynamic_programming/test_dynamic_programming_ltl.cpp
        rv_algorithms/event_consuming/test_event_consuming_ltl.cpp
        rv_algorithms/mtl_event_consuming/test_mtl_event_consuming.cpp
        rv_algorithms/mt_fsm/test_mt_fsm.cpp
        rv_algorithms/hydra_2/hydra_test_utils.cpp
        rv_algorithms/hydra_2/test_hydra.cpp
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
        rv_algorithms/hydra_2/hydra_monitors/test_existential_monitor.cpp
        rv_algorithms/hydra_2/hydra_monitors/test_counting_monitor.cpp
        rv_algorithms/hydra_2/hydra_monitors/test_universal_monitor.cpp
        rv_algorithms/hydra_2/hydra_monitors/test_quote_monitor.cpp
        rv_algorithms/hydra_2/test_hydra_creation.cpp
        rv_algorithms/hydra/hydra_test_utils.cpp
        rv_algorithms/hydra/test_hydra.cpp
        rv_algorithms/hydra/hydra_monitors/test_constant_monitor.cpp
        rv_algorithms/hydra/hydra_monitors/test_or_monitor.cpp
        rv_algorithms/hydra/hydra_monitors/test_and_monitor.cpp
        rv_algorithms/hydra/hydra_monitors/test_negation_monitor.cpp
        rv_algorithms/hydra/hydra_monitors/test_atomic_monitor.cpp
        rv_algorithms/hydra/hydra_monitors/test_predicate_monitor.cpp
        rv_algorithms/hydra/hydra_monitors/test_next_monitor.cpp
        rv_algorithms/hydra/hydra_monitors/test_previous_monitor.cpp
        rv_algorithms/hydra/hydra_monitors/test_since_monitor.cpp
        rv_algorithms/hydra/hydra_monitors/test_until_monitor.cpp
        rv_algorithms/test_database.cpp
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
        collections/test_function.cpp
        collections/test_domain.cpp
        collections/test_domain_collection.cpp
        )

# Create executable for test
if (RVMONITORS_BUILD_TESTS)
    add_executable(rvmonitors_test ${RV_MONITOR_TEST_SRC_FILES})

    target_compile_features(rvmonitors_test
            PUBLIC cxx_lambdas cxx_auto_type
            PRIVATE cxx_lambdas cxx_auto_type)

    set_property(TARGET rvmonitors_test PROPERTY POSITION_INDEPENDENT_CODE ON)

    target_link_libraries(rvmonitors_test
            PRIVATE
            rvmonitors
            GTest::gtest
            )

    target_precompile_headers(rvmonitors_test
            PRIVATE
            <gtest/gtest.h>)

    include(GoogleTest)
    # queries the compiled executable for tests, this requires the executable to be runnable. if you are
    # cross compiling, make sure to properly set CMAKE_CROSSCOMPILING_EMULATOR.
    gtest_discover_tests(
            rvmonitors_test
            # TEST_PREFIX "./"
            # increase the discovery timeout for `rvmonitors_test --gtest_list_tests`
            DISCOVERY_TIMEOUT 60
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
            XML_OUTPUT_DIR ${PROJECT_BINARY_DIR}/test-reports
    )
endif ()


add_executable(rvmonitors_performance_eval performance/eval_performance.cpp)
target_compile_definitions(rvmonitors_performance_eval PRIVATE)
target_link_libraries(
        rvmonitors_performance_eval
        PUBLIC
        rvmonitors
)


# Code coverage
# NOTE: The CodeCoverage module will already warn if the build type is not Debug
if (RVMONITORS_BUILD_CODE_COVERAGE)
    include(FetchContent)
    FetchContent_Declare(
            bilke_cmake_modules

            GIT_REPOSITORY https://github.com/bilke/cmake-modules.git
            GIT_TAG        1b68d17d3e3321c08092f994a40e0b6a13dff817
    )
    FetchContent_MakeAvailable(bilke_cmake_modules)
    include(${bilke_cmake_modules_SOURCE_DIR}/CodeCoverage.cmake)

    # Add definition for toolchain::gcov
    include(toolchain/ToolchainLibraries)

    # Unfortunately, append_coverage_compiler_flags_to_target is currently broken,
    # so we simply add the required flags ourselves

    set(COVERAGE_FLAGS -g -fprofile-arcs -ftest-coverage -fprofile-abs-path)

    set_property(SOURCE allTests.cpp PROPERTY COMPILE_DEFINITIONS CODE_COVERAGE)

    foreach(target rvmonitors_core rvmonitors_test)
        # append_coverage_compiler_flags_to_target(${target})
        target_compile_options(${target} PRIVATE ${COVERAGE_FLAGS})
        target_link_libraries(${target} PRIVATE toolchain::gcov)
    endforeach()

    setup_target_for_coverage_gcovr_html(
            NAME rvmonitors_coverage
            EXECUTABLE rvmonitors_test
            EXCLUDES ${FETCHCONTENT_BASE_DIR}
    )
endif()