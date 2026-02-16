#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}

TEST(LexerTest, StringLiteral)
{
    auto lexer = cxz::lexer::Lexer{};
    auto tokens = lexer.tokenize("test", "\"hello\"");

    ASSERT_EQ(tokens.size(), 2);

    EXPECT_EQ(tokens[0].kind(), TokenKind::STRING_LITERAL);
    ASSERT_TRUE(tokens[0].is<std::string>());
    EXPECT_EQ(tokens[0].as<std::string>(), "hello");
}

TEST(LexerTest, StringLiteralLostClosingQoute)
{
    auto lexer = cxz::lexer::Lexer{}; 

    EXPECT_THROW({
        lexer.tokenize("test", "\"hello");
    }, std::runtime_error);
}
