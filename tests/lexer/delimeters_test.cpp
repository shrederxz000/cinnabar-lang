#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}

TEST(LexerTest, Delimiters)
{
auto lexer = make_lexer();
auto tokens = lexer.tokenize("test", 
    "( ) " 
    "{ } " 
    "; " 
    ", " 
    ". .. ... " 
    ": :: " 
    "[ ] " 
    "< > "
);

ASSERT_EQ(tokens.size(), 16);

EXPECT_EQ(tokens[0].kind(), TokenKind::LPAR);
EXPECT_EQ(tokens[1].kind(), TokenKind::RPAR);
EXPECT_EQ(tokens[2].kind(), TokenKind::LBRACE);
EXPECT_EQ(tokens[3].kind(), TokenKind::RBRACE);
EXPECT_EQ(tokens[4].kind(), TokenKind::SEMICOLON);
EXPECT_EQ(tokens[5].kind(), TokenKind::COMMA);
EXPECT_EQ(tokens[6].kind(), TokenKind::DOT);
EXPECT_EQ(tokens[7].kind(), TokenKind::RANGE);
EXPECT_EQ(tokens[8].kind(), TokenKind::ELLIPSIS);
EXPECT_EQ(tokens[9].kind(), TokenKind::COLON);
EXPECT_EQ(tokens[10].kind(), TokenKind::DCOLON);
EXPECT_EQ(tokens[11].kind(), TokenKind::LBRACKET);
EXPECT_EQ(tokens[12].kind(), TokenKind::RBRACKET);
EXPECT_EQ(tokens[13].kind(), TokenKind::LT);
EXPECT_EQ(tokens[14].kind(), TokenKind::GT);
}
