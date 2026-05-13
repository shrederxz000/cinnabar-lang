#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

static Lexer make_lexer() {return Lexer{};}

TEST(LexerTest, UnexpectedSyniax){
    auto lexer = Lexer{};
    EXPECT_THROW({lexer.tokenize("test", "авы");}, std::runtime_error);
}
