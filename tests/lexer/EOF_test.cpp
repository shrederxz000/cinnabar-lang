#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}
TEST(LexerTest, EofToken)
{
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "let x = 5;");

    ASSERT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[5].kind(), TokenKind::Eof);
    EXPECT_FALSE(tokens[5].has_value());
}
