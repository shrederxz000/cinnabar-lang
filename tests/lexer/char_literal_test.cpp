#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}
TEST(LexerTest, CharLiteral)
{
auto lexer = make_lexer();
auto tokens = lexer.tokenize("test", "'c'");

ASSERT_EQ(tokens.size(), 2);

}

TEST(LexerTest, CharLiteralLostClosingQuote)
{
    auto lexer = make_lexer();

    EXPECT_THROW({
        lexer.tokenize("test", "'c");
    }, std::runtime_error);
}

TEST(LexerTest, CharLiteralEmpty)
{
    auto lexer = make_lexer();

    EXPECT_THROW({
        lexer.tokenize("test", "''");
    }, std::runtime_error);
}

