#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}

TEST(LexerTest, TokenPosition)
{
auto lexer = make_lexer();
auto tokens = lexer.tokenize("test", "let x\n= 5;");

ASSERT_GE(tokens.size(), 6); //<let>,<x>,<=>,<5>,<;>,<EOF>

EXPECT_EQ(tokens[0].pos().line, 1);
EXPECT_EQ(tokens[2].pos().line, 2); // '=' на второй строке
}
