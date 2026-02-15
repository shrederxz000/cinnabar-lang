add_library(
    cinnabar_parser 
    STATIC
    parser.cpp
    expr.cpp
    stmt.cpp
    utils.cpp
)

target_include_directories(
    cinnabar_parser 
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
    cinnabar_parser 
    PUBLIC 
    cinnabar_lexer
)
