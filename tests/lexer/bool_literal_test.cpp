#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}

TEST(LexerTest, BoolLiteral)
{
    auto lexer = cxz::lexer::Lexer{};
    auto tokens = lexer.tokenize("test", "true false");

    ASSERT_EQ(tokens.size(), 3);

    EXPECT_EQ(tokens[0].kind(), TokenKind::BOOL_LITERAL);
    EXPECT_TRUE(tokens[0].as<bool>());

    EXPECT_EQ(tokens[1].kind(), TokenKind::BOOL_LITERAL);
    EXPECT_FALSE(tokens[1].as<bool>());

}
