// include/ast/stmts/function_decl.hpp
#pragma once
#include <memory>
#include <vector>
#include "utils/pos.hpp"
#include "ast/node.hpp"

namespace cxz::ast {

    struct Param {
        std::string name;
        std::string type;
        std::unique_ptr<Expr> default_value; // optional

        Param(
                std::string name,
                std::string type,
                std::unique_ptr<Expr> default_value = nullptr
        )
                : name(std::move(name)),
                  type(std::move(type)),
                  default_value(std::move(default_value)) {}
    };

    struct FuncDecl : Decl {
        std::string name;
        std::vector<Param> params;
        std::string return_type;
        std::unique_ptr<Block> body;

        FuncDecl(
                std::string name,
                std::vector<Param> params,
                std::string return_type,
                std::unique_ptr<Block> body,
                utils::Pos pos
        )
                : Decl(NodeKind::FuncDecl, pos),
                  name(std::move(name)),
                  params(std::move(params)),
                  return_type(std::move(return_type)),
                  body(std::move(body)) {}
    };

}