// include/parsrer/parser.cpp
#pragma once
#include "vector"
#include "memory"
#include "tuple"
#include "lexer/token.hpp"
#include "ast/program.hpp"
#include "ast/expr.hpp"
#include "ast/stmt.hpp"

namespace cxz::parser {

class Parser {
public:
    explicit Parser(const std::vector<token::Token>& tokens);

    std::unique_ptr<ast::Program> parse_program();

private:
    const std::vector<token::Token>& tokens_;
    size_t pos_ = 0;

    // helpers
    const token::Token& peek(size_t offset = 0) const;
    const token::Token& advance();
    bool match(token::TokenKind kind);
    void expect(token::TokenKind kind, const char* msg);

    // statements
    std::unique_ptr<ast::Node> parse_statement();
    std::unique_ptr<ast::Node> parse_let();
    std::unique_ptr<ast::Node> parse_return();
    std::unique_ptr<ast::Node> parse_expr_stmt();
    std::unique_ptr<ast::Block> parse_block();

    // expressions (Pratt)
    std::unique_ptr<ast::Node> parse_expression(int min_prec = 0);
    std::unique_ptr<ast::Node> parse_prefix();

    std::tuple<int, int> precedence(token::TokenKind kind) const;
};

}
