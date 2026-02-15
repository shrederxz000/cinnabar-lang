add_subdirectory(lexer)
add_subdirectory(parser)

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    cinnabar_parser
    cinnabar_lexer
    fmt::fmt
)
