// include/semantic/semantic.hpp
#pragma once
#include "vector"
#include "string"
#include "unordered_map"
#include "unordered_set"
#include "memory"
#include "lexer/token.hpp"
#include "ast/node.hpp"
#include "ast/expr.hpp"
#include "ast/stmt.hpp"

struct TypeInfo {
    TokenKind type;    // INT, FLOAT, STR, CHAR, BOOL, Null
//    bool is_const;
    bool is_null;
};

class SemanticVisitor {
private:
    // стек scope-ов
    std::vector<std::unordered_map<std::string, TypeInfo>> scopes_;

    // встроенные функции
    static const std::unordered_set<std::string> builtins_;

    void push_scope();
    void pop_scope();
    TypeInfo* lookup(const std::string& name);
    void declare(const std::string& name, TypeInfo info);

    // проверка узлов
    void check_stmt(Stmt* node);
    void check_var_stmt(VarStmt* node);
    void check_if(IfStmt* node);
    void check_while(WhileStmt* node);
    void check_block(Block* node);


    TypeInfo check_assign(AssignExpr* node);
    TypeInfo check_expr(Expr* node);
    TypeInfo check_binary(BinaryExpr* node);
    TypeInfo check_unary(UnaryExpr* node);
    TypeInfo check_call(CallExpr* node);
    TypeInfo check_identifier(IdentifierExpr* node);
    TypeInfo check_literal(LiteralExpr* node);

    void error(const Pos& pos, const std::string& msg);

public:
    void check(Program* node);
};