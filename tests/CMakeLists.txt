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
        allTests.cpp
        BTT_FSM/TestBTT_FSMEvaluationFuture.cpp
        BTT_FSM/TestBTT_FSMEvaluationPast.cpp
        BTT_FSM/TestBTT_FSMCreation.cpp
        DynamicProgramming/TestDynamicFuture.cpp
        DynamicProgramming/TestDynamicPast.cpp
        EventConsuming/TestEventConsumingFuture.cpp
        EventConsuming/TestEventConsumingPast.cpp
        MT_FSM/TestMT_FSMEvaluationFuture.cpp
        MT_FSM/TestMT_FSMEvaluationPast.cpp
        MTL/Test_MTL.cpp
        parser/TestParser.cpp
        QMC/TestQMC.cpp
        Rewriting/TestBasicRewriteVisitor.cpp
        Rewriting/TestRewriteVisitor.cpp
        utils/TestLazyBool.cpp
        )


add_executable(rvmonitors_test ${RV_MONITOR_TEST_SRC_FILES})

target_compile_definitions(rvmonitors_test PRIVATE)

target_link_libraries(
        rvmonitors_test
        PUBLIC
        ${EXECUTABLE_NAME}
        gmock # gmock includes the gtest target, so we don't need it here.
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
    if (CodeCoverage MATCHES ON)
        add_executable(rvmonitors_test_coverage ${RV_MONITOR_SRC_FILES} ${RV_MONITOR_TEST_SRC_FILES})
        target_include_directories(rvmonitors_test_coverage PUBLIC "./../src/")
        target_link_libraries(rvmonitors_test_coverage PUBLIC
                ${EXECUTABLE_NAME}
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
        )
    endif()
endif()
