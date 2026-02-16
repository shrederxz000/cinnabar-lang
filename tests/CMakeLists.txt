
file(GLOB_RECURSE LEXER_TESTS CONFIGURE_DEPENDS lexer/*.cpp)
#file(GLOB_RECURSE PARSER_TESTS CONFIGURE_DEPENDS parser/*.cpp)

add_executable(cinnabar_tests
    ${LEXER_TESTS}
)

target_link_libraries(cinnabar_tests
    PRIVATE
        cinnabar_lexer
        cinnabar_parser
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cinnabar_tests)
