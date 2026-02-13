// src/parser/parser.cpp
#include "stdexcept"
#include "vector"
#include "tuple"
#include "any"
#include "memory"
#include "lexer/token.hpp"
#include "ast/program.hpp"
#include "ast/expr.hpp"
#include "ast/stmt.hpp"
#include "parser/parser.hpp"

namespace cxz::parser {

Parser::Parser(const std::vector<token::Token>& tokens)
    : tokens_(tokens) {}

// MAIN FUNCTION
std::unique_ptr<ast::Program> Parser::parse_program() {
    auto prog = std::make_unique<ast::Program>(peek().pos());

    while (peek().kind() != token::TokenKind::Eof) {
        prog->body.push_back(parse_statement());
    }

    return prog;
}

//'{' <stmt> '}' 
std::unique_ptr<ast::Block> Parser::parse_block() {
    auto block = std::make_unique<ast::Block>(peek().pos());
    expect(token::TokenKind::LBRACE, "expected '{'");

    while (peek().kind() != token::TokenKind::RBRACE) {
        block->statements.push_back(parse_statement());
    }

    expect(token::TokenKind::RBRACE, "expected '}'");
    return block;
}

}// namespace cxz::parser
