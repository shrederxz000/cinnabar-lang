#include "codegen/serializer.hpp"
#include <memory>
#include <variant>

std::string ASTSerializer::escape_json_string(const std::string& str) {
    std::ostringstream out;
    for (char c : str) {
        switch (c) {
        case '"':
            out << "\\\"";
            break;
        case '\\':
            out << "\\\\";
            break;
        case '\n':
            out << "\\n";
            break;
        case '\r':
            out << "\\r";
            break;
        case '\t':
            out << "\\t";
            break;
        default:
            out << c;
            break;
        }
    }
    return out.str();
}

std::string ASTSerializer::infix_op_to_string(InfixOp op) {
    switch (op) {
    case InfixOp::ADD:
        return "Add";
    case InfixOp::SUB:
        return "Sub";
    case InfixOp::MUL:
        return "Mult";
    case InfixOp::DIV:
        return "Div";
    case InfixOp::POW:
        return "Pow";
    case InfixOp::EQ:
        return "Eq";
    case InfixOp::NEQ:
        return "NotEq";
    case InfixOp::LT:
        return "Lt";
    case InfixOp::GT:
        return "Gt";
    case InfixOp::LE:
        return "LtE";
    case InfixOp::GE:
        return "GtE";
    case InfixOp::OR:
        return "Or";
    case InfixOp::AND:
        return "And";
    default:
        return "Unknown";
    }
}

std::string ASTSerializer::prefix_op_to_string(PrefixOp op) {
    switch (op) {
    case PrefixOp::MINUS:
        return "USub";
    case PrefixOp::NOT:
        return "Not";
    default:
        return "Unknown";
    }
}

std::string ASTSerializer::to_json(const Node* node) {
    std::ostringstream out;
    serialize_node(out, node, 0);
    return out.str();
}

void ASTSerializer::serialize_node(std::ostringstream& out, const Node* node, int indent) {
    switch (node->kind) {
    case NodeKind::Program:
        serialize_program(out, static_cast<const Program*>(node), indent);
        break;
    case NodeKind::BlockStmt:
        serialize_block(out, static_cast<const Block*>(node), indent);
        break;
    case NodeKind::VarStmt:
        serialize_var_stmt(out, static_cast<const VarStmt*>(node), indent);
        break;
    case NodeKind::IfStmt:
        serialize_if_stmt(out, static_cast<const IfStmt*>(node), indent);
        break;
    case NodeKind::WhileStmt:
        serialize_while_stmt(out, static_cast<const WhileStmt*>(node), indent);
        break;
    case NodeKind::ExprStmt:
        serialize_expr_stmt(out, static_cast<const ExprStmt*>(node), indent);
        break;
    case NodeKind::BinaryExpr:
        serialize_binary_expr(out, static_cast<const BinaryExpr*>(node), indent);
        break;
    case NodeKind::UnaryExpr:
        serialize_unary_expr(out, static_cast<const UnaryExpr*>(node), indent);
        break;
    case NodeKind::AssignExpr:
        serialize_assign_expr(out, static_cast<const AssignExpr*>(node), indent);
        break;
    case NodeKind::CallExpr:
        serialize_call_expr(out, static_cast<const CallExpr*>(node), indent);
        break;
    case NodeKind::IdentifierExpr:
        serialize_identifier_expr(out, static_cast<const IdentifierExpr*>(node), indent);
        break;
    case NodeKind::LiteralExpr:
        serialize_literal_expr(out, static_cast<const LiteralExpr*>(node), indent);
        break;
    default:
        out << "{\"kind\": \"Unknown\"}";
    }
}

void ASTSerializer::serialize_program(std::ostringstream& out, const Program* node, int indent) {
    out << "{\n";
    out << std::string(indent + 2, ' ') << "\"kind\": \"Program\",\n";
    out << std::string(indent + 2, ' ') << "\"body\": [\n";
    for (size_t i = 0; i < node->body.size(); ++i) {
        out << std::string(indent + 4, ' ');
        serialize_node(out, node->body[i].get(), indent + 4);
        if (i < node->body.size() - 1)
            out << ",";
        out << "\n";
    }
    out << std::string(indent + 2, ' ') << "]\n";
    out << std::string(indent, ' ') << "}";
}

void ASTSerializer::serialize_block(std::ostringstream& out, const Block* node, int indent) {
    out << "{\n";
    out << std::string(indent + 2, ' ') << "\"kind\": \"Block\",\n";
    out << std::string(indent + 2, ' ') << "\"statements\": [\n";
    for (size_t i = 0; i < node->statements.size(); ++i) {
        out << std::string(indent + 4, ' ');
        serialize_node(out, node->statements[i].get(), indent + 4);
        if (i < node->statements.size() - 1)
            out << ",";
        out << "\n";
    }
    out << std::string(indent + 2, ' ') << "]\n";
    out << std::string(indent, ' ') << "}";
}

void ASTSerializer::serialize_var_stmt(std::ostringstream& out, const VarStmt* node, int indent) {
    out << "{\n";
    out << std::string(indent + 2, ' ') << "\"kind\": \"VarStmt\",\n";
    out << std::string(indent + 2, ' ') << "\"name\": \"" << node->name << "\",\n";
    out << std::string(indent + 2, ' ') << "\"value\": ";
    serialize_node(out, node->value.get(), indent + 2);
    out << "\n" << std::string(indent, ' ') << "}";
}

void ASTSerializer::serialize_if_stmt(std::ostringstream& out, const IfStmt* node, int indent) {
    out << "{\n";
    out << std::string(indent + 2, ' ') << "\"kind\": \"IfStmt\",\n";
    out << std::string(indent + 2, ' ') << "\"condition\": ";
    serialize_node(out, node->condition.get(), indent + 2);
    out << ",\n";
    out << std::string(indent + 2, ' ') << "\"then_branch\": ";
    serialize_node(out, node->then_branch.get(), indent + 2);
    if (node->else_branch) {
        out << ",\n";
        out << std::string(indent + 2, ' ') << "\"else_branch\": ";
        serialize_node(out, node->else_branch.get(), indent + 2);
    }
    out << "\n" << std::string(indent, ' ') << "}";
}

void ASTSerializer::serialize_while_stmt(std::ostringstream& out, const WhileStmt* node,
                                         int indent) {
    out << "{\n";
    out << std::string(indent + 2, ' ') << "\"kind\": \"WhileStmt\",\n";
    out << std::string(indent + 2, ' ') << "\"condition\": ";
    serialize_node(out, node->condition.get(), indent + 2);
    out << ",\n";
    out << std::string(indent + 2, ' ') << "\"body\": ";
    serialize_node(out, node->loop_body.get(), indent + 2);
    out << "\n" << std::string(indent, ' ') << "}";
}

void ASTSerializer::serialize_expr_stmt(std::ostringstream& out, const ExprStmt* node, int indent) {
    out << "{\n";
    out << std::string(indent + 2, ' ') << "\"kind\": \"ExprStmt\",\n";
    out << std::string(indent + 2, ' ') << "\"expr\": ";
    serialize_node(out, node->expr.get(), indent + 2);
    out << "\n" << std::string(indent, ' ') << "}";
}

void ASTSerializer::serialize_binary_expr(std::ostringstream& out, const BinaryExpr* node,
                                          int indent) {
    out << "{\n";
    out << std::string(indent + 2, ' ') << "\"kind\": \"BinaryExpr\",\n";
    out << std::string(indent + 2, ' ') << "\"op\": \"" << infix_op_to_string(node->op) << "\",\n";
    out << std::string(indent + 2, ' ') << "\"left\": ";
    serialize_node(out, node->lhs.get(), indent + 2);
    out << ",\n";
    out << std::string(indent + 2, ' ') << "\"right\": ";
    serialize_node(out, node->rhs.get(), indent + 2);
    out << "\n" << std::string(indent, ' ') << "}";
}

void ASTSerializer::serialize_unary_expr(std::ostringstream& out, const UnaryExpr* node,
                                         int indent) {
    out << "{\n";
    out << std::string(indent + 2, ' ') << "\"kind\": \"UnaryExpr\",\n";
    out << std::string(indent + 2, ' ') << "\"op\": \"" << prefix_op_to_string(node->op) << "\",\n";
    out << std::string(indent + 2, ' ') << "\"operand\": ";
    serialize_node(out, node->operand.get(), indent + 2);
    out << "\n" << std::string(indent, ' ') << "}";
}

void ASTSerializer::serialize_assign_expr(std::ostringstream& out, const AssignExpr* node,
                                          int indent) {
    out << "{\n";
    out << std::string(indent + 2, ' ') << "\"kind\": \"AssignExpr\",\n";
    out << std::string(indent + 2, ' ') << "\"target\": ";
    serialize_node(out, node->target.get(), indent + 2);
    out << ",\n";
    out << std::string(indent + 2, ' ') << "\"value\": ";
    serialize_node(out, node->value.get(), indent + 2);
    out << "\n" << std::string(indent, ' ') << "}";
}

void ASTSerializer::serialize_call_expr(std::ostringstream& out, const CallExpr* node, int indent) {
    out << "{\n";
    out << std::string(indent + 2, ' ') << "\"kind\": \"CallExpr\",\n";
    out << std::string(indent + 2, ' ') << "\"callee\": ";
    serialize_node(out, node->callee.get(), indent + 2);
    out << ",\n";
    out << std::string(indent + 2, ' ') << "\"args\": [\n";
    for (size_t i = 0; i < node->args.size(); ++i) {
        out << std::string(indent + 4, ' ');
        serialize_node(out, node->args[i].get(), indent + 4);
        if (i < node->args.size() - 1)
            out << ",";
        out << "\n";
    }
    out << std::string(indent + 2, ' ') << "]\n";
    out << std::string(indent, ' ') << "}";
}

void ASTSerializer::serialize_identifier_expr(std::ostringstream& out, const IdentifierExpr* node,
                                              int indent) {
    out << "{\n";
    out << std::string(indent + 2, ' ') << "\"kind\": \"Identifier\",\n";
    out << std::string(indent + 2, ' ') << "\"name\": \"" << node->name << "\"\n";
    out << std::string(indent, ' ') << "}";
}

void ASTSerializer::serialize_literal_expr(std::ostringstream& out, const LiteralExpr* node,
                                           int indent) {
    out << "{\n";
    out << std::string(indent + 2, ' ') << "\"kind\": \"Literal\",\n";
    out << std::string(indent + 2, ' ') << "\"value\": ";

    std::visit(
        [&out](const auto& v) {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                out << "null";
            } else if constexpr (std::is_same_v<T, std::string>) {
                out << "\"" << ASTSerializer::escape_json_string(v) << "\"";
            } else if constexpr (std::is_same_v<T, bool>) {
                out << (v ? "true" : "false");
            } else if constexpr (std::is_same_v<T, char>) {
                out << "\"" << v << "\"";
            } else {
                out << v;
            }
        },
        node->value);

    out << "\n" << std::string(indent, ' ') << "}";
}
