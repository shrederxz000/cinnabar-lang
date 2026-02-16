// src/parser/parser.cpp
#include "stdexcept"
#include "any"
#include "memory"
#include "lexer/token.hpp"
#include "ast/program.hpp"
#include "ast/stmt.hpp"
#include "parser/parser.hpp"

namespace cxz::parser {

std::unique_ptr<ast::Node> Parser::parse_statement() {

    switch (peek().kind()) {
        case token::TokenKind::LET: {return parse_let();}
        case token::TokenKind::CONST: {return parse_let();}    
        case token::TokenKind::RETURN: {return parse_return();}
            
        default: {return parse_expr_stmt();} 
    }
}

// "let"/"const",<id>,":",<type>,"=", <literal>
    std::unique_ptr<ast::Node> Parser::parse_let() {
        bool is_const = match(token::TokenKind::CONST);

        if (!is_const) {
            expect(token::TokenKind::LET, "expected 'let'");
        }

        expect(token::TokenKind::ID, "expected identifier");
        auto name = tokens_[pos_ - 1];  // токен, который съел expect

        expect(token::TokenKind::COLON, "expected ':'");

        auto typing = advance(); // тип (int / float / etc)

        expect(token::TokenKind::ASSIGN, "expected '='");

        auto value = parse_expression();

        expect(token::TokenKind::SEMICOLON, "expected ';'");

        return std::make_unique<ast::LetStmt>(
                name.as<std::string>(),
                is_const,
                typing.kind(),
                std::move(value),
                name.pos()
        );
    }

// 'return'<expr>';' 
std::unique_ptr<ast::Node> Parser::parse_return() {
    auto kw = advance();
    auto value = parse_expression();
    expect(token::TokenKind::SEMICOLON, "expected ';'");

    return std::make_unique<ast::ReturnStmt>(
        std::move(value),
        kw.pos()
    );
}

}// namespace cxz::parser

