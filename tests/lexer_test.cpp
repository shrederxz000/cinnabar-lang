#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

TEST(LexerTest, SimpleLetStatement)
{
    std::string input = "let x = 5;";
    cxz::lexer::Lexer lexer(input);

    auto tokens = lexer.tokenize();

    EXPECT_FALSE(tokens.empty());
    EXPECT_EQ(tokens[0].type, TokenType::Let);
}
