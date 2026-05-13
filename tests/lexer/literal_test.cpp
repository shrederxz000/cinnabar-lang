#include <gtest/gtest.h>
#include "lexer/lexer.hpp"


static Lexer make_lexer() {return Lexer{};}

// bool
TEST(LexerTest, BoolLiteral){
    auto lexer = Lexer{};
    auto tokens = lexer.tokenize("test", "true false");

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].kind(), TokenKind::BOOL_LITERAL);
    EXPECT_TRUE(tokens[0].as<bool>());
    EXPECT_EQ(tokens[1].kind(), TokenKind::BOOL_LITERAL);
    EXPECT_FALSE(tokens[1].as<bool>());
}

// char
TEST(LexerTest, CharLiteral){
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "'c'");

    ASSERT_EQ(tokens.size(), 2);
}

TEST(LexerTest, CharLiteralLostClosingQuote){
    auto lexer = make_lexer();

    EXPECT_THROW({lexer.tokenize("test", "'c");},std::runtime_error);
}

TEST(LexerTest, CharLiteralEmpty){
    auto lexer = make_lexer();

    EXPECT_THROW({lexer.tokenize("test", "''");}, std::runtime_error);
}

// string
TEST(LexerTest, StringLiteral){
    auto lexer = Lexer{};
    auto tokens = lexer.tokenize("test", "\"hello\"");

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].kind(), TokenKind::STRING_LITERAL);
    ASSERT_TRUE(tokens[0].is<std::string>());
    EXPECT_EQ(tokens[0].as<std::string>(), "hello");
}

TEST(LexerTest, StringLiteralLostClosingQoute){
    auto lexer = Lexer{};

    EXPECT_THROW({lexer.tokenize("test", "\"hello");}, std::runtime_error);
}

TEST(LexerTest, StringWithEscapes){
    auto lexer = Lexer{};
    auto tokens = lexer.tokenize("test",
                                 "\"hello\\nworld\" "
                                 "\"hello\\rworld\" "
                                 "\"hello\\tworld\" "
                                 "\"hello\\aworld\" "
                                 "\"hello\\bworld\" "
                                 "\"hello\\0world\" "
                                 "\"hello\\\"world\\\"\" "
    );

    ASSERT_EQ(tokens.size(), 8);
    EXPECT_EQ(tokens[0].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[0].as<std::string>(), "hello\nworld");
    EXPECT_EQ(tokens[1].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[1].as<std::string>(), "hello\rworld");
    EXPECT_EQ(tokens[2].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[2].as<std::string>(), "hello\tworld");
    EXPECT_EQ(tokens[3].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[3].as<std::string>(), "hello\aworld");
    EXPECT_EQ(tokens[4].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[4].as<std::string>(), "hello\bworld");
    EXPECT_EQ(tokens[5].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[5].as<std::string>(), std::string("hello\0world", 11));
    EXPECT_EQ(tokens[6].kind(), TokenKind::STRING_LITERAL);
    EXPECT_EQ(tokens[6].as<std::string>(), "hello\"world\"");
}

// float
TEST(LexerTest, FloatLiteral) {
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "3.14");

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].kind(), TokenKind::FLOAT_LITERAL);
    ASSERT_TRUE(tokens[0].is<double>());
    EXPECT_DOUBLE_EQ(tokens[0].as<double>(), 3.14);
}

TEST(LexerTest, FloatLiteralWithSemicolon) {
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "3.14;");

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].kind(), TokenKind::FLOAT_LITERAL);
    EXPECT_DOUBLE_EQ(tokens[0].as<double>(), 3.14);
    EXPECT_EQ(tokens[1].kind(), TokenKind::SEMICOLON);
    EXPECT_EQ(tokens[2].kind(), TokenKind::Eof);
}

TEST(LexerTest, FloatLiteralNegative) {
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "-3.14");

    // унарный минус это отдельный токен, не часть литерала
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].kind(), TokenKind::MINUS);
    EXPECT_EQ(tokens[1].kind(), TokenKind::FLOAT_LITERAL);
    EXPECT_DOUBLE_EQ(tokens[1].as<double>(), 3.14);
}

// int
TEST(LexerTest, IntegerLiteral){
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "123456");

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].kind(), TokenKind::INT_LITERAL);
    ASSERT_TRUE(tokens[0].is<int64_t>());
    EXPECT_EQ(tokens[0].as<int64_t>(), 123456);
}

TEST(LexerTest, IntegerLiteralIdInside){
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "123a456");

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].kind(), TokenKind::INT_LITERAL);
    ASSERT_TRUE(tokens[0].is<int64_t>());
    EXPECT_EQ(tokens[0].as<int64_t>(), 123);
    EXPECT_EQ(tokens[1].kind(), TokenKind::ID);
    ASSERT_TRUE(tokens[1].is<std::string>());
    EXPECT_EQ(tokens[1].as<std::string>(), "a456");
}