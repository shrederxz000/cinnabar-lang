#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}

TEST(LexerTest, StringWithEscapes)
{
    auto lexer = cxz::lexer::Lexer{};
    auto tokens = lexer.tokenize("test", 
        "\"hello\\nworld\" "
        "\"hello\\rworld\" "
        "\"hello\\tworld\" "
        "\"hello\\aworld\" "
        "\"hello\\bworld\" "
        "\"hello\\0world\" "
        "\"hello\\\"world\\\"\" "
    );

    ASSERT_EQ(tokens.size(), 8);

    EXPECT_EQ(tokens[0].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[0].as<std::string>(), "hello\nworld");

    EXPECT_EQ(tokens[1].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[1].as<std::string>(), "hello\rworld");

    EXPECT_EQ(tokens[2].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[2].as<std::string>(), "hello\tworld");

    EXPECT_EQ(tokens[3].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[3].as<std::string>(), "hello\aworld");

    EXPECT_EQ(tokens[4].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[4].as<std::string>(), "hello\bworld");

    EXPECT_EQ(tokens[5].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[5].as<std::string>(), std::string("hello\0world", 11));

    EXPECT_EQ(tokens[6].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[6].as<std::string>(), "hello\"world\"");
}
