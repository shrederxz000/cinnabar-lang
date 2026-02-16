#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}

TEST(LexerTest, IntegerLiteral)
{
auto lexer = make_lexer();
auto tokens = lexer.tokenize("test", "123456");

ASSERT_EQ(tokens.size(), 2);

EXPECT_EQ(tokens[0].kind(), TokenKind::INT_LITERAL);
ASSERT_TRUE(tokens[0].is<int64_t>());
EXPECT_EQ(tokens[0].as<int64_t>(), 123456);
}

TEST(LexerTest, IntegerLiteralIdInside)
{
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
