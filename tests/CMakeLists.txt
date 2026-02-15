add_executable(cinnabar_tests
    lexer_test.cpp
    parser_test.cpp
)

target_link_libraries(cinnabar_tests
    PRIVATE
        cinnabar_lexer
        cinnabar_parser
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cinnabar_tests)
