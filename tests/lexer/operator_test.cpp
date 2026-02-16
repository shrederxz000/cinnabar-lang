#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}

TEST(LexerTest, Operators)
{
    auto lexer = make_lexer();
    auto tokens = lexer.tokenize("test", 
        "+ "
        "+= "
        "- "
        "-= "
        "/ "
        "/= "
        "* "
        "*= "
        "** "
        "**= "
        "= "
        "@ "
        "! "
        "# "
        "$ "
        "% "
        "%= "
        "^ "
        "& "
        "? "
        "~ "
        "| "
        "|| "
        "&& "
        "<= "
        ">= "
        "== "
        "!= "
        "<< "
        ">> "
        "=> "
        "-> "
    );

    ASSERT_EQ(tokens.size(), 33);

    EXPECT_EQ(tokens[0].kind(), TokenKind::PLUS);
    EXPECT_EQ(tokens[1].kind(), TokenKind::PLUS_ASS);
    EXPECT_EQ(tokens[2].kind(), TokenKind::MINUS);
    EXPECT_EQ(tokens[3].kind(), TokenKind::MINUS_ASS);
    EXPECT_EQ(tokens[4].kind(), TokenKind::SLASH);
    EXPECT_EQ(tokens[5].kind(), TokenKind::SLASH_ASS);
    EXPECT_EQ(tokens[6].kind(), TokenKind::STAR);
    EXPECT_EQ(tokens[7].kind(), TokenKind::STAR_ASS);
    EXPECT_EQ(tokens[8].kind(), TokenKind::POW);
    EXPECT_EQ(tokens[9].kind(), TokenKind::POW_ASS);
    EXPECT_EQ(tokens[10].kind(), TokenKind::ASSIGN);
    EXPECT_EQ(tokens[11].kind(), TokenKind::AT);
    EXPECT_EQ(tokens[12].kind(), TokenKind::BANG);
    EXPECT_EQ(tokens[13].kind(), TokenKind::HASH);
    EXPECT_EQ(tokens[14].kind(), TokenKind::DOLLAR);
    EXPECT_EQ(tokens[15].kind(), TokenKind::PERCENT);
    EXPECT_EQ(tokens[16].kind(), TokenKind::PERCENT_ASS);
    EXPECT_EQ(tokens[17].kind(), TokenKind::CARET);
    EXPECT_EQ(tokens[18].kind(), TokenKind::AMPERSAND);
    EXPECT_EQ(tokens[19].kind(), TokenKind::QUESTION);
    EXPECT_EQ(tokens[20].kind(), TokenKind::TILDE);
    EXPECT_EQ(tokens[21].kind(), TokenKind::PIPE);
    EXPECT_EQ(tokens[22].kind(), TokenKind::OR);
    EXPECT_EQ(tokens[23].kind(), TokenKind::AND);
    EXPECT_EQ(tokens[24].kind(), TokenKind::LE);
    EXPECT_EQ(tokens[25].kind(), TokenKind::GE);
    EXPECT_EQ(tokens[26].kind(), TokenKind::EQ);
    EXPECT_EQ(tokens[27].kind(), TokenKind::NEQ);
    EXPECT_EQ(tokens[28].kind(), TokenKind::SHIFT_LEFT);
    EXPECT_EQ(tokens[29].kind(), TokenKind::SHIFT_RIGHT);
    EXPECT_EQ(tokens[30].kind(), TokenKind::IMPL_ARR);
    EXPECT_EQ(tokens[31].kind(), TokenKind::RETURN_ARR);
    
}
