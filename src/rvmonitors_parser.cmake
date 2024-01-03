add_library(rvmonitors_parser)

set(parser_include_path "monitors/parser/generated")
set(generated_base_dir ${CMAKE_CURRENT_BINARY_DIR}/antlr4_generated)
# Upstream bug: Checks for ANTLR4_CPP_GENERATED_SRC_DIR, but uses ANTLR4_GENERATED_SRC_DIR
# Use bogus path here so we notice if they ever fix their bug and use ANTLR4_CPP_GENERATED_SRC_DIR again
set(ANTLR4_CPP_GENERATED_SRC_DIR "/boguspath")
set(ANTLR4_GENERATED_SRC_DIR ${generated_base_dir}/${parser_include_path})

foreach(grammar Log Formula)
    set(generated_name ${grammar}GrammarParser)

    antlr4_generate(${generated_name} ${PROJECT_SOURCE_DIR}/${grammar}.g4 BOTH LISTENER VISITOR)

    set(grammar_dir ${ANTLR4_INCLUDE_DIR_${generated_name}})
    set(grammar_src_files ${ANTLR4_SRC_FILES_${generated_name}})
    set(grammar_hdr_files ${ANTLR4_SRC_FILES_${generated_name}})
    list(FILTER grammar_hdr_files INCLUDE REGEX "\.h$")

    target_include_directories(rvmonitors_parser SYSTEM PUBLIC
        $<BUILD_INTERFACE:${generated_base_dir}>
        )
    target_sources(rvmonitors_parser PRIVATE ${grammar_src_files})

    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.23.0)
        target_sources(rvmonitors_parser
            INTERFACE
            FILE_SET rvmonitors_parser_headers
            TYPE HEADERS
            BASE_DIRS ${generated_base_dir}
            FILES ${grammar_hdr_files}
        )
    else()
        install(DIRECTORY ${generated_base_dir}
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
                FILES_MATCHING PATTERN "*.h")
    endif()
endforeach()

target_link_libraries(rvmonitors_parser PUBLIC antlr4)
