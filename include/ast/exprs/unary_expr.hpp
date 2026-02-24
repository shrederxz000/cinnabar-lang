// include/ast/exprs/unary_expr.hpp
#pragma once
#include "cstdint"
#include "string"
#include "variant"
#include "memory"
#include "utils/pos.hpp"
#include "ast/node.hpp"

namespace cxz::ast {

enum class UnaryOp {
    PLUS,     // +a
    MINUS,    // -a
    NOT,      // !a
    BIT_NOT,  // ~a
};

struct UnaryExpr final : Expr {
    UnaryOp op;
    std::unique_ptr<Expr> operand;

    UnaryExpr(UnaryOp op,
              std::unique_ptr<Expr> operand,
              utils::Pos pos)
        : Expr(NodeKind::UnaryExpr, pos),
          op(op),
          operand(std::move(operand)) {}
};

}// namespace cxz::ast
