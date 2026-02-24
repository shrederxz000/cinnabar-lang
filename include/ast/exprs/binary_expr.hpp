// include/ast/exprs/binary_expr.hpp
#pragma once
#include "cstdint"
#include "string"
#include "variant"
#include "memory"
#include "utils/pos.hpp"
#include "ast/node.hpp"

namespace cxz::ast {

enum class BinaryOp {
    ADD, SUB, MUL, DIV, MOD, POW,
    EQ, NEQ, LT, GT, LE, GE,
};// enum class BinaryOp

struct BinaryExpr final : Expr {
    BinaryOp op;
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    BinaryExpr(BinaryOp op,
               std::unique_ptr<Expr> lhs,
               std::unique_ptr<Expr> rhs,
               utils::Pos pos)
        : Expr(NodeKind::BinaryExpr, pos),
          op(op),
          lhs(std::move(lhs)),
          rhs(std::move(rhs)) {}
};// struct BinaryExpr

}// namespace cxz::ast
