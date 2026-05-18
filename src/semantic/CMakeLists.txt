add_library(
    cinnabar_semantic
    STATIC
    semantic.cpp
    expr.cpp
    scope.cpp
    stmt.cpp
    std.cpp
    typechecker.cpp
)

target_include_directories(
    cinnabar_semantic
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
    cinnabar_semantic
    PUBLIC 
    cinnabar_lexer
)
