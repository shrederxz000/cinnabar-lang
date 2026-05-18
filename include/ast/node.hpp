#pragma once
#include "utils.hpp"
#include "vector"
#include "memory"

enum class NodeKind {
    Program,
    BlockStmt,
    VarStmt,
    ExprStmt,
    IfStmt,
    WhileStmt,

    AssignExpr,
    BinaryExpr,
    UnaryExpr,
    IdentifierExpr,
    LiteralExpr,
    CallExpr
};

struct Node {
    NodeKind kind;
    Pos pos;
    explicit Node(NodeKind k, Pos p): kind(k), pos(p) {}
    virtual ~Node() = default;
};

struct Stmt : Node {
    using Node::Node;
    virtual ~Stmt() = default;
};

struct Expr : Node {
    using Node::Node;
    virtual ~Expr() = default;
};

struct Program final : Node {
    std::vector<std::unique_ptr<Node>> body;
    Program(Pos pos): Node(NodeKind::Program, pos) {}
};


