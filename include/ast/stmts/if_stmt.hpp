// include/ast/stmts/if_stmt.hpp
#pragma once
#include "string"
#include "memory"
#include "ast/node.hpp"

namespace cxz::ast {

        struct IfStmt final : Stmt {
            std::unique_ptr<Expr> condition;
            std::unique_ptr<Block> then_branch;
            std::unique_ptr<Block> else_branch;

            IfStmt(std::unique_ptr<Expr> cond,
                   std::unique_ptr<Block> then_b,
                   std::unique_ptr<Block> else_b,
                   utils::Pos pos)
            : Stmt(NodeKind::IfStmt, pos),
                    condition(std::move(cond)),
                    then_branch(std::move(then_b)),
                    else_branch(std::move(else_b)) {}
        };// struct IfStmt

}// namespace cxz::ast

