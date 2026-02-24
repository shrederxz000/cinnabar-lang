// include/ast/stmts/let_stmt.hpp
#pragma once
#include "string"
#include "memory"
#include "ast/node.hpp"

namespace cxz::ast {

        struct LetDecl final : Decl {
            std::string name;
            bool has_const;
            token::TokenKind typing;
            std::unique_ptr<Expr> value;

            LetDecl(std::string name,
                    bool has_const,
                    token::TokenKind typing,
                    std::unique_ptr<Expr> value,
                    utils::Pos pos)
            : Decl(NodeKind::LetDecl, pos),
                    name(std::move(name)),
                    has_const(has_const),
                    typing(typing),
                    value(std::move(value)) {}
        };

}// namespace cxz::ast

