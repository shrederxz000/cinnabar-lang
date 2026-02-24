// include/ast/stmts/expr_stmt.hpp
#pragma once
#include "memory"
#include "utils/pos.hpp"
#include "ast/node.hpp"

namespace cxz::ast{

struct ExprStmt final : Stmt {
    std::unique_ptr<Expr> expr;
};

}
