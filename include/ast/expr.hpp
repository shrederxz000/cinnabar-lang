#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <memory>
#include <variant>
#include "utils.hpp"
#include "ast/node.hpp"

enum class PrefixOp {
    MINUS, // -a
    NOT,   // !a
};

enum class InfixOp {
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    EQ,
    NEQ,
    LT,
    GT,
    LE,
    GE,
    OR,
    AND,
};

struct AssignExpr final : Expr {
    std::unique_ptr<Expr> target;
    std::unique_ptr<Expr> value;

    AssignExpr(std::unique_ptr<Expr> target, std::unique_ptr<Expr> value, Pos pos)
        : Expr(NodeKind::AssignExpr, pos), target(std::move(target)), value(std::move(value)) {}
};

struct BinaryExpr final : Expr {
    InfixOp op;
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    BinaryExpr(InfixOp op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs, Pos pos)
        : Expr(NodeKind::BinaryExpr, pos), op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
};

struct UnaryExpr final : Expr {
    PrefixOp op;
    std::unique_ptr<Expr> operand;

    UnaryExpr(PrefixOp op, std::unique_ptr<Expr> operand, Pos pos)
        : Expr(NodeKind::UnaryExpr, pos), op(op), operand(std::move(operand)) {}
};

struct IdentifierExpr final : Expr {
    std::string name;

    IdentifierExpr(std::string name, Pos pos)
        : Expr(NodeKind::IdentifierExpr, pos), name(std::move(name)) {}
};

using LiteralValue = std::variant<std::monostate, int64_t, double, std::string, char, bool>;

struct LiteralExpr final : Expr {
    LiteralValue value;

    LiteralExpr(LiteralValue val, Pos pos)
        : Expr(NodeKind::LiteralExpr, pos), value(std::move(val)) {}
};

struct CallExpr final : Expr {
    std::unique_ptr<Expr> callee;
    std::vector<std::unique_ptr<Expr>> args;

    CallExpr(std::unique_ptr<Expr> callee, std::vector<std::unique_ptr<Expr>> args, Pos pos)
        : Expr(NodeKind::CallExpr, pos), callee(std::move(callee)), args(std::move(args)) {}
};
