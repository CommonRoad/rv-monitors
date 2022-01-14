include(FetchContent)
include(GNUInstallDirs)

FetchContent_Declare(
        external_googletest

        GIT_REPOSITORY https://github.com/google/googletest.git
        # Commit for v1.11.0
        GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929
        GIT_PROGRESS   true
)

# Stage 2: Run FetchContent_MakeAvailable
# CMake recommends running FetchContent_Declare for all third-party projects first
# and finally running FetchContent_MakeAvailable in the end
# See https://cmake.org/cmake/help/v3.22/module/FetchContent.html#command:fetchcontent_makeavailable

FetchContent_MakeAvailable(external_googletest)

# Add google-test
mark_as_advanced(
        BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
        gmock_build_tests gtest_build_samples gtest_build_tests
        gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)
set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)