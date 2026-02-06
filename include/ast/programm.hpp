// ast/program.hpp
#pragma once
#include "vector"
#include "memory"
#include "ast/node.hpp"

namespace cxz::ast {

struct Program final : Node {
    std::vector<std::unique_ptr<Node>> body;

    Program(pos::Pos pos)
        : Node(NodeKind::Program, pos) {}
};

struct Block final : Node {
    std::vector<std::unique_ptr<Node>> statements;

    Block(pos::Pos pos)
        : Node(NodeKind::Block, pos) {}
};

}
