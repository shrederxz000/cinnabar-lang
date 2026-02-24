// include/ast/assign_expr.hpp
#pragma once
#include "memory"
#include "utils/pos.hpp"
#include "ast/node.hpp"

namespace cxz::ast {

struct AssignExpr final : Expr {
    std::unique_ptr<Expr> target;
    std::unique_ptr<Expr> value;
};

}// namespace cxz::ast
