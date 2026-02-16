#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}

TEST(LexerTest, SimpleLetStatement)
{
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "let x = 5;");

    ASSERT_GE(tokens.size(), 6);

    EXPECT_EQ(tokens[0].kind(), TokenKind::LET);

    EXPECT_EQ(tokens[1].kind(), TokenKind::ID);
    ASSERT_TRUE(tokens[1].is<std::string>());
    EXPECT_EQ(tokens[1].as<std::string>(), "x");

    EXPECT_EQ(tokens[2].kind(), TokenKind::ASSIGN);

    EXPECT_EQ(tokens[3].kind(), TokenKind::INT_LITERAL);
    ASSERT_TRUE(tokens[3].is<int64_t>());
    EXPECT_EQ(tokens[3].as<int64_t>(), 5);

    EXPECT_EQ(tokens[4].kind(), TokenKind::SEMICOLON);
}
