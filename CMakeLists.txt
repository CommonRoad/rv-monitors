cmake_minimum_required(VERSION 3.16)
project(RVMonitors)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(RVMONITORS_BUILD_DOXYGEN "Build doxygen" OFF)
option(RVMONITORS_BUILD_TESTS "Build tests" ON)
option(RVMONITORS_BUILD_PYBIND "Build python interface" OFF)
option(RVMONITORS_BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(RVMONITORS_BUILD_CODE_COVERAGE "Build code coverage" OFF)

# Set a default build type if none was specified
set(default_build_type "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    set(default_build_type "Debug")
endif()
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
            STRING "Choose the type of build." FORCE)
endif()

# Include spdlog
#environment-model
include(FetchContent)
FetchContent_Declare(
        spdlog
        URL https://github.com/gabime/spdlog/archive/refs/tags/v1.11.0.tar.gz

        URL_HASH SHA256=ca5cae8d6cac15dae0ec63b21d6ad3530070650f68076f3a4a862ca293a858bb
        FIND_PACKAGE_ARGS 1.8.0
        )
set(SPDLOG_BUILD_SHARED OFF)
FetchContent_MakeAvailable(spdlog)

# spdlog::spdlog is only present in some configurations, so add it if it is not already present
if(NOT TARGET spdlog::spdlog)
    add_library(spdlog::spdlog ALIAS spdlog)
endif()
find_package(spdlog REQUIRED)

# Appends dictionary where other cmake files are stored which install required Software
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/)

# Set compiler optimizations for build types
set(CMAKE_CXX_FLAGS_RELEASE  "-O3")
set(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

set(ANTLR4CPP_JAR_LOCATION ${CMAKE_SOURCE_DIR}/libs/antlr.jar)

# get antlr4 as external project
include(monitors_antlr4cpp)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

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

# Provides configure_package_config_file
include(CMakePackageConfigHelpers)

# Includes sane defaults for installation paths (CMAKE_INSTALL_LIBDIR, CMAKE_INSTALL_BINDIR etc.)
include(GNUInstallDirs)

install(TARGETS rvmonitors
        EXPORT ${PROJECT_NAME}_Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Export target configuration (for installation)
install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        )

# Export target configuration (for local building)
export(EXPORT ${PROJECT_NAME}_Targets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
        NAMESPACE ${PROJECT_NAME}::
        )

install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
