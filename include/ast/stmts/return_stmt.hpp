// include/ast/stmts/return_stmt.hpp
#pragma once
#include "string"
#include "memory"
#include "ast/node.hpp"

namespace cxz::ast {

        struct ReturnStmt final : Stmt {
            std::unique_ptr<Expr> value;

            ReturnStmt(std::unique_ptr<Expr> value, utils::Pos pos)
            : Stmt(NodeKind::ReturnStmt, pos),
                    value(std::move(value)) {}
        };// struct ReturnStmt

}// namespace cxz::ast

