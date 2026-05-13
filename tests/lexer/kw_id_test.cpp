#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

static Lexer make_lexer() {return Lexer{};}

TEST(LexerTest, KeywordsAndIdentifiers){
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test","if else while int float str char bool true false x foo");

    ASSERT_EQ(tokens.size(), 13);
    EXPECT_EQ(tokens[0].kind(), TokenKind::IF);
    EXPECT_EQ(tokens[1].kind(), TokenKind::ELSE);
    EXPECT_EQ(tokens[2].kind(), TokenKind::WHILE);
    EXPECT_EQ(tokens[3].kind(), TokenKind::INT);
    EXPECT_EQ(tokens[4].kind(), TokenKind::FLOAT);
    EXPECT_EQ(tokens[5].kind(), TokenKind::STR);
    EXPECT_EQ(tokens[6].kind(), TokenKind::CHAR);
    EXPECT_EQ(tokens[7].kind(), TokenKind::BOOL);
    EXPECT_EQ(tokens[8].kind(), TokenKind::BOOL_LITERAL);
    EXPECT_EQ(tokens[9].kind(), TokenKind::BOOL_LITERAL);
    EXPECT_EQ(tokens[10].kind(), TokenKind::ID);
    EXPECT_EQ(tokens[11].kind(), TokenKind::ID);
    ASSERT_TRUE(tokens[8].is<bool>());
    EXPECT_TRUE(tokens[8].has_value());
    EXPECT_TRUE(tokens[8].as<bool>());
    ASSERT_TRUE(tokens[9].is<bool>());
    EXPECT_TRUE(tokens[9].has_value());
    EXPECT_FALSE(tokens[9].as<bool>());
    ASSERT_TRUE(tokens[10].is<std::string>());
    EXPECT_EQ(tokens[10].as<std::string>(), "x");
    ASSERT_TRUE(tokens[11].is<std::string>());
    EXPECT_EQ(tokens[11].as<std::string>(), "foo");
}

TEST(LexerTest, LineComment){
    auto lexer = Lexer{};
    auto tokens = lexer.tokenize("test","x := 5; // comment");

    ASSERT_GE(tokens.size(), 5);
    EXPECT_EQ(tokens[0].kind(), TokenKind::ID);
    EXPECT_EQ(tokens[1].kind(), TokenKind::COLON_ASSIGN);
    EXPECT_EQ(tokens[3].kind(), TokenKind::SEMICOLON);
}


TEST(LexerTest, BlockComment){
    auto lexer = Lexer{};
    auto tokens = lexer.tokenize("test", "x/* hidden */ := 10;");

    EXPECT_EQ(tokens[0].as<std::string>(), "x");
    EXPECT_EQ(tokens[1].kind(), TokenKind::COLON_ASSIGN);
}

TEST(LexerTest, BlockCommentNotClosed){
    auto lexer = Lexer{};

    EXPECT_THROW({lexer.tokenize("test","/* ");}, std::runtime_error);
}