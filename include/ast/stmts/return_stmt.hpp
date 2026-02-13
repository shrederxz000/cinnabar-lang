// include/ast/stmts/return_stmt.hpp
#pragma once
#include "string"
#include "memory"
#include "ast/node.hpp"
#include "ast/program.hpp"

namespace cxz::ast {

        struct ReturnStmt final : Node {
            std::unique_ptr<Node> value;

            ReturnStmt(std::unique_ptr<Node> value, utils::Pos pos)
            : Node(NodeKind::ReturnStmt, pos),
                    value(std::move(value)) {}
        };// struct ReturnStmt

}// namespace cxz::ast

