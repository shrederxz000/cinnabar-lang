#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}

TEST(LexerTest, LineComment)
{
    auto lexer = cxz::lexer::Lexer{};
    auto tokens = lexer.tokenize("test",
        "let x = 5; // comment");

    ASSERT_GE(tokens.size(), 6);

    EXPECT_EQ(tokens[0].kind(), TokenKind::LET);
}


TEST(LexerTest, BlockComment)
{
    auto lexer = cxz::lexer::Lexer{};
    auto tokens = lexer.tokenize("test",
        "let /* hidden */ x = 10;");

    EXPECT_EQ(tokens[1].kind(), TokenKind::ID);
    EXPECT_EQ(tokens[1].as<std::string>(), "x");
}

TEST(LexerTest, BlockCommentNotClosed)
{
    auto lexer = cxz::lexer::Lexer{};

    EXPECT_THROW({lexer.tokenize("test",
        "/* ");
    }, std::runtime_error);

}
