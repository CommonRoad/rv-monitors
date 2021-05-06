cmake_minimum_required(VERSION 3.16)
project(RV-Monitors)

#SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wno-ignored-attributes)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Appends dictionary where other cmake files are stored which install required Software
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/)

# Set compiler optimizations for build types
set(CMAKE_CXX_FLAGS_RELEASE  "-O3")
set(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

set(ANTLR4CPP_JAR_LOCATION ${CMAKE_SOURCE_DIR}/libs/antlr.jar)

include(monitors_sanitizers)

# get antlr4 as external project
include(monitors_antlr4cpp)
# Doxygen
include(doxygen)

# Activate code coverage
if (CODE_COVERAGE)
    option(CodeCoverage "CodeCoverage" ON)
endif()


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../)

# Add Environment model node
add_subdirectory(src)

# add unit testing
enable_testing()
add_subdirectory(tests)
