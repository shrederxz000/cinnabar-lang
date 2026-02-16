#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}

TEST(LexerTest, UnexpectedSyniax)
{
    auto lexer = cxz::lexer::Lexer{};
    EXPECT_THROW({
        lexer.tokenize("test", "авы");
    }, std::runtime_error);
}
