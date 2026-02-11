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
private:
    const std::vector<token::Token>& tokens_;
    size_t pos_ = 0;
    const token::Token& peek(size_t offset = 0) const;
    const token::Token& advance();
    bool match(token::TokenKind kind);
    void expect(token::TokenKind kind, const char* msg);
    std::unique_ptr<ast::Node> parse_statement();

    
    std::unique_ptr<ast::Node> parse_let();

    std::unique_ptr<ast::Node> parse_return();
    std::unique_ptr<ast::Node> parse_expr_stmt();
    std::unique_ptr<ast::Block> parse_block();
    bool is_binary_op(token::TokenKind kind) const;
    std::unique_ptr<ast::Node> parse_expression(int min_prec = 0);
    std::unique_ptr<ast::Node> parse_prefix();
    static std::tuple<int, int> precedence(token::TokenKind kind) ;
public:
    explicit Parser(const std::vector<token::Token>& tokens);

    std::unique_ptr<ast::Program> parse_program();
}; // class Parser

}//namespace cxz::parser
