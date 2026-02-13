// include/ast/stmts/let_stmt.hpp
#pragma once
#include "string"
#include "memory"
#include "ast/node.hpp"
#include "ast/program.hpp"

namespace cxz::ast {

        struct LetStmt final : Node {
            std::string name;
            bool has_const;
            token::TokenKind typing;
            std::unique_ptr<Node> value;

            LetStmt(std::string name,
                    bool has_const,
                    token::TokenKind typing,
                    std::unique_ptr<Node> value,
                    utils::Pos pos)
            : Node(NodeKind::LetStmt, pos),
                    name(std::move(name)),
                    has_const(has_const),
                    typing(typing),
                    value(std::move(value)) {}
        };

}// namespace cxz::ast

