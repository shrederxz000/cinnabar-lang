#pragma once
#include "memory"
#include "utils.hpp"
#include "ast/node.hpp"
#include <vector>
#include "utils.hpp"
#include "lexer/token.hpp"

struct Block final : Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;

    Block(Pos pos): Stmt(NodeKind::BlockStmt, pos) {}
};

struct ExprStmt final : Stmt {
    std::unique_ptr<Expr> expr;
    ExprStmt(std::unique_ptr<Expr> expression, Pos pos)
    : Stmt(NodeKind::ExprStmt, pos), expr(std::move(expression)){}
};

struct IfStmt final : Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Block> then_branch;
    std::unique_ptr<Block> else_branch;

    IfStmt(std::unique_ptr<Expr> cond,std::unique_ptr<Block> then_b, std::unique_ptr<Block> else_b, Pos pos)
        : Stmt(NodeKind::IfStmt, pos), condition(std::move(cond)), then_branch(std::move(then_b)), else_branch(std::move(else_b)) {}
};

struct VarStmt final : Stmt {
    std::string name;
    TokenKind var_type_kind; // это поле потом проверяется семантикой на соответствие типов
    std::unique_ptr<Expr> value;

    VarStmt(std::string name, TokenKind type_kind, std::unique_ptr<Expr> value, Pos pos)
            : Stmt(NodeKind::VarStmt, pos), name(std::move(name)), var_type_kind(type_kind), value(std::move(value)) {}
};

struct WhileStmt final : Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Block> loop_body;

    WhileStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Block> body, Pos pos)
            : Stmt(NodeKind::WhileStmt, pos), condition(std::move(cond)), loop_body(std::move(body)) {}
};

