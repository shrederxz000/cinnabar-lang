// include/ast/statements.hpp
#pragma once
#include "ast/node.hpp"
#include "ast/programm.hpp"
#include "memory"
#include "string"

namespace cxz::ast {

struct LetStmt final : Node {
    std::string name;
    bool has_const;
    std::unique_ptr<Node> value;

    LetStmt(std::string name,
            bool has_const,
            std::unique_ptr<Node> value,
            pos::Pos pos)
        : Node(NodeKind::LetStmt, pos),
          name(std::move(name)),
          has_const(has_const),
          value(std::move(value)) {}
};
struct ReturnStmt final : Node {
    std::unique_ptr<Node> value;

    ReturnStmt(std::unique_ptr<Node> value, pos::Pos pos)
        : Node(NodeKind::ReturnStmt, pos),
          value(std::move(value)) {}
};
struct IfStmt final : Node {
    std::unique_ptr<Node> condition;
    std::unique_ptr<Block> then_branch;
    std::unique_ptr<Block> else_branch;

    IfStmt(std::unique_ptr<Node> cond,
           std::unique_ptr<Block> then_b,
           std::unique_ptr<Block> else_b,
           pos::Pos pos)
        : Node(NodeKind::IfStmt, pos),
          condition(std::move(cond)),
          then_branch(std::move(then_b)),
          else_branch(std::move(else_b)) {}
};

}

