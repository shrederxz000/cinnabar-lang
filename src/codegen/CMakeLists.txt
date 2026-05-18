add_library(
    cinnabar_codegen
    STATIC
    serializer.cpp
)

target_include_directories(
    cinnabar_codegen
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
    cinnabar_codegen
    PUBLIC
    cinnabar_lexer
    cinnabar_parser
)
