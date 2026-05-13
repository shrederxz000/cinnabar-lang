#include "stdexcept"
#include "vector"
#include "memory"
#include "lexer/token.hpp"
#include "ast/node.hpp"
#include "parser/parser.hpp"

Parser::Parser(const std::vector<Token>& tokens)
        : tokens_(tokens) {}

std::unique_ptr<Program> Parser::parse_program() {
    std::unique_ptr<Program> prog = std::make_unique<Program>(peek().pos());

    while (peek().kind() != TokenKind::Eof) {
        prog->body.push_back(parse_statement());
    }

    return prog;
}

std::unique_ptr<Block> Parser::parse_block() {
    std::unique_ptr<Block> block = std::make_unique<Block>(peek().pos());
    expect(TokenKind::LBRACE, "expected '{'");

    while (!check(TokenKind::RBRACE) && !check(TokenKind::Eof)) {
        block->statements.push_back(parse_statement());
    }

    expect(TokenKind::RBRACE, "expected '}'");
    return block;
}



