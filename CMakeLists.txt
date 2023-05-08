cmake_minimum_required(VERSION 3.20..3.25)

project(RVMonitors
        LANGUAGES C CXX
        VERSION 0.5
        HOMEPAGE_URL "https://commonroad.in.tum.de/"
        DESCRIPTION "Classes and methods to monitor traces based on temporal logic specifications in C++17")

# CMP0135 - URL download timestamp
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# Adapted from Eigen3 - snippet to get a value for PROJECT_IS_TOP_LEVEL
# on CMake versions before v3.21.0
if(CMAKE_VERSION VERSION_LESS 3.21.0)
    if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
        set(PROJECT_IS_TOP_LEVEL ON)
    else()
        set(PROJECT_IS_TOP_LEVEL OFF)
    endif()
    set(${PROJECT_NAME}_IS_TOP_LEVEL ${PROJECT_IS_TOP_LEVEL})
endif()

set(CMAKE_VERIFY_INTERFACE_HEADER_SETS ON)

set(CMAKE_COLOR_DIAGNOSTICS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_LINK_LIBRARIES_ONLY_TARGETS ON)

set(CMAKE_MESSAGE_CONTEXT_SHOW ON)

# Compile command database is required for Clang-assisted parsing in Doxygen
# TODO: Set this conditionally
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

include(CMakeDependentOption)

set(RVMONITORS_BUILD_EXTRAS_DEFAULT ON)
if(NOT RVMonitors_IS_TOP_LEVEL)
    set(RVMONITORS_BUILD_EXTRAS_DEFAULT OFF)
endif()

# TODO Move below dependency discovery
find_package(Doxygen QUIET COMPONENTS dot)

cmake_dependent_option(RVMONITORS_MODEL_BUILD_DOXYGEN
        "Build doxygen"
        ${RVMONITORS_BUILD_EXTRAS_DEFAULT}
        "Doxygen_FOUND;NOT SKBUILD"
        OFF)

cmake_dependent_option(RVMONITORS_BUILD_TESTS
        "Build tests"
        ${RVMONITORS_BUILD_EXTRAS_DEFAULT}
        "NOT SKBUILD"
        OFF)

# add_library (without STATIC or SHARED) will respect BUILD_SHARED_LIBS when determining the library type
# The following definitions allow setting BUILD_SHARED_LIBS globally or only for this project
# (useful when including it into another project).
#
# If ${PROJECT_NAME}_BUILD_SHARED_LIBS is set to a specific value by another project or the user,
# then we will use that value (by setting BUILD_SHARED_LIBS to the value of ${PROJECT_NAME}_BUILD_SHARED_LIBS).
# Otherwise, the default for ${PROJECT_NAME}_BUILD_SHARED_LIBS is taken from BUILD_SHARED_LIBS if it exists.

if(DEFINED BUILD_SHARED_LIBS)
    set(_BUILD_SHARED_LIBS_DEFAULT ${BUILD_SHARED_LIBS})
else()
    set(_BUILD_SHARED_LIBS_DEFAULT ON)
endif()

option(${PROJECT_NAME}_BUILD_SHARED_LIBS "Build ${PROJECT_NAME} as a shared library" ${_BUILD_SHARED_LIBS_DEFAULT})

set(BUILD_SHARED_LIBS ${${PROJECT_NAME}_BUILD_SHARED_LIBS})

cmake_dependent_option(RVMONITORS_BUILD_CODE_COVERAGE
        "Build code coverage"
        OFF
        "RVMONITORS_BUILD_TESTS;NOT SKBUILD"
        OFF)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_SUPPORTS_TRY_FIND_PACKAGE OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24.0)
    set(CMAKE_SUPPORTS_TRY_FIND_PACKAGE ON)
else()
    message(WARNING "Your CMake version (${CMAKE_VERSION}) does not support "
            "the FetchContent find_package integration introduced in CMake 3.24. "
            "As a fallback, we will simply build all dependencies ourselves "
            "irrespective of whether a suitable system version exists. "
            "While this does not impair functionality, it might slow down the build "
            "process a bit.\n"
            "In case you have all required dependencies installed, you can try "
            "enabling the option\n"
            "\tRVMONITORS_SYSTEM_PACKAGES_FORCE\n"
            "which will force using find_package for all dependencies.")
endif()

option(RVMONITORS_SYSTEM_PACKAGES "Try to use system packages for dependencies" ON)
cmake_dependent_option(RVMONITORS_SYSTEM_PACKAGES_FORCE
        "For CMake<3.24: Force using system packages for all dependencies"
        OFF
        "NOT CMAKE_SUPPORTS_TRY_FIND_PACKAGE"
        OFF
        )

include(FetchContent)
FetchContent_Declare(
        commonroad_cmake

        GIT_REPOSITORY https://gitlab.lrz.de/tum-cps/commonroad-cmake.git
        GIT_TAG        main
)
FetchContent_MakeAvailable(commonroad_cmake)

list(APPEND CMAKE_MODULE_PATH ${commonroad_cmake_SOURCE_DIR})

include(toolchain/DiscoverLLD OPTIONAL)
include(toolchain/DiscoverSanitizers OPTIONAL)

# This is a helper script that will automatically add a .gitignore file to the
# binary directory (build directory) so you don't have to do add every build folder
# to your .gitignore.
include(extras/GitIgnoreBinaryDir OPTIONAL)

# Appends dictionary where other cmake files are stored which install required Software
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/external/)

if(CMAKE_SUPPORTS_TRY_FIND_PACKAGE)
    if(SKBUILD_SELF_CONTAINED)
        set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE NEVER)
    endif()
    if(RVMONITORS_SYSTEM_PACKAGES)
        set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE OPT_IN)
    else()
        set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE NEVER)
    endif()
endif()

# Include Eigen3, Boost filesystem, spdlog, and OpenMP
if(RVMONITORS_SYSTEM_PACKAGES_FORCE)
    # This is the fallback branch in case the CMake version is older than 3.24
    # and the user requested we try to use system packages
    # For CMake > 3.24, fallback is automatic through the FetchContent find_package
    # integration.

    message(WARNING "RVMONITORS_SYSTEM_PACKAGES_FORCE is set - trying to satisfy "
            "all dependencies using installed system packages.\n"
            "If this fails, consider disabling RVMONITORS_SYSTEM_PACKAGES_FORCE and "
            "trying again.")

    find_package(GTest)
else()
    # Normal path: We try to use find_package via FetchContent, otherwise we fall
    # back to normal FetchContent

    include(ExternalGoogleTest)
endif()

include(ExternalAntlr)

# Ensure executables are in the top level directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add source code
add_subdirectory(src)

# Doxygen
if(RVMONITORS_BUILD_DOXYGEN)
    include(doxygen)
endif(RVMONITORS_BUILD_DOXYGEN)

# add unit testing
if(RVMONITORS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(RVMONITORS_BUILD_TESTS)


include(cmake/install.cmake)

