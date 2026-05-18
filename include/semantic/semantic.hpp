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

enum class Type {
    INT,
    FLOAT,
    STR,
    CHAR,
    BOOL,
    NULL_TYPE,
    UNKNOWN
};

struct TypeInfo {
    Type type;
    bool is_null;

    TypeInfo() : type(Type::UNKNOWN), is_null(false) {}
    TypeInfo(Type t, bool null = false) : type(t), is_null(null) {}
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

    // type checking helpers
    Type token_kind_to_type(TokenKind kind);
    bool is_numeric(Type t);
    bool is_comparable(Type t);
    Type infer_binary_result_type(InfixOp op, Type lhs, Type rhs, const Pos& pos);
    void check_type_compatibility(Type expected, Type actual, const Pos& pos, const std::string& context);

    void error(const Pos& pos, const std::string& msg);

public:
    void check(Program* node);
};