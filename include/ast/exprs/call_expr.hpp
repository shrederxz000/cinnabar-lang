// include/ast/exprs/call_expr.hpp
#pragma once
#include <memory>
#include <vector>
#include "utils/pos.hpp"
#include "ast/node.hpp"

namespace cxz::ast {

struct CallExpr final : Expr {
    std::unique_ptr<Expr> callee;
    std::vector<std::unique_ptr<Expr>> arguments;

    CallExpr(std::unique_ptr<Expr> callee,
             std::vector<std::unique_ptr<Expr>> arguments,
             utils::Pos pos)
        : Expr(NodeKind::CallExpr, pos),
          callee(std::move(callee)),
          arguments(std::move(arguments)) {}
};

}
