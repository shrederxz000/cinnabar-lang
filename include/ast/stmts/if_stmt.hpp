// include/ast/stmts/if_stmt.hpp
#pragma once
#include "string"
#include "memory"
#include "ast/node.hpp"
#include "ast/program.hpp"

namespace cxz::ast {

        struct IfStmt final : Node {
            std::unique_ptr<Node> condition;
            std::unique_ptr<Block> then_branch;
            std::unique_ptr<Block> else_branch;

            IfStmt(std::unique_ptr<Node> cond,
                   std::unique_ptr<Block> then_b,
                   std::unique_ptr<Block> else_b,
                   utils::Pos pos)
            : Node(NodeKind::IfStmt, pos),
                    condition(std::move(cond)),
                    then_branch(std::move(then_b)),
                    else_branch(std::move(else_b)) {}
        };// struct IfStmt

}// namespace cxz::ast

