include(FetchContent)
include(utils/FetchContentHelper)

set(ANTLR_VERSION 4.13.1)

set(antlr_jar_name antlr-${ANTLR_VERSION}-complete.jar)

include(UseJava)
find_jar(
    ANTLR4_JAR_LOCATION
    NAMES antlr
    VERSION ${ANTLR_VERSION} ${ANTLR_VERSION}-complete

    DOC "Path to antlr4 jar file"
    )

if(ANTLR4_JAR_LOCATION STREQUAL "ANTLR4_JAR_LOCATION-NOTFOUND")
    message(STATUS "Downloading antlr4 jar")
    set(_antlr_jar_destination ${CMAKE_CURRENT_BINARY_DIR}/${antlr_jar_name})
    file(DOWNLOAD https://www.antlr.org/download/${antlr_jar_name}
        ${_antlr_jar_destination}
        EXPECTED_HASH SHA256=bc13a9c57a8dd7d5196888211e5ede657cb64a3ce968608697e4f668251a8487
    )
    set(ANTLR4_JAR_LOCATION ${_antlr_jar_destination} CACHE FILEPATH "Path to antlr4 jar file" FORCE)
else()
    message(STATUS "Found antlr4 jar at ${ANTLR4_JAR_LOCATION}")
endif()

FetchContent_Declare_Fallback(
    antlr4

    SYSTEM

    # NOTE - URL download should be preferred:
    # HTTP downloads are faster than Git clones and therefore reduce configuration time

    # Git version for reference
    # GIT_REPOSITORY  https://github.com/antlr/antlr4.git
    # GIT_TAG         ${ANTLR_VERSION}

    SOURCE_SUBDIR runtime/Cpp

    URL https://github.com/antlr/antlr4/archive/refs/tags/${ANTLR_VERSION}.tar.gz
    URL_HASH SHA256=da20d487524d7f0a8b13f73a8dc326de7fc2e5775f5a49693c0a4e59c6b1410c

    FIND_PACKAGE_ARGS ${ANTLR_VERSION} NAMES antlr4-runtime
)

set(ANTLR_BUILD_CPP_TESTS OFF)
set(ANTLR4_INSTALL ON)

FetchContent_MakeAvailable(antlr4)

# find_package(Java 11 COMPONENTS Runtime)

if(DEFINED antlr4_SOURCE_DIR)
    find_package(antlr4-generator REQUIRED
        PATHS ${antlr4_BINARY_DIR}
        NO_DEFAULT_PATH
        )

    foreach(target antlr4_shared antlr4_static)
        if(TARGET ${target})
            target_include_directories(${target} INTERFACE
                $<BUILD_INTERFACE:${antlr4_SOURCE_DIR}/runtime/Cpp/runtime/src>
                )
        endif()
    endforeach()
else()
    find_package(antlr4-generator REQUIRED)

    foreach(target antlr4_shared antlr4_static)
        if(TARGET ${target})
            target_include_directories(${target} INTERFACE ${ANTLR4_INCLUDE_DIR})
        endif()
    endforeach()
endif()

if(BUILD_SHARED_LIBS AND TARGET antlr4_shared)
    add_library(antlr4 ALIAS antlr4_shared)
elseif(TARGET antlr4_static)
    add_library(antlr4 ALIAS antlr4_static)
else()
    message(FATAL_ERROR "antlr4 not available under the usual target names")
endif()

include(CheckIncludeFileCXX)

get_target_property(antlr4_interface_include_dirs antlr4 INTERFACE_INCLUDE_DIRECTORIES)
message(VERBOSE "Considering the following include directories for \"antlr4-runtime.h\": ${antlr4_interface_include_dirs}")

set(CMAKE_REQUIRED_INCLUDES ${antlr4_interface_include_dirs})
CHECK_INCLUDE_FILE_CXX(antlr4-runtime.h ANTLR4_HEADER_EXISTS)

if(NOT ANTLR4_HEADER_EXISTS)
    message(SEND_ERROR "Header \"antlr4-runtime.h\" is not where we expect it to be")
endif()