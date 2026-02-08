// include/ast/program.hpp
#pragma once
#include "vector"
#include "memory"
#include "ast/node.hpp"

namespace cxz::ast {

struct Program final : Node {
    std::vector<std::unique_ptr<Node>> body;

    Program(utils::Pos pos)
        : Node(NodeKind::Program, pos) {}
};// struct Program

struct Block final : Node {
    std::vector<std::unique_ptr<Node>> statements;

    Block(utils::Pos pos)
        : Node(NodeKind::Block, pos) {}
};// struct Block

}// namespace cxz::ast
