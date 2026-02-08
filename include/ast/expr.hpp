// include/ast/expr.hpp
#pragma once
#include "cstdint"
#include "string"
#include "variant"
#include "memory"
#include "ast/node.hpp"

namespace cxz::ast {

using LiteralValue = std::variant<
    int64_t,
    double,
    std::string,
    char,
    bool
>;

struct Literal final : Node {
    LiteralValue value;

    Literal(LiteralValue val, pos::Pos pos)
        : Node(NodeKind::Literal, pos),
          value(std::move(val)) {}
};

struct Identifier final : Node {
    std::string name;

    Identifier(std::string name, pos::Pos pos)
        : Node(NodeKind::Identifier, pos),
          name(std::move(name)) {}
};

enum class BinaryOp {
    ADD, SUB, MUL, DIV, MOD, POW,
    EQ, NEQ, LT, GT, LE, GE,
};

struct BinaryExpr final : Node {
    BinaryOp op;
    std::unique_ptr<Node> lhs;
    std::unique_ptr<Node> rhs;

    BinaryExpr(BinaryOp op,
               std::unique_ptr<Node> lhs,
               std::unique_ptr<Node> rhs,
               pos::Pos pos)
        : Node(NodeKind::BinaryExpr, pos),
          op(op),
          lhs(std::move(lhs)),
          rhs(std::move(rhs)) {}
};

}
