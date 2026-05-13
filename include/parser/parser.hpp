#pragma once
#include "vector"
#include "memory"
#include "tuple"
#include "lexer/token.hpp"
#include "ast/node.hpp"
#include "ast/expr.hpp"
#include "ast/stmt.hpp"

class Parser {
private:
    const std::vector<Token>& tokens_;
    size_t pos_ = 0;

    // stmts
    std::unique_ptr<Stmt> parse_statement();
    std::unique_ptr<Stmt> parse_var_stmt();
    std::unique_ptr<Stmt> parse_expr_stmt();
    std::unique_ptr<Stmt> parse_if_stmt();
    std::unique_ptr<Stmt> parse_while_stmt();

    // exprs
    std::unique_ptr<Expr> parse_expression(int min_prec = 0);
    std::unique_ptr<Expr> parse_prefix();
    static std::tuple<int, int> precedence(TokenKind kind);
    std::unique_ptr<Expr> parse_call(std::unique_ptr<Expr> callee);

    // utils
    std::unique_ptr<Block> parse_block();
    bool is_binary_op(TokenKind kind) const;
    const Token& peek(size_t offset = 0) const;
    const Token& advance();
    bool match(TokenKind kind);
    void expect(TokenKind kind, const char* msg);
    bool check(TokenKind kind) const;

public:
    explicit Parser(const std::vector<Token>& tokens);
    std::unique_ptr<Program> parse_program();
};