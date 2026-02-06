// include/ast/node.hpp
#pragma once
#include "utils/pos.hpp"

namespace cxz::ast {

enum class NodeKind {
    Program,
    Block,

    // statements
    LetStmt,
    ReturnStmt,
    ExprStmt,
    IfStmt,
    WhileStmt,
    ForStmt,

    // expressions
    BinaryExpr,
    UnaryExpr,
    CallExpr,
    Identifier,
    Literal,

    // async
    // AsyncExpr,
    // AwaitExpr,
    // SpawnExpr,
};

struct Node {
    NodeKind kind;
    pos::Pos pos;

    explicit Node(NodeKind k, pos::Pos p)
        : kind(k), pos(p) {}

    virtual ~Node() = default;
};

}
