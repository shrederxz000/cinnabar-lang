// include/ast/exprs/literal_expr.hpp
#pragma once
#include "cstdint"
#include "string"
#include "variant"
#include "memory"
#include "ast/node.hpp"

namespace cxz::ast {

using LiteralValue = std::variant<std::monostate, int64_t,double,std::string,char,bool>;

struct Literal final : Node {
    LiteralValue value;

    Literal(LiteralValue val, utils::Pos pos)
        : Node(NodeKind::Literal, pos),
          value(std::move(val)) {}
};// struct Literal

}// namespace cxz::ast
