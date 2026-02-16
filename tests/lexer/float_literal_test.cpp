#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}

TEST(LexerTest, FloatLiteral)
{
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "3.14");

    ASSERT_EQ(tokens.size(), 2);

    EXPECT_EQ(tokens[0].kind(), TokenKind::FLOAT_LITERAL);
    ASSERT_TRUE(tokens[0].is<double>());
    EXPECT_DOUBLE_EQ(tokens[0].as<double>(), 3.14);
}

TEST(LexerTest, FloatLiteralRange)
{
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "3..14");

    ASSERT_EQ(tokens.size(), 4);

    EXPECT_EQ(tokens[0].kind(), TokenKind::INT_LITERAL);
    ASSERT_TRUE(tokens[0].is<int64_t>());
    EXPECT_EQ(tokens[0].as<int64_t>(), 3);

    EXPECT_EQ(tokens[1].kind(), TokenKind::RANGE);

    EXPECT_EQ(tokens[2].kind(), TokenKind::INT_LITERAL);
    ASSERT_TRUE(tokens[2].is<int64_t>());
    EXPECT_EQ(tokens[2].as<int64_t>(), 14);
}

TEST(LexerTest, FloatLiteralEllipsis)
{
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "3...14");

    ASSERT_EQ(tokens.size(), 4);

    EXPECT_EQ(tokens[0].kind(), TokenKind::INT_LITERAL);
    ASSERT_TRUE(tokens[0].is<int64_t>());
    EXPECT_EQ(tokens[0].as<int64_t>(), 3);

    EXPECT_EQ(tokens[1].kind(), TokenKind::ELLIPSIS);

    EXPECT_EQ(tokens[2].kind(), TokenKind::INT_LITERAL);
    ASSERT_TRUE(tokens[2].is<int64_t>());
    EXPECT_EQ(tokens[2].as<int64_t>(), 14);
}

TEST(LexerTest, FloatLiteralIdInside)
{
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", "3.a.14");

    ASSERT_EQ(tokens.size(), 6);

    EXPECT_EQ(tokens[0].kind(), TokenKind::INT_LITERAL);
    ASSERT_TRUE(tokens[0].is<int64_t>());
    EXPECT_EQ(tokens[0].as<int64_t>(), 3);

    EXPECT_EQ(tokens[2].kind(), TokenKind::ID);
    ASSERT_TRUE(tokens[2].is<std::string>());
    EXPECT_EQ(tokens[2].as<std::string>(), "a");

    EXPECT_EQ(tokens[4].kind(), TokenKind::INT_LITERAL);
    ASSERT_TRUE(tokens[4].is<int64_t>());
    EXPECT_EQ(tokens[4].as<int64_t>(), 14);
}
