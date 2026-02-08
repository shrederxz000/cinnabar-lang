// include/ast/node.hpp
#pragma once
#include "utils/pos.hpp"

namespace cxz::ast {

enum class NodeKind {
    Program,
    Block,
    LetStmt,
    ReturnStmt,
    ExprStmt,
    IfStmt,
    WhileStmt,
    ForStmt,
    BinaryExpr,
    UnaryExpr,
    CallExpr,
    Identifier,
    Literal,
    // AsyncExpr,
    // AwaitExpr,
    // SpawnExpr,
};// enum class NodeKind

struct Node {
    NodeKind kind;
    utils::Pos pos;

    explicit Node(NodeKind k, utils::Pos p)
        : kind(k), pos(p) {}

    virtual ~Node() = default;
};// struct Node

}// namespace cxz::ast
