#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

static Lexer make_lexer() {return Lexer{};}

TEST(LexerTest, Operators){
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "+ - / * ** = || && <= >= == != ");

    ASSERT_EQ(tokens.size(), 13);
    EXPECT_EQ(tokens[0].kind(), TokenKind::PLUS);
    EXPECT_EQ(tokens[1].kind(), TokenKind::MINUS);
    EXPECT_EQ(tokens[2].kind(), TokenKind::SLASH);
    EXPECT_EQ(tokens[3].kind(), TokenKind::STAR);
    EXPECT_EQ(tokens[4].kind(), TokenKind::POW);
    EXPECT_EQ(tokens[5].kind(), TokenKind::ASSIGN);
    EXPECT_EQ(tokens[6].kind(), TokenKind::OR);
    EXPECT_EQ(tokens[7].kind(), TokenKind::AND);
    EXPECT_EQ(tokens[8].kind(), TokenKind::LE);
    EXPECT_EQ(tokens[9].kind(), TokenKind::GE);
    EXPECT_EQ(tokens[10].kind(), TokenKind::EQ);
    EXPECT_EQ(tokens[11].kind(), TokenKind::NEQ);
}

TEST(LexerTest, EofToken){
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "x:=5;");

    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[4].kind(), TokenKind::Eof);
    EXPECT_FALSE(tokens[4].has_value());
}

TEST(LexerTest, EofOnEmptyInput) {
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "");

    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].kind(), TokenKind::Eof);
}

TEST(LexerTest, Delimiters){
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "(){};:<>");

    ASSERT_EQ(tokens.size(), 9);
    EXPECT_EQ(tokens[0].kind(), TokenKind::LPAR);
    EXPECT_EQ(tokens[1].kind(), TokenKind::RPAR);
    EXPECT_EQ(tokens[2].kind(), TokenKind::LBRACE);
    EXPECT_EQ(tokens[3].kind(), TokenKind::RBRACE);
    EXPECT_EQ(tokens[4].kind(), TokenKind::SEMICOLON);
    EXPECT_EQ(tokens[5].kind(), TokenKind::COLON);
    EXPECT_EQ(tokens[6].kind(), TokenKind::LT);
    EXPECT_EQ(tokens[7].kind(), TokenKind::GT);
    EXPECT_EQ(tokens[8].kind(), TokenKind::Eof);
}

TEST(LexerTest, TokenPosition){
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "x: int\n = 5;");

    ASSERT_GE(tokens.size(), 7); //<x>,<:>,<int>,<=>,<5>,<;>,<EOF>
    EXPECT_EQ(tokens[0].pos().line, 1);
    EXPECT_EQ(tokens[3].pos().line, 2); // '=' на второй строке
}