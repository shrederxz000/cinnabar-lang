// include/ast/exprs/identifier_exprs.hpp
#pragma once
#include "cstdint"
#include "string"
#include "variant"
#include "memory"
#include "utils/pos.hpp"
#include "ast/node.hpp"

namespace cxz::ast {

struct Identifier final : Expr {
    std::string name;

    Identifier(std::string name, utils::Pos pos)
        : Expr(NodeKind::Identifier, pos),
          name(std::move(name)) {}
};// struct Identifier

}// namespace cxz::ast
