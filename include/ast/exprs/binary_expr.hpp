// include/ast/exprs/binary_expr.hpp
#pragma once
#include "cstdint"
#include "string"
#include "variant"
#include "memory"
#include "ast/node.hpp"

namespace cxz::ast {

enum class BinaryOp {
    ADD, SUB, MUL, DIV, MOD, POW,
    EQ, NEQ, LT, GT, LE, GE,
};// enum class BinaryOp

struct BinaryExpr final : Node {
    BinaryOp op;
    std::unique_ptr<Node> lhs;
    std::unique_ptr<Node> rhs;

    BinaryExpr(BinaryOp op,
               std::unique_ptr<Node> lhs,
               std::unique_ptr<Node> rhs,
               utils::Pos pos)
        : Node(NodeKind::BinaryExpr, pos),
          op(op),
          lhs(std::move(lhs)),
          rhs(std::move(rhs)) {}
};// struct BinaryExpr

}// namespace cxz::ast
