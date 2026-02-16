#include <gtest/gtest.h>
#include "lexer/lexer.hpp"

using namespace cxz::token;

static cxz::lexer::Lexer make_lexer() {
    return cxz::lexer::Lexer{};
}

TEST(LexerTest, KeywordsAndIdentifiers)
{
auto lexer = make_lexer();
auto tokens = lexer.tokenize("test",
                             "if "
                             "else "
                             "switch "
                             "case "
                             "default "
                             "let "
                             "const "
                             "for "
                             "while "
                             "break "
                             "continue "
                             "def "
                             "return "
                             "int "
                             "float "
                             "str "
                             "char "
                             "static "
                             "pub "
                             "struct "
                             "scope "
                             "del "
                             "bool "
                             "true "
                             "false "
                             "x "
                             "foo "
                             );

ASSERT_EQ(tokens.size(), 28);

EXPECT_EQ(tokens[0].kind(), TokenKind::IF);
    EXPECT_EQ(tokens[1].kind(), TokenKind::ELSE);
    EXPECT_EQ(tokens[2].kind(), TokenKind::SWITCH);
    EXPECT_EQ(tokens[3].kind(), TokenKind::CASE);
    EXPECT_EQ(tokens[4].kind(), TokenKind::DEFAULT);
    EXPECT_EQ(tokens[5].kind(), TokenKind::LET);
    EXPECT_EQ(tokens[6].kind(), TokenKind::CONST);
    EXPECT_EQ(tokens[7].kind(), TokenKind::FOR);
    EXPECT_EQ(tokens[8].kind(), TokenKind::WHILE);
    EXPECT_EQ(tokens[9].kind(), TokenKind::BREAK);
    EXPECT_EQ(tokens[10].kind(), TokenKind::CONTINUE);

    EXPECT_EQ(tokens[11].kind(), TokenKind::DEF);
    EXPECT_EQ(tokens[12].kind(), TokenKind::RETURN);
    EXPECT_EQ(tokens[13].kind(), TokenKind::INT);
    EXPECT_EQ(tokens[14].kind(), TokenKind::FLOAT);
    EXPECT_EQ(tokens[15].kind(), TokenKind::STR);
    EXPECT_EQ(tokens[16].kind(), TokenKind::CHAR);
    EXPECT_EQ(tokens[17].kind(), TokenKind::STATIC);
    EXPECT_EQ(tokens[18].kind(), TokenKind::PUBLIC);
    EXPECT_EQ(tokens[19].kind(), TokenKind::STRUCT);
    EXPECT_EQ(tokens[20].kind(), TokenKind::SCOPE);

    EXPECT_EQ(tokens[21].kind(), TokenKind::DELETE);
    EXPECT_EQ(tokens[22].kind(), TokenKind::BOOL);
    EXPECT_EQ(tokens[23].kind(), TokenKind::BOOL_LITERAL);
    EXPECT_EQ(tokens[24].kind(), TokenKind::BOOL_LITERAL);
    EXPECT_EQ(tokens[25].kind(), TokenKind::ID);
    EXPECT_EQ(tokens[26].kind(), TokenKind::ID);

    ASSERT_TRUE(tokens[23].is<bool>());
    EXPECT_TRUE(tokens[23].has_value());
    EXPECT_TRUE(tokens[23].as<bool>());

    ASSERT_TRUE(tokens[24].is<bool>());
    EXPECT_TRUE(tokens[24].has_value());
    EXPECT_FALSE(tokens[24].as<bool>());

    
    ASSERT_TRUE(tokens[25].is<std::string>());
    EXPECT_EQ(tokens[25].as<std::string>(), "x");

    ASSERT_TRUE(tokens[26].is<std::string>());
    EXPECT_EQ(tokens[26].as<std::string>(), "foo");

}
