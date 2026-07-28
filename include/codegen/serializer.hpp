#pragma once
#include <string>
#include <sstream>
#include "ast/expr.hpp"
#include "ast/node.hpp"
#include "ast/stmt.hpp"

class ASTSerializer {
  public:
    static std::string to_json(const Node* node);

  private:
    static void serialize_node(std::ostringstream& out, const Node* node, int indent = 0);
    static void serialize_program(std::ostringstream& out, const Program* node, int indent);
    static void serialize_block(std::ostringstream& out, const Block* node, int indent);
    static void serialize_var_stmt(std::ostringstream& out, const VarStmt* node, int indent);
    static void serialize_if_stmt(std::ostringstream& out, const IfStmt* node, int indent);
    static void serialize_while_stmt(std::ostringstream& out, const WhileStmt* node, int indent);
    static void serialize_expr_stmt(std::ostringstream& out, const ExprStmt* node, int indent);
    static void serialize_binary_expr(std::ostringstream& out, const BinaryExpr* node, int indent);
    static void serialize_unary_expr(std::ostringstream& out, const UnaryExpr* node, int indent);
    static void serialize_assign_expr(std::ostringstream& out, const AssignExpr* node, int indent);
    static void serialize_call_expr(std::ostringstream& out, const CallExpr* node, int indent);
    static void serialize_identifier_expr(std::ostringstream& out, const IdentifierExpr* node,
                                          int indent);
    static void serialize_literal_expr(std::ostringstream& out, const LiteralExpr* node,
                                       int indent);

    static std::string escape_json_string(const std::string& str);
    static std::string infix_op_to_string(InfixOp op);
    static std::string prefix_op_to_string(PrefixOp op);
};
