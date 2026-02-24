// include/ast/node.hpp
#pragma once
#include "utils/pos.hpp"
#include "vector"

namespace cxz::ast {

enum class NodeKind {
    Program,
    Block,
    LetDecl,
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
    FuncDecl,
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

struct Stmt : Node {
    using Node::Node;
    virtual ~Stmt() = default;
};

struct Expr : Node {
    using Node::Node;
    virtual ~Expr() = default;
};

struct Decl : Node {
    using Node::Node;
    virtual ~Decl() = default;
};

struct Program final : Node {
    std::vector<std::unique_ptr<Node>> body;
    Program(utils::Pos pos)
        : Node(NodeKind::Program, pos) {}
};// struct Program

struct Block final : Node {
    std::vector<std::unique_ptr<Stmt>> statements;

    Block(utils::Pos pos)
        : Node(NodeKind::Block, pos) {}
};// struct Block

}// namespace cxz::ast
