#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

static Lexer make_lexer() {return Lexer{};}

TEST(LexerTest, SimpleVarStatement){
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "x:int=5;");

    ASSERT_GE(tokens.size(), 7);
    EXPECT_EQ(tokens[0].kind(), TokenKind::ID);
    ASSERT_TRUE(tokens[0].is<std::string>());
    EXPECT_EQ(tokens[0].as<std::string>(), "x");
    EXPECT_EQ(tokens[1].kind(), TokenKind::COLON);
    EXPECT_EQ(tokens[2].kind(), TokenKind::INT);
    EXPECT_EQ(tokens[3].kind(), TokenKind::ASSIGN);
    EXPECT_EQ(tokens[4].kind(), TokenKind::INT_LITERAL);
    ASSERT_TRUE(tokens[4].is<int64_t>());
    EXPECT_EQ(tokens[4].as<int64_t>(), 5);
    EXPECT_EQ(tokens[5].kind(), TokenKind::SEMICOLON);
}
